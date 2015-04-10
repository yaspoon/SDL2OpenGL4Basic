#version 150 

in vec4 vPosition;
in vec3 vColour;
in vec3 vNormal;

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

	lightDirection = normalize(vec3(0.0, 0.0, -5.0) - vertPos);

	gl_Position = vprojectionMat * transformedPosition;
}
