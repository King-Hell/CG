//Æ¬¶Î×ÅÉ«Æ÷
#version 450 core
out vec4 FragColor;
uniform bool type;

void main(){
	if(type)
		FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	else
		FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
}