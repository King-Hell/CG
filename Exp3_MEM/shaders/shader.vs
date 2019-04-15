//顶点着色器
#version 450 core
layout (location=0) in int aType;
out int type;
const int POINT=1;
const int LINE=2;
const int PADDING=3;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int WORLD_WIDTH;
uniform int WORLD_HEIGHT;

void main(){
	//gl_Position = projection*view*model*vec4(aPos,0.0f, 1.0f);
	gl_Position = projection*model*vec4(gl_VertexID%WORLD_WIDTH,gl_VertexID/WORLD_HEIGHT,0.0f, 1.0f);
	type=aType;
	gl_PointSize=8.0f;
/*	switch(type){
		case POINT:
			gl_PointSize=8.0f;
			break;
	}*/
}