#version 300 es

precision mediump float;

in vec2 tex;

uniform sampler2D s_texture;

layout (location = 0) out vec4 color;

void main ()
{
	color = texture (s_texture, tex);
}
