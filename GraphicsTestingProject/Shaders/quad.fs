#version 330 core

out vec4 PixelColor;

in vec2 t_TextureCords;
uniform sampler2D u_Texture;

void main(){
	PixelColor=texture(u_Texture,t_TextureCords);
}