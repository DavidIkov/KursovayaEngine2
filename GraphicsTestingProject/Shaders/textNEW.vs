#version 330 core
layout (location = 0) in vec2 i_VertPos;
layout (location = 1) in vec2 i_TextTexturePos;

out vec2 t_TextTexturePos;

void main()
{
    gl_Position = vec4(i_VertPos, 0.0, 1.0);
    t_TextTexturePos = i_TextTexturePos;
}  