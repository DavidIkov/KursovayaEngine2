#version 330 core

uniform vec3 u_LineColor;
uniform sampler1D u_DataTex;
in vec2 t_GraphCords;


void main()
{
	gl_FragColor=vec4(0,0,0,0);
	float size=0.3;
	float v=texture(u_DataTex,t_GraphCords.x).r;
	if (t_GraphCords.y>v) return;
	gl_FragColor=vec4(u_LineColor,1-(v-t_GraphCords.y)/size);
}