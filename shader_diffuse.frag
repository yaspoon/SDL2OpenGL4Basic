#version 150

in vec3 colour;
in vec3 normal;
in vec3 lightDirection;

out vec4 fColor;

uniform bool enableDiffuse;
uniform bool enableSpecular;

uniform vec3 camPosition;
void main()
{
	vec3 ambientLight = vec3(0.5, 0.0, 0.0);
	vec3 diffuseLight = vec3(0.5, 0.0, 0.0);

	float diffuse = 0.0;
	if(enableDiffuse)
	{
		diffuse = dot(lightDirection, normal);
	}

	vec3 lValue = colour * ambientLight + (diffuse * diffuseLight);
	fColor = min(vec4(lValue, 1.0), vec4(1.0, 0.0, 0.0, 1.0f));
}
