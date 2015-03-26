#version 430 core

in vec3 colour;
in vec2 texCoord;
in float lighting;

out vec4 fColor;

uniform sampler2D tex;

void main()
{
	//fColor = texture(tex, texCoord) * lighting;
	fColor = vec4(1.0, 0.0, 0.0, 1.0);
	//fColor = vec4(texCoord, 0.0, 1.0);
	//fColor = vec4(colour, 1.0);
}
