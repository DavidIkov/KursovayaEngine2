#version 330 core
out vec4 PixelColor;

uniform vec3 u_Color;

void main()
{
    PixelColor=vec4(u_Color,1);
}