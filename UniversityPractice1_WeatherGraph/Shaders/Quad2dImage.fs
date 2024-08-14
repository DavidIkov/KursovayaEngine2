#version 330 core
out vec4 PixelColor;

uniform sampler2D u_Tex;

in vec2 t_TexCords;

void main()
{
    PixelColor=texture(u_Tex,t_TexCords);
}