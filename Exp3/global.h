#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<my_util/shader.h>
#include<vector>
#include <map>
#include"mWindow.h"
#include"mPoint.h"
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/string_cast.hpp>
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
const float SCALE=0.01f;//缩放比
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
unsigned int VBOs[2], VAOs[2];
// 同步移动速度
float deltaTime = 0.0f;	// 当前帧与上一帧时间差
float lastFrame = 0.0f;
Camera camera(glm::vec3(0.0f,0.0f,3.0f));
std::vector<mPoint> keyPoints;
std::vector<mPoint> otherPoints;
std::vector<std::pair<int,int>> circleTable;//该表用来记录起始节点与终止节点
//鼠标指针形状
GLFWcursor *crossCursor;
GLFWcursor *handCursor;
//用于标示一个多边形的初始坐标点
int beginX=0;
int beginY=0;
bool close=true;
int beginIndex=0;
//用于标示是否在拖动点
bool pressed=false;
int selected=0;
//绘制函数
void IntegerBresenhamline(int x0,int y0,int x1,int y1);
void drawPixel(int x,int y,int direct);
void drawKeyPixel(int x,int y);
void updateBuffer();
void reDraw(int index,int x,int y);
glm::ivec2 posToScreen(double x,double y);
//回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

//变换矩阵
glm::mat4 modelMatrix=glm::scale(glm::mat4(1.0f),glm::vec3(SCALE,SCALE,1.0f));
glm::mat4 projection=glm::ortho(0.0f,float(SCR_WIDTH),float(SCR_HEIGHT),0.0f,-1.0f,1.0f);

void IntegerBresenhamline(int x0,int y0,int x1,int y1){
	addNet(x0,y0,x1,y1);//增加新边表
    int direct=0;
	if(x0>x1){
		//保证x0总小于x1
		std::swap(x0,x1);
		std::swap(y0,y1);
	}
	int x,y,dx,dy;
	dx=x1-x0,dy=y1-y0;
	x=x0,y=y0;
	if(dx==0){
		//斜率为无穷情况
		if(y0>y1)
			std::swap(y0,y1);
		for(;y0<=y1;y0++)
			drawPixel(x,y0,direct);
		return;
	}
	float k=float(dy)/dx;
	if(k>1){
		std::swap(x,y);
		std::swap(dx,dy);
		direct=2;
	}else if(k<-1){
		dy*=-1;
		y*=-1;
		std::swap(x,y);
		std::swap(dx,dy);
		direct=-2;
	}else if(k<0 && k>=-1){
		dy*=-1;
		y*=-1;
		direct=-1;
	}
	int e=-dx;
	for(int i=0;i<=dx;i++){
		drawPixel(x,y,direct);
		x++,e=e+2*dy;
		if(e>=0){
			y++;
			e=e-2*dx;
		}
	}
	
}

void drawPixel(int x,int y,int direct) {
	switch(direct){
		case 2:
			otherPoints.push_back(mPoint(y,x,1));
			break;
		case -1:
			otherPoints.push_back(mPoint(x,-y,1));
			break;
		case -2:
			otherPoints.push_back(mPoint(y,-x,1));
			break;
		default:
			otherPoints.push_back(mPoint(x,y,1));
	}	
}

void drawKeyPixel(int x,int y){
	keyPoints.push_back(mPoint(x,y,0));
}

//回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{//键盘事件
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window,true);
	else{
		if(action==GLFW_REPEAT){
			switch(key){
				case GLFW_KEY_W:
					modelMatrix=glm::translate(modelMatrix,glm::vec3(0.0f,1.0f,0.0f));
					break;
				case GLFW_KEY_S:
					modelMatrix=glm::translate(modelMatrix,glm::vec3(0.0f,-1.0f,0.0f));	
					break;
				case GLFW_KEY_A:
					modelMatrix=glm::translate(modelMatrix,glm::vec3(-1.0f,0.0f,0.0f));	
					break;
				case GLFW_KEY_D:
					modelMatrix=glm::translate(modelMatrix,glm::vec3(1.0f,0.0f,0.0f));
					break;
				case GLFW_KEY_Q:
					modelMatrix=glm::rotate(modelMatrix,0.1f,glm::vec3(0.0f,0.0f,1.0f));
					break;
				case GLFW_KEY_E:
					modelMatrix=glm::rotate(modelMatrix,-0.1f,glm::vec3(0.0f,0.0f,1.0f));
					break;
				case GLFW_KEY_R:
					modelMatrix=glm::scale(modelMatrix,glm::vec3(1.1f,1.1f,1.0f));
					break;
				case GLFW_KEY_F:
					modelMatrix=glm::scale(modelMatrix,glm::vec3(0.9f,0.9f,1.0f));
					break;
			}
		}
	}
}		

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{//鼠标移动事件
	//std::cout<<xpos<<","<<ypos<<std::endl;
	glm::ivec2 pos=posToScreen(xpos,ypos);
	if(pressed && close){
		reDraw(selected,pos[0],pos[1]);
	}else{
		for(auto i=keyPoints.begin();i!=keyPoints.end();i++){
			if(pos[0]==(*i).x && pos[1]==(*i).y){
				glfwSetCursor(window,handCursor);
				return;
			}
		}
		glfwSetCursor(window,crossCursor);
	}
	
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{//滚轮事件
	camera.ProcessMouseScroll((float)yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{//鼠标点击事件
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		double x,y;
        glfwGetCursorPos(window,&x,&y);
		glm::ivec2 pos=posToScreen(x,y);
        int x1=pos[0];
        int y1=pos[1];
		for(int i=0;i<keyPoints.size();i++){
			if(keyPoints[i].x==x1 && keyPoints[i].y==y1){
				if(x1==beginX && y1==beginY && !close)
					break;
				selected=i;
				pressed=true;
				return;
			}
		}
        if (close){
            drawKeyPixel(x1,y1);
			beginX=x1;
			beginY=y1;
			close=false;
			beginIndex=keyPoints.size()-1;
        }else{
			int x0=keyPoints.back().x;
			int y0=keyPoints.back().y;
        	IntegerBresenhamline(x0,y0,x1,y1);
			if(x1==beginX && y1==beginY){
				close=true;
				circleTable.push_back(std::pair<int,int>(beginIndex,keyPoints.size()-1));
			}else{
				drawKeyPixel(x1,y1);
			}
				
		}
        updateBuffer();
        std::cout<<"绘制"<<x1<<","<<y1<<std::endl;
    }else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		pressed=false;
	}
        
}

void updateBuffer(){
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, keyPoints.size()*sizeof(mPoint), &keyPoints[0]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, otherPoints.size()*sizeof(mPoint), &otherPoints[0]);
}

void reDraw(int index,int x,int y){
	otherPoints.clear();
	keyPoints[index].x=x;
	keyPoints[index].y=y;
	auto it=circleTable.begin();
	int bI=(*it).first;
	int eI=(*it).second;
	for(int i=0;i<keyPoints.size();i++){
		if(i==eI){
			IntegerBresenhamline(keyPoints[bI].x,keyPoints[bI].y,keyPoints[eI].x,keyPoints[eI].y);
			it++;
			bI=(*it).first;
			eI=(*it).second;
		}else{
			IntegerBresenhamline(keyPoints[i].x,keyPoints[i].y,keyPoints[i+1].x,keyPoints[i+1].y);
		}
	}
	updateBuffer();
}

glm::ivec2 posToScreen(double x,double y){
	glm::vec4 pos(float(x),float(y),0.0f,1.0f);
	glm::vec4 result=projection*pos;
	result=glm::inverse(modelMatrix)*result;
	return glm::ivec2(int(result[0]),int(result[1]));
}

void polyfill(){
    for(){

    }
}