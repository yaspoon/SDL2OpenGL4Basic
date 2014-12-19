#version 430 core

layout(location=0) in vec4 vPosition;
uniform int rotStep;
const float PI = 3.14;

mat4 projection_mat = mat4(1.0f);
vec4 proj1 = vec4(

void main()
{
	float radians = (float(rotStep) * PI) / 180.0f;
	vec2 test = vec2(vPosition);
	test.x = vPosition.x * cos(radians) - vPosition.y * sin(radians);
	test.y = vPosition.x * sin(radians) + vPosition.y * cos(radians);

	gl_Position = vec4(test, 0.0f, 1.0f);
}
