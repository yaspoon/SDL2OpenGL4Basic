#version 430 core

in vec3 colour;
in vec3 lightDirection;
out vec4 fColor;

void main()
{
	vec3 diffuseLight = vec3(1.0, 1.0, 1.0);
	vec3 ambientLight = vec3(1.0, 1.0, 1.0);
	float strength = 0.5;
	vec3 lightingCalc = colour * (ambientLight * strength);
	fColor = vec4(lightingCalc, 1.0);
}
