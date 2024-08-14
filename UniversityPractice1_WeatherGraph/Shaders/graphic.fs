#version 330 core

struct GraphDataStruct{
    float t;
    float v;
};
uniform GraphDataStruct GraphData[200];
uniform int GraphDataLength;
uniform vec3 u_LineColor;
uniform float u_PixelSizeX;//number that represents a single pixel scaled to whole screen

in vec2 t_GraphCords;

float getLevel(float cordX){

    if (cordX<0) return GraphData[0].v;
    else if (cordX>1) return GraphData[GraphDataLength-1].v;

    for (int i=0;i<(GraphDataLength-1);i++){
        if (GraphData[i].t<=cordX && GraphData[i+1].t>=cordX){
            float lineT=(cordX-GraphData[i].t)/(GraphData[i+1].t-GraphData[i].t);
            float curY=GraphData[i].v + (GraphData[i+1].v-GraphData[i].v)*lineT;
            return curY;
        }
    }

    return 0;
}

void main()
{
    float leftY=getLevel(t_GraphCords.x-u_PixelSizeX);
    float curY=getLevel(t_GraphCords.x);
    float rightY=getLevel(t_GraphCords.x+u_PixelSizeX);

    float minY=min(leftY,min(curY,rightY));
    float maxY=max(leftY,max(curY,rightY));
    if (minY==maxY){
        minY-=u_PixelSizeX;
        maxY+=u_PixelSizeX;
    }

    if (minY<=t_GraphCords.y && t_GraphCords.y<=maxY){
        gl_FragColor=vec4(u_LineColor,1);
        return;
    }
    
    gl_FragColor=vec4(0,0,0,0);
}