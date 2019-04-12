//片段着色器
#version 450 core
out vec4 FragColor;
flat in int type;
const int POINT=0;
const int LINE=1;
const int PADDING=2;
const vec4 WHITE=vec4(1.0f, 1.0f, 1.0f, 1.0f);
const vec4 BLACK=vec4(0.0f, 0.0f, 0.0f, 1.0f);
const vec4 RED=vec4(1.0f, 0.0f, 0.0f, 1.0f);

void main(){
	switch(type){
		case POINT:
			FragColor=RED;
			break;
		case LINE:
			FragColor=WHITE;
			break;
		case PADDING:
			FragColor=BLACK;
			break;
		default:
			FragColor=BLACK;
	}
	
}