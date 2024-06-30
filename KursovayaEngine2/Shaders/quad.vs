#version 330 core

layout (location=0) in vec2 i_Pos;

out vec2 t_TextureCords;

void main(){
	t_TextureCords=(i_Pos+1)/2;
	gl_Position=vec4(i_Pos,0,2);
}