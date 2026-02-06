#version 420 core

layout (location = 0) in vec3 inPos;

out vec2 texturePos;

void main()
{
	texturePos = vec2((inPos.x + 1) / 2, (inPos.y + 1) / 2);
	gl_Position = vec4(inPos, 1);
}
