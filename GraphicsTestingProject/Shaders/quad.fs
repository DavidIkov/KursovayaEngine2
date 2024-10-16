#version 330 core

out vec4 PixelColor;

in vec2 t_TextureCords;
uniform sampler2D u_Texture;
uniform mat3 u_CameraRotation;
uniform vec3 u_CameraPosition;
uniform vec2 u_CameraResolutionSize;
uniform float u_EffectMult;
uniform	float u_Time;

float radius=0.1f;
float dist(vec3 pos){
	return length(vec3(
		pos.x-0.4f-0.8f*floor(pos.x/0.8f),
		pos.y-1.f-2*floor(pos.y/2),
		pos.z-1.f-2*floor(pos.z/2)
	))-radius;
	/*vec3 localPos=vec3(
		0.4f+0.8f*floor(pos.x/0.8f),
		1.f+2*floor(pos.y/2),
		1.f+2*floor(pos.z/2)
	);
	pos=localPos-pos;
	pos = abs(pos);
	return (pos.x+pos.y+pos.z-0.2f)*0.57735027;*/
}
vec3 closestPoint(vec3 pos){
	return vec3(
		0.4f+0.8f*floor(pos.x/0.8f),
		1.f+2*floor(pos.y/2),
		1.f+2*floor(pos.z/2)
	);
}

float minDist=0.1f;
int maxSteps=200;
float maxWholeDist=300;
vec3 fogColor=vec3(1.f,0.6f,0.15f);

vec3 timedCamPos=vec3(0,0,u_Time);


vec3 color(vec3 pos){

	vec3 colorClose=vec3(0.12f,0.45f*cos(pos.x+pos.y+pos.z),0.61f);
	vec3 colorFar=fogColor;

	float t=1-length(timedCamPos-pos)/maxWholeDist;
	if (t<0) t=0;
	t=t*t*t*t;

	return colorClose*t+colorFar*(1-t);
}

void main(){


	PixelColor=texture(u_Texture,t_TextureCords);



	vec3 skyboxColor=vec3(0,0,0);

	vec3 pixelVectorInWorld=normalize(u_CameraRotation*vec3(u_CameraResolutionSize*(t_TextureCords*2-1),1));

	if (pixelVectorInWorld.z<=0){
		float depth=u_EffectMult;
		
		float size=0.5f;
		float triangleMultiplier=depth/pixelVectorInWorld.y;
		vec2 localCords=pixelVectorInWorld.xz*triangleMultiplier-u_CameraPosition.xz;

		ivec2 funcCords=ivec2(floor(vec2(
			exp(acos(-localCords.x*localCords.y)),
			log(sin(-localCords.y/localCords.x))
		)));
		ivec2 boardCords=ivec2(floor(localCords/size));

		if (mod(boardCords.x+boardCords.y,2)==0) skyboxColor=vec3(1,1,1);
		else skyboxColor=vec3(0,0,0);
	}
	else {
		vec3 vector=normalize(u_CameraRotation*vec3(u_CameraResolutionSize*(t_TextureCords*2-1),1));
		vec3 curPos=timedCamPos;
		float curDist=dist(curPos);
		int steps=0;
		float wholeDist=curDist;
		while (maxSteps>steps && curDist>minDist && wholeDist<maxWholeDist){
			float angle=wholeDist/maxWholeDist*4*u_EffectMult*3.14f;

			vec3 nextPos=timedCamPos+mat3(cos(angle),sin(angle),0,-sin(angle),cos(angle),0,0,0,1)*(wholeDist*vector);

			curPos+=normalize(nextPos-curPos)*curDist;
			
			curDist=dist(curPos);
			wholeDist+=curDist;
			steps++;
		}
		if (curDist<=minDist) skyboxColor=color(curPos);
		else skyboxColor=fogColor;
	}
	
	
	PixelColor=vec4(skyboxColor*(1-PixelColor.w)+PixelColor.xyz*PixelColor.w,1);
}