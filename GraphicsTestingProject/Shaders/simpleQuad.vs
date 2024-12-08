#version 330 core

layout (location=0) in vec2 i_Pos;
layout(location=1) in float i_Color;

out float t_Color;

void main(){
	t_Color=i_Color;
	gl_Position=vec4(i_Pos,0,1);
}