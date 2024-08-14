#version 330 core
layout (location = 0) in vec2 i_LocalPos;
layout (location = 1) in vec2 i_TextTexturePos;

out vec2 t_TextTexturePos;

uniform vec2 u_PosOnScreen;

void main()
{
    gl_Position = vec4(u_PosOnScreen+i_LocalPos, 0.0, 1.0);
    t_TextTexturePos = i_TextTexturePos;
}  