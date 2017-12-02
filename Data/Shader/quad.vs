#version 140


in vec2 position;
in vec2 uv;

out vec2 vUV;

void main()
{
   	vUV = vec2(uv.x,1-uv.y);
   	gl_Position = vec4(position.xy,0,1);
}