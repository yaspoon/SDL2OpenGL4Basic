#version 430 core

layout(location=0) in vec4 vPosition;
layout(location=1) in vec3 vColour;

out vec3 colour;
out vec3 lightDirection;

uniform mat4 vprojectionMat;
uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;
uniform vec3 lightPosition;

void main()
{
	colour = vColour;
	
	vec4 transformedPosition = cameraMatrix * (modelMatrix * vPosition);
	
	lightDirection = normalize(lightPosition - transformedPosition.xyz);

	gl_Position = vprojectionMat * transformedPosition;
}
