//顶点着色器
#version 450 core
layout (location=0) in vec2 aPos;
layout (location=1) in int aType;
out int type;
const int POINT=0;
const int LINE=1;
const int PADDING=2;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	//gl_Position = projection*view*model*vec4(aPos,0.0f, 1.0f);
	gl_Position = model*vec4(aPos,0.0f, 1.0f);
	type=aType;
	switch(type){
		case POINT:
			gl_PointSize=8.0f;
			break;
		default:
			gl_PointSize=4.0f;
	}
}