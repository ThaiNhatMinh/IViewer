#version 140

uniform sampler2D Texture;

in vec2 vUV;

out vec4 Out_Color;

void main()
{
	Out_Color = texture( Texture, vUV.st);
}