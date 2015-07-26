#version 430

const int MAX_LIGHTS = 10;
const int MAX_MATERIALS = 10;

struct LightProperties
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
	//float shininess;
	float strength;

	vec3 position; //For directional lights this is instead the direction the light points, yup terrible naming I know. 
	vec3 normal; //Direction spotlights are pointing
	float angle; //How many degrees left and right of the light normal does this light `shine` it's in cos(theta) form

	float linearAtten; //= 1.0;
	float quadAtten; // = 1.0;
	float constAtten; // = 0.1;
	float spotponent; // = 2.0;
};

struct MaterialProperties
{
	vec3 emission;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

in vec3 colour;
in vec2 texCoord;
in vec3 normalInterp;
in vec3 lightPosInterp; 
in vec3 vertPos;
in vec3 camDirection;
in vec3 lightNormalTransform;
in mat3 NormalMatrix;
in mat4 modelCamMatrix;
flat in int enabledMaterials;

out vec4 fColor;

uniform sampler2D tex;
uniform sampler2D normalMap;

uniform int numEnabledLights;
uniform UniformBlock
{
	LightProperties lights[MAX_LIGHTS];
	MaterialProperties materials[MAX_MATERIALS];
};

void main()
{
	vec3 scatteredLight = vec3(0.0);
	vec3 reflectedLight = vec3(0.0);
	vec4 outColour;
	int i;

	vec3 totalEmission = vec3(0.0);
	vec3 totalDiffuse = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);
	float totalShininess = 0.0;

	for(i = 0; i < MAX_MATERIALS; i++)
	{
		if((enabledMaterials & i) != 0)
		{
			totalEmission += materials[i].emission;
			totalDiffuse += materials[i].diffuse;
			totalSpecular += materials[i].specular;
			totalShininess += materials[i].shininess;
		}
	}

	totalEmission = min(totalEmission, vec3(1.0));
	totalDiffuse = min(totalDiffuse, vec3(1.0));
	totalSpecular = min(totalSpecular, vec3(1.0));

	for(i = 0; i < numEnabledLights; i++)
	{
		if(lights[i].isEnabled)
		{

			vec3 normal = normalize(normalInterp);
			vec3 lightDirection;
			float attenuation = 0.0;

			if(lights[i].constAtten > 0.0)
			{
				attenuation = (1.0 / lights[i].constAtten);
			}

			if(lights[i].isPointlight || lights[i].isSpotlight)
			{
				vec4 lightTransformed = modelCamMatrix * vec4(lights[i].position, 1.0);
				vec3 lightVector = vec3(lightTransformed) - vertPos;
				float lightLength = length(lightVector);
				lightDirection = normalize(lightVector);
				vec3 lightNormal = normalize(NormalMatrix * lights[i].normal);

				if(lights[i].linearAtten > 0.0) //Don't want divide by zero errors
				{
					attenuation += (lights[i].linearAtten / lightLength);
				}

				if(lights[i].quadAtten > 0.0) //Don't want divide by zero errors
				{
					attenuation += lights[i].quadAtten / (lightLength * lightLength);
				}

				if(lights[i].isSpotlight)
				{
					float lightDot = dot(lightDirection, lightNormal);
					if(lightDot > lights[i].angle)
					{
						attenuation = 0.0;
					}
					else
					{
						attenuation *= pow(lightDot, lights[i].spotponent);
					}
				}
			}
			else
			{
				lightDirection = normalize(NormalMatrix * lights[i].position);
			}

			float diffuse = 0.0;
			if(lights[i].enableDiffuse)
			{
				diffuse = dot(lightDirection, normal);
			}

			float specular = 0.0f;
			if(lights[i].enableSpecular && diffuse > 0.0)
			{
				if(lights[i].specularMode)
				{
					vec3 reflectDir = reflect(-lightDirection, normal);
					float specAngle = max(dot(reflectDir, camDirection), 0.0);
					specular = pow(specAngle, totalShininess);
				}
				else
				{
					//shininess = 20.0;
					vec3 halfVector = normalize(camDirection + lightDirection);
					float specAngle = max(dot(halfVector, normal), 0.0);
					specular = pow(specAngle, totalShininess);
				}
			}

			scatteredLight += (lights[i].ambientLight * attenuation) + max((diffuse * lights[i].diffuseLight), 0.0) * attenuation;
			reflectedLight += max((specular * lights[i].specularLight), 0.0f) * attenuation;
			
		}
	}
	outColour = vec4(texture(tex, texCoord).xyz * scatteredLight + reflectedLight, 1.0);
	fColor = min(outColour, vec4(1.0));

}
