#version 150

in vec3 colour;
in vec3 normalInterp;
in vec3 vertPos;
in vec3 lightPosition;
out vec4 fColor;

uniform vec3 camPosition;
void main()
{
	vec3 ambientLight = vec3(0.2, 0.0, 0.0);
	vec3 lightColour = vec3(1.0, 1.0, 1.0);
	float strength = 10.0;
	float shininess = 16.0;
	
	
	vec3 normal = normalize(normalInterp);
	vec3 lightDirection = lightPosition - vertPos;
        float lightLength = length(lightDirection);

	lightDirection = lightDirection / lightLength;

	float attenuation = 1.0;
	float diffuse = max(dot(normal, lightDirection), 0.0);
	float specular = 0.0f;
	
	if(diffuse > 0.0)
	{
		vec3 camDirection = normalize(camPosition - vertPos);
		vec3 halfDir = normalize(lightDirection + camDirection);
		
		float specAngle = max(dot(halfDir, normal), 0.0);
		
		specular = pow(specAngle, shininess) * strength;
	}

	vec3 scatteredLight = ambientLight + lightColour * diffuse * attenuation;
	vec3 reflectedLight = lightColour * specular * attenuation; 
	
	vec3 lightingCalc = colour * min(vec3(1.0), colour * scatteredLight + reflectedLight);
	fColor = vec4(lightingCalc, 1.0);
}
