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
	vec3 colour;
	vec3 ambientLight;
	vec3 diffuseLight;
	vec3 specularLight;
	float strength;

	vec4 position; //Where is this thing in SPAAACCCEEEEEEEEEEE (Our space we defined)
	vec4 normal; //Direction light is pointing (It's actually the opposite, imagine the normal is attached to the back of the light! pointing away from it), it makes the math easier
	float angle; //How many degrees left and right of the light normal does this light `shine` it's in cos(theta) form

	float linearAtten; 
	float quadAtten;
	float constAtten;
	float spotponent; //Spot light fade (Or hardness)
};

struct MaterialProperties
{
	vec3 emission;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec3 colour;
	bool hasTexture;
	bool enabled;
};

in SceneData
{
	vec2 texCoord;
	vec4 normal;
	vec4 vertPos;
	vec4 camDirection;
	mat4 normalMatrix;
	mat4 camMatrix;
	mat4 sModelMatrix;
};

out vec4 fColor;

uniform sampler2D tex;
uniform sampler2D normalMap;

uniform int numEnabledLights;
uniform UniformBlock
{
	LightProperties lights[MAX_LIGHTS];
	MaterialProperties materials[MAX_MATERIALS];
};

layout(binding=0, rgba32f) restrict uniform imageBuffer transformedNormals;

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
	vec3 colour = vec3(1.0);
	
	bool hasTexture = true; /*Sigh these shaders just keep getting more gross*/

	for(i = 0; i < MAX_MATERIALS; i++)
	{
		if(materials[i].enabled)
		{
			totalEmission += materials[i].emission;
			totalDiffuse += materials[i].diffuse;
			totalSpecular += materials[i].specular;
			totalShininess += materials[i].shininess;
			if(!hasTexture)
			{
				colour += materials[i].colour;
				hasTexture = false;
			}
		}
	}
	
	if(hasTexture)
	{
		colour = texture(tex, texCoord).xyz;
	}

	totalEmission = min(totalEmission, vec3(1.0));
	totalDiffuse = min(totalDiffuse, vec3(1.0));
	totalSpecular = min(totalSpecular, vec3(1.0));

	for(i = 0; i < numEnabledLights; i++)
	{
		if(lights[i].isEnabled)
		{
			vec4 lightDirection;
			float attenuation = 1.0;

			if(lights[i].constAtten > 0.0)
			{
				attenuation /= lights[i].constAtten;
			}

			if(lights[i].isPointlight || lights[i].isSpotlight)
			{
				vec4 lightTransformed = camMatrix * lights[i].position;
				vec4 lightVector = lightTransformed - vertPos;
				float lightLength = length(lightVector);
				lightDirection = normalize(lightVector);
				vec4 lightNormal = normalize(normalMatrix * lights[i].normal);
				//vec4 lightNormal = lights[i].normal;

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
					float spotCos = dot(lightDirection, -lightNormal);
					if(spotCos < lights[i].angle)
					{
						attenuation = 0.0;
					}
					else
					{
						//add fade!
						attenuation *= pow(spotCos, lights[i].spotponent);
					}
				}
			}
			else
			{
				lightDirection = normalize(normalMatrix * lights[i].normal);
			}

			float diffuse = 0.0;
			if(lights[i].enableDiffuse)
			{
				diffuse = clamp(dot(lightDirection, normal), 0, 1);
			}

			float specular = 0.0f;
			if(diffuse > 0.0)
			{
				if(lights[i].enableSpecular && totalShininess > 0.0)
				{
					if(lights[i].specularMode)
					{
						vec4 reflectDir = reflect(-lightDirection, normal);
						float specAngle = max(dot(reflectDir, camDirection), 0.0);
						specular = pow(specAngle, totalShininess);
					}
					else
					{
						//shininess = 20.0;
						vec4 halfVector = normalize(camDirection + lightDirection);
						float specAngle = max(dot(halfVector, normal), 0.0);
						specular = pow(specAngle, totalShininess) * lights[i].strength;
					}
				}
			}

			//scatteredLight += (lights[i].ambientLight * attenuation) + (max((diffuse * lights[i].diffuseLight), 0.0) * attenuation);
			scatteredLight += lights[i].ambientLight + lights[i].colour * diffuse * attenuation;
			//reflectedLight += max((specular * lights[i].specularLight), 0.0f) * attenuation;
			reflectedLight += lights[i].colour * specular * attenuation;
			
		}
	}
	//outColour = vec4(colour * scatteredLight + reflectedLight, 1.0);
	vec3 rgb = min(colour.rgb * scatteredLight + reflectedLight, vec3(1.0));
	fColor = vec4(rgb, 1.0);

}
