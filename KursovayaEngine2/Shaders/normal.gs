#version 330 core

layout (triangles) in;
//layout (line_strip, max_vertices = 2) out;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 normal;
} gs_in[];

uniform mat4 u_ProjectionMatrix;

uniform mat3 u_InversedCameraRotationMatrix;

void main() {

    for (int i=0;i<3;i++){
        gl_Position=u_ProjectionMatrix*gl_in[i].gl_Position;
        EmitVertex();
        gl_Position=u_ProjectionMatrix*(gl_in[i].gl_Position+vec4(u_InversedCameraRotationMatrix*gs_in[i].normal/20,0));
        EmitVertex();
        EndPrimitive();
    }
    //vec4 pos=(gl_in[0].gl_Position+gl_in[1].gl_Position+gl_in[2].gl_Position)/3;
    //
    //gl_Position = u_ProjectionMatrix*pos;
    //EmitVertex();
    //gl_Position = u_ProjectionMatrix*(pos+vec4(u_InversedCameraRotationMatrix*gs_in[0].normal/20,0));
    //EmitVertex();
    //
    //EndPrimitive();
}