#version 330 core

layout (location=0) in vec3 i_VertexLocalPosition;
layout (location=1) in vec3 i_NormalVector;

out VS_OUT{
	vec3 normal;
} vs_out;

uniform vec3 u_ObjectPosition;
uniform mat3 u_ObjectRotationMatrix;
uniform vec3 u_ObjectScale;

uniform vec3 u_CameraPosition;
uniform mat3 u_InversedCameraRotationMatrix;//inversed for optimization

void main(){
	vec3 posInSpace=u_ObjectPosition+u_ObjectRotationMatrix*(i_VertexLocalPosition*u_ObjectScale);
	vs_out.normal=normalize(u_ObjectRotationMatrix*i_NormalVector/u_ObjectScale);

	vec3 posFromCamera=u_InversedCameraRotationMatrix*(posInSpace-u_CameraPosition);

	gl_Position=vec4(posFromCamera,1);
}