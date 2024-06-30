#version 330 core

out vec4 PixelColor;

in vec3 t_PosInSpace;
in vec2 t_TextureCords;
in vec3 t_NormalVector;

uniform sampler2D u_tex1;
uniform sampler2D u_tex2;
uniform vec3 u_CameraPosition;

struct VisualData{
	float Shininess;
	float LightSourceReflectionMaxAngle;

	//theese two is responsible for lighting which is calculated for surfaces
	float MinColorMultiplierForSurfaceLighting;
};
uniform VisualData u_VisualData;

uniform vec3 u_LightPos;

void main() {
	PixelColor=mix(texture(u_tex1,t_TextureCords),texture(u_tex2,t_TextureCords),0.f);

	{//basic lighting
		float colorMultiplier=(dot(t_NormalVector,normalize(u_LightPos-t_PosInSpace))+1)/2;
		float smoothedOutColorMult=colorMultiplier*(1-u_VisualData.MinColorMultiplierForSurfaceLighting)+u_VisualData.MinColorMultiplierForSurfaceLighting;

		PixelColor=vec4(PixelColor.xyz*smoothedOutColorMult,PixelColor.w);
	}



	{//light source white effect

		vec4 diffToWhite=vec4(1,1,1,1)-PixelColor;
		vec3 reflectedVec=t_PosInSpace-u_LightPos-t_NormalVector*2*dot(t_NormalVector,t_PosInSpace-u_LightPos)/dot(t_NormalVector,t_NormalVector);
		float dotProd=dot(normalize(reflectedVec),normalize(u_CameraPosition-t_PosInSpace));
		
		float maxAngle=cos(u_VisualData.LightSourceReflectionMaxAngle);
		float linearMult=(dotProd-maxAngle)/(1-maxAngle);
		
		
		if (dotProd>maxAngle) PixelColor=PixelColor+diffToWhite*pow(linearMult,u_VisualData.Shininess);
	}
}