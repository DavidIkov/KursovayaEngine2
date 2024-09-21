#version 330 core
out vec4 o_PixelColor;

in vec2 t_TextTexturePos;
uniform sampler2D u_TextTexture;

void main()
{   float symbolColor=texture(u_TextTexture, t_TextTexturePos).r;
    o_PixelColor=vec4(0,0,0,symbolColor);
    //o_PixelColor=vec4(0,0,0,1);
}