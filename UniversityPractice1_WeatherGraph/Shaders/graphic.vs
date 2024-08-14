#version 330 core
layout (location = 0) in vec2 i_PosInScreen;
layout (location = 1) in vec2 i_GraphLocalPos;

out vec2 t_GraphCords;

void main()
{
    t_GraphCords=i_GraphLocalPos;
    gl_Position = vec4(i_PosInScreen, 0.0, 1.0);
}