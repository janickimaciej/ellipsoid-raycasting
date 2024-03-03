#version 330 core

layout (location = 0) in vec3 inPosition;

out vec2 texturePosition;

void main()
{
	texturePosition = vec2((inPosition.x + 1) / 2, (inPosition.y + 1) / 2);
	gl_Position = vec4(inPosition, 1);
}
