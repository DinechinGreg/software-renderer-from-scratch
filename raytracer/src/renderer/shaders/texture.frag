#version 330 core

uniform sampler2D tex;

in vec2 v2f_tex_coord;

out vec4 f_color;

void main()
{
    f_color = texture(tex, v2f_tex_coord);
} 