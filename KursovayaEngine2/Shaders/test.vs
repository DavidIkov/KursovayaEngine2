#version 330 core

layout (location=0) in vec3 i_VertexLocalPosition;
layout (location=2) in vec3 i_FaceNormalVector;
layout (location=3) in vec2 i_TextureCords;

out vec3 t_PosInSpace;
out vec2 t_TextureCords;
out vec3 t_NormalVector;

uniform vec3 u_ObjectPosition;
uniform mat3 u_ObjectRotationMatrix;
uniform vec3 u_ObjectScale;

uniform vec3 u_CameraPosition;
uniform mat3 u_InversedCameraRotationMatrix;//inversed for optimization

uniform mat4 u_ProjectionMatrix;

void main(){
	t_PosInSpace=u_ObjectPosition+u_ObjectRotationMatrix*(i_VertexLocalPosition*u_ObjectScale);
	t_NormalVector=normalize(u_ObjectRotationMatrix*i_FaceNormalVector/u_ObjectScale);
	t_TextureCords=i_TextureCords;

	vec3 posFromCamera=u_InversedCameraRotationMatrix*(t_PosInSpace-u_CameraPosition);

	gl_Position=u_ProjectionMatrix*vec4(posFromCamera,1);
}