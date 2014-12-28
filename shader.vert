#version 430 core

layout(location=0) in vec4 vPosition;
layout(location=1) in vec3 vColour;

out vec3 colour;

uniform mat4 vprojectionMat;
uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;

void main()
{
	colour = vColour;

	gl_Position = vprojectionMat * (cameraMatrix * (modelMatrix * vPosition));
}
