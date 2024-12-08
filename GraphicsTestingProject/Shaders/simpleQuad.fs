#version 330 core

out vec4 o_PixelColor;
in float t_Color;

void main()
{
    o_PixelColor=vec4(t_Color, t_Color,t_Color,1.f);
}