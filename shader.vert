#version 150 

//layout(location=0) in vec4 vPosition;
//layout(location=1) in vec3 vColour;
//layout(location=2) in vec3 vNormal;
in vec4 vPosition;
in vec3 vColour;
in vec3 vNormal;

out vec3 colour;
out vec3 normalInterp;
out vec3 vertPos;
out vec3 lightPosition; 

uniform mat4 vprojectionMat;
uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;
uniform vec3 lightPos;

uniform float angle;

void main()
{
	vec3 vLight = lightPos - vec3(vPosition);
	vec3 nLight = normalize(vLight);
	//lighting = 1.0; //dot(normal, nLight);
	colour = vColour;
	
	float radians = angle * (3.14 / 180.0);
	
	mat4 modelCamera = cameraMatrix * modelMatrix;
	mat3 NormalMatrix = transpose(inverse(mat3(modelCamera)));
	
	normalInterp = NormalMatrix * vNormal;
	vec4 transformedPosition = modelCamera * vPosition;
	vertPos = vec3(transformedPosition) / transformedPosition.w;
	
	vec4 tmp = vec4(0.0, 0.0, 30.0, 1.0);
	vec4 lightPos = vec4(1.0);
	lightPos.x = tmp.x * cos(radians) + tmp.z * -sin(radians);
	lightPos.y = tmp.y;
	lightPos.z = tmp.x * sin(radians) + tmp.z * cos(radians);
	
	lightPosition = vec3(modelCamera * lightPos);

	gl_Position = vprojectionMat * transformedPosition;
}
