#version 330 core

layout (location=0) in vec3 i_VertexLocalPosition;
layout (location=1) in vec3 i_SmoothedNormal;

uniform vec3 u_ObjectPosition;
uniform mat3 u_ObjectRotationMatrix;
uniform vec3 u_ObjectScale;
uniform float u_OutlineScale;

uniform vec3 u_CameraPosition;
uniform mat3 u_InversedCameraRotationMatrix;//inversed for optimization

uniform mat4 u_ProjectionMatrix;

void main(){
	vec3 PosInSpace=u_ObjectPosition+u_ObjectRotationMatrix*(i_VertexLocalPosition*u_ObjectScale+normalize(i_SmoothedNormal/u_ObjectScale)*u_OutlineScale);

	vec3 posFromCamera=u_InversedCameraRotationMatrix*(PosInSpace-u_CameraPosition);

	gl_Position=u_ProjectionMatrix*vec4(posFromCamera,1);
}