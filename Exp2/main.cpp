#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<my_util/camera.h>
#include<my_util/shader.h>

#include<vector>

//设置
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
const bool DRAW_NET = true;
const bool DRAW_POINT = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

Camera camera(glm::vec3(0.0f,0.0f,3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
//自定义函数
void DDALine(int x0, int y0, int x1, int y1);
void IntegerBresenhamline(int x0,int y0,int x1,int y1);
void drawpixel(int x, int y);
void CirclePoints(int x,int y,int x0,int y0);
void MidPointCircle(int x,int y,int r);
// 同步移动速度
float deltaTime = 0.0f;	// 当前帧与上一帧时间差
float lastFrame = 0.0f;

std::vector<float> points;

bool isLine=true;
void DDALine(int x0,int y0,int x1,int y1) {
	int x;
	float dx, dy, y, k;
	dx = float(x1 - x0), dy = float(y1 - y0);
	k = dy / dx, y = float(y0);
	for (x = x0; x <= x1; x++) {
		drawpixel(x, int(y + 0.5));
		y = y + k;
	}
	isLine=true;
}

void IntegerBresenhamline(int x0,int y0,int x1,int y1){
	int x,y,dx,dy,e;
	dx=x1-x0,dy=y1-y0,e=-dx;
	x=x0,y=y0;
	for(int i=0;i<=dx;i++){
		drawpixel(x,y);
		x++,e=e+2*dy;
		if(e>=0){
			y++;
			e=e-2*dx;
		}
	}
	isLine=true;
}

void MidPointCircle(int x0,int y0,int r){
	int x=0,y=r;
	float d;
	d=1.25-r;
	CirclePoints(x,y,x0,y0);
	while(x<=y){
		if(d<0)
			d+=2*x+3;
		else{
			d+=2*(x-y)+5;
			y--;
		}
		x++;
		CirclePoints(x,y,x0,y0);
	}
	isLine=false;
}

void CirclePoints(int x,int y,int x0,int y0){
	drawpixel(x+x0,y+y0);drawpixel(y+x0,x+y0);
	drawpixel(-x+x0,y+y0);drawpixel(y+x0,-x+y0);
	drawpixel(x+x0,-y+y0);drawpixel(-y+x0,x+y0);
	drawpixel(-x+x0,y+y0);drawpixel(-y+x0,-x+y0);
}

void drawpixel(int x,int y) {
	points.push_back(float(x-6));
	points.push_back(float(y-6));
}

int main(int argc,char *argv[]) {
	if(argc>1 and argc<=6){
		if(!strcmp(argv[1],"DDA")){
			DDALine(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));
		}else if(!strcmp(argv[1],"BH")){
			IntegerBresenhamline(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));
		}else if(!strcmp(argv[1],"MC")){
			MidPointCircle(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
		}
	}
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);//副版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//创建窗口
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "DDA and Bresenham", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//绑定输入处理函数
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader shader("shader.vs", "shader.fs");
	//VBO VAO
	float vertices[] = {
		// 位置  
		-1.0f, -1.0f,  
		1.0f, -1.0f,   
		1.0f,  1.0f,   
		-1.0f, 1.0f,   
	};
	unsigned int indices[] = {0,1,2,3};
	unsigned int EBOs[2];
	glGenBuffers(2, EBOs);
	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	//1.绑定VAO
	glBindVertexArray(VAOs[0]);
	//2.把顶点数组复制到缓冲中
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//3.把索引数组复制到缓冲中
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//4.设置顶点位置指针
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(float), &points[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//取消绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	//线框模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//允许修改点大小
	glEnable(GL_PROGRAM_POINT_SIZE);
	std::vector<glm::mat4> netPosition(100);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			glm::mat4 trans(1.0f);//GLM0.9.9以上需要将矩阵初始化为单位矩阵
			trans = glm::scale(trans, glm::vec3(0.1f, 0.1f, 0.1f));
			trans = glm::translate(trans, glm::vec3(i - 5, j - 5, 0.0f));
			netPosition.push_back(trans);
		}
	}

	//渲染循环
	while (!glfwWindowShouldClose(window)) {
		//每一帧渲染时间
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;	
		//输入
		processInput(window);
		//渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//使用着色器程序
		shader.use();

		//观察矩阵
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);
		//投影矩阵
		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(camera.getZoom()), SCR_WIDTH / SCR_HEIGHT * 1.0f, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		shader.setBool("type", DRAW_POINT);
		glm::mat4 trans(1.0f);
		trans = glm::scale(trans, glm::vec3(0.1f, 0.1f, 0.1f));
		shader.setMat4("model", trans);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_POINTS, 0, GLsizei(points.size() / 2));
		if(isLine)
			glDrawArrays(GL_LINE_STRIP, 0, GLsizei(points.size() / 2));
		//绘制网格
		glBindVertexArray(VAOs[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
		//变换矩阵
		shader.setBool("type", DRAW_NET);
		for (auto i = netPosition.begin(); i != netPosition.end(); i++) {
			shader.setMat4("model",*i);
			glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
		}
		//检查并调用事件，交换缓存
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{//键盘事件
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{//鼠标事件
	if (firstMouse){
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}
	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top
	lastX = (float)xpos;
	lastY = (float)ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{//滚轮事件
	camera.ProcessMouseScroll((float)yoffset);
}