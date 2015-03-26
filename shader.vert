#version 430 core

layout(location=0) in vec4 vPosition;
layout(location=1) in vec3 vColour;
layout(location=2) in vec2 tCoord;
layout(location=3) in vec3 normal;

out vec3 colour;
out vec2 texCoord;
out float lighting;

uniform mat4 vprojectionMat;
uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;
uniform vec3 lightPos;

void main()
{
	vec3 vLight = lightPos - vec3(vPosition);
	vec3 nLight = normalize(vLight);
	lighting = 1.0; //dot(normal, nLight);
	colour = vColour;
	texCoord = vec2(1.0, 1.0);tCoord;

	gl_Position = vprojectionMat * (cameraMatrix * (modelMatrix * vPosition));
}
