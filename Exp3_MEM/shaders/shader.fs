//片段着色器
#version 450 core
out vec4 FragColor;
flat in int type;
const int BACKGROUND=0;
const int POINT=1;
const int LINE=2;
const int PADDING=3;
const vec4 BACK=vec4(0.2f, 0.3f, 0.3f, 1.0f);
const vec4 WHITE=vec4(1.0f, 1.0f, 1.0f, 1.0f);
const vec4 YELLOW=vec4(1.0f, 1.0f, 0.5f, 1.0f);
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
			FragColor=YELLOW;
			break;
		default:
			FragColor=BACK;
	}
	
}
