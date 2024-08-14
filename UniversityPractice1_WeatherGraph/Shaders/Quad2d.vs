#version 330 core
layout (location = 0) in vec2 i_LocalPos;

uniform vec2 u_Pos;
uniform vec2 u_Size;
uniform vec2 u_LocalOffset;

void main()
{
    gl_Position = vec4(u_Pos+(i_LocalPos-u_LocalOffset)*u_Size, 0.0, 1.0);
}