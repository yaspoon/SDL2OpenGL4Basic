#version 430

layout(location=0) in vec4 vPosition;
layout(location=1) in vec3 vColour;
layout(location=2) in vec3 vNormal;
layout(location=3) in vec2 vTexCoord;

out vec3 colour;
out vec2 texCoord;
out vec3 normalInterp;
out vec3 lightPosInterp; 
out vec3 vertPos;
out vec3 camDirection;

uniform vec3 lightPos;
uniform mat4 vprojectionMat;
uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;
uniform vec3 camPosition;

void main()
{
	colour = vColour;
	texCoord = vTexCoord;
	
	mat4 modelCamera = cameraMatrix * modelMatrix;
	mat3 NormalMatrix = transpose(inverse(mat3(modelCamera)));
	
	normalInterp = NormalMatrix * vNormal;
	vec4 transformedPosition = modelCamera * vPosition;
	vertPos = vec3(transformedPosition);
	vec4 lightTransform = modelCamera * vec4(lightPos, 1.0);//modelCamera * vec4(0.0, 0.0, -20.0, 1.0);

	lightPosInterp = vec3(lightTransform);
	camDirection = normalize(camPosition - vertPos);

	gl_Position = vprojectionMat * transformedPosition;
}
