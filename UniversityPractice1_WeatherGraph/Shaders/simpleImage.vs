#version 330 core
layout (location = 0) in vec2 i_LocalPos;
layout (location = 1) in vec2 i_TextureCords;

out vec2 t_TexCords;

void main()
{
    t_TexCords=i_TextureCords;
    gl_Position = vec4(i_LocalPos, 0.0, 1.0);
}