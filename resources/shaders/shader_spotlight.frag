#version 430

in vec3 colour;
in vec2 texCoord;
in vec3 normalInterp;
in vec3 lightPosInterp;
in vec3 vertPos;
in vec3 camDirection;

out vec4 fColor;

uniform bool enableDiffuse;
uniform bool enableSpecular;
uniform bool specularMode;

uniform sampler2D tex;
uniform sampler2D normalMap;

void main()
{
	float shininess = 4.0;
	float strength = 10.0;
	vec3 ambientLight = vec3(0.6, 0.6, 0.6);
	vec3 lightColour = vec3(1.0, 1.0, 1.0);
	vec3 diffuseLight = vec3(0.4, 0.4, 0.4);
	vec3 specularLight = vec3(1.0, 1.0, 1.0);

	vec3 normal = normalize(normalInterp);
	vec3 lightDirection = normalize(lightPosInterp - vertPos);

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
			shininess = 20.0;
			vec3 halfVector = normalize(camDirection + lightDirection);
			float specAngle = max(dot(halfVector, normal), 0.0);
			specular = pow(specAngle, shininess);
		}
	}

	vec3 lValue = (texture(tex, texCoord).xyz * ambientLight) + max((diffuse * diffuseLight), 0.0) + max((specular * specularLight), 0.0f);
	fColor = vec4(lValue, 1.0);
	//vec3 scatteredLight = ambientLight + max(lightColour * diffuse, vec3(0.0));
	//vec3 reflectedLight = lightColour * specular * strength;
	//fColor = min(vec4(colour * scatteredLight + reflectedLight, 1.0), vec4(1.0));
}
