#version 430

layout(location=0) in vec4 vPosition;
layout(location=1) in vec3 vColour;
layout(location=2) in vec4 vNormal;
layout(location=3) in vec2 vTexCoord;

out SceneData
{
	vec2 texCoord;
	vec4 normal;
	vec4 vertPos;
	vec4 camDirection;
	mat4 normalMatrix;
	mat4 camMatrix;
	mat4 sModelMatrix;
};

uniform mat4 vprojectionMat;
uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;
uniform vec4 camPosition;

void main()
{
	texCoord = vTexCoord;	

	sModelMatrix = modelMatrix;
	mat4 modelCamMatrix = cameraMatrix * modelMatrix;
	camMatrix = cameraMatrix;
	normalMatrix = transpose(inverse(modelMatrix));
	
	normal = normalize(normalMatrix * vNormal);

	vertPos = modelCamMatrix * vPosition;

	camDirection = normalize(camPosition - vertPos);

	gl_Position = vprojectionMat * vertPos;
}
