#version 430

layout(location=0) in vec4 vPosition;
layout(location=1) in vec3 vColour;
layout(location=2) in vec3 vNormal;

out vec3 colour;
out vec3 normal;
out vec3 lightDirection; 

uniform vec3 lightPos;
uniform mat4 vprojectionMat;
uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;

void main()
{
	colour = vColour;
	
	mat4 modelCamera = cameraMatrix * modelMatrix;
	mat3 NormalMatrix = transpose(inverse(mat3(modelCamera)));
	
	normal = normalize(NormalMatrix * vNormal);
	vec4 transformedPosition = modelCamera * vPosition;
	vec3 vertPos = vec3(transformedPosition);
	vec4 lightTransform = modelCamera * vec4(lightPos, 1.0);//modelCamera * vec4(0.0, 0.0, -20.0, 1.0);

	lightDirection = normalize(vec3(lightTransform) - vertPos);

	gl_Position = vprojectionMat * transformedPosition;
}
