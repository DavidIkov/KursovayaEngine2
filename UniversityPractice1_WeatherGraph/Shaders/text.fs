#version 330 core
out vec4 PixelColor;

in vec2 t_TextTexturePos;

uniform sampler2D u_TextTexture;

void main()
{   float symbolColor=texture(u_TextTexture, t_TextTexturePos).r;
    PixelColor=vec4(0,0,0,symbolColor);
}