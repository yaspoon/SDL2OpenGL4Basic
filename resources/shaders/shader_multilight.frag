#version 430

const int MAX_LIGHTS = 10;

uniform LightProperties
{
	bool isEnabled;
	bool enableDiffuse;
	bool enableSpecular;
	bool isSpotlight;
	bool isPointlight;
	bool specularMode;
	vec3 ambientLight;
	vec3 diffuseLight;
	vec3 specularLight;
	float shininess;
	float strength;

	vec3 position; //For directional lights this is instead the direction the light points, yup terrible naming I know. 
	vec3 normal; //Direction spotlights are pointing
	float angle; //How many degrees left and right of the light normal does this light `shine` it's in cos(theta) form

	float linearAtten; //= 1.0;
	float quadAtten; // = 1.0;
	float constAtten; // = 0.1;
	float spotponent; // = 2.0;
};

in vec3 colour;
in vec2 texCoord;
in vec3 normalInterp;
in vec3 lightPosInterp; 
in vec3 vertPos;
in vec3 camDirection;
in vec3 lightNormalTransform;
in mat3 NormalMatrix;

out vec4 fColor;

uniform sampler2D tex;
uniform sampler2D normalMap;
//uniform LightProperties lights[MAX_LIGHTS];

void main()
{
	if(!isEnabled)
	{
		fColor = vec4(0.0);
	}
	else
	{

		vec3 normal = normalize(normalInterp);
		vec3 lightDirection;
		float attenuation = 1.0;

		if(isPointlight || isSpotlight)
		{
			vec3 lightVector = position - vertPos;
			float lightLength = length(lightVector);
			lightDirection = normalize(lightVector);

			float lightDot = dot(lightDirection, normal);

			attenuation = 1.0 / (constAtten * lightLength); // (constAtten + linearAtten * lightLength + quadAtten * lightLength * lightLength);

			if(isSpotlight && (lightDot < angle))
			{
				attenuation = 0.0;
			}
			else
			{
				attenuation *= pow(lightDot, spotponent);
			}
		}
		else
		{
			lightDirection = normalize(NormalMatrix * position);
		}

		float diffuse = 0.0;
		if(enableDiffuse)
		{
			diffuse = dot(lightDirection, normal);
		}

		float specular = 0.0f;
		if(enableSpecular && diffuse > 0.0)
		{
			if(specularMode)
			{
				vec3 reflectDir = reflect(-lightDirection, normal);
				float specAngle = max(dot(reflectDir, camDirection), 0.0);
				specular = pow(specAngle, shininess);
			}
			else
			{
				//shininess = 20.0;
				vec3 halfVector = normalize(camDirection + lightDirection);
				float specAngle = max(dot(halfVector, normal), 0.0);
				specular = pow(specAngle, shininess);
			}
		}

		vec3 test = vec3(0.5);

		vec3 scatteredLight = ambientLight + max((diffuse * test), 0.0) * attenuation;
		vec3 reflectedLight = max((specular * specularLight), 0.0f) * attenuation;
		fColor = min(vec4(texture(tex, texCoord).xyz * scatteredLight + reflectedLight, 1.0), vec4(1.0));
	}

}
