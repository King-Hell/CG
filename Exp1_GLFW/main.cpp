#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include<stb/stb_image.h>

#include<my_util/camera.h>
#include<my_util/shader.h>

//设置
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

Camera camera(glm::vec3(0.0f,0.0f,3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// 同步移动速度
float deltaTime = 0.0f;	// 当前帧与上一帧时间差
float lastFrame = 0.0f;


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);//副版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//创建窗口
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test", NULL, NULL);
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
		// 位置              // 颜色				//纹理
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f,	0.0f,0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,	-1.0f,0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,	-1.0f,-1.0f,
		-0.5f, 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,	0.0f,-1.0f,
		-0.5f, -0.5f,-0.5f,  1.0f, 0.0f, 0.0f,	-1.0f,0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,	0.0f,0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,	0.0f,-1.0f,
		-0.5f, 0.5f, -0.5f,  1.0f, 1.0f, 1.0f,	-1.0f,-1.0f
	};
	//unsigned int indices[] = {
	//	0,2,1,0,2,3,
	//	1,6,5,1,6,2,
	//	3,6,2,3,6,7,
	//	0,7,3,0,7,4,
	//	1,4,0,1,4,5,
	//	4,6,5,4,6,7
	//};
	unsigned int indices0[] =   {0,3,2,1,5,4,7,3};
	unsigned int indices1[] = {6,7,4,5,1,2,3,7};
	unsigned int EBOs[2];
	glGenBuffers(2, EBOs);

	unsigned int VBO, VAO;
	//1.绑定VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//2.把顶点数组复制到缓冲中
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//3.把索引数组复制到缓冲中
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices0), indices0, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
	//4.设置顶点位置指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//5.设置顶点颜色指针
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//载入图片
	int width, height, nrChannels;
	unsigned char *data = stbi_load("zwj.jpg", &width, &height, &nrChannels, 0);
	//生成纹理
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	//设置纹理指针
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//取消绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	//线框模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	
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
		//变换矩阵
		glm::mat4 trans(1.0f);//GLM0.9.9以上需要将矩阵初始化为单位矩阵
		float timeValue =(float) glfwGetTime();
		trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		trans = glm::rotate(trans, timeValue, glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setMat4("model", trans);
		//观察矩阵
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);
		//投影矩阵
		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(camera.getZoom()), SCR_WIDTH / SCR_HEIGHT * 1.0f, 0.1f, 100.0f);
		shader.setMat4("projection", projection);
		//绑定纹理
		glBindTexture(GL_TEXTURE_2D, texture);
		//绘制三角形
		glBindVertexArray(VAO);
		//glDrawArrays(GL_LINE_LOOP, 0, 3);
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
		glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
		glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_INT, 0);  

		//glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
		
		//检查并调用事件，交换缓存
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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