#version 140

uniform vec2 screen;
uniform float scale;
in vec2 Position;
in vec2 UV;

out vec2 Frag_UV;
out vec4 Frag_Color;

void main()
{
   	Frag_UV = vec2(UV.x,1-UV.y);
	vec2 halfscreen = screen/vec2(2);
	vec2 pos = Position-halfscreen;
	pos /= halfscreen;

	gl_Position = vec4(pos*scale,0,1);
}
