#version 330 core

in vec2 texturePosition;

uniform sampler2D textureSampler;

out vec4 outColor;

void main()
{
	outColor = vec4(texture(textureSampler, texturePosition).xyz, 1);
}
