#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include<stb/stb_image.h>

#include"shader.h"
//����
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);//���汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//��������
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
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	Shader shader("shader.vs", "shader.fs");
	//VBO VAO
	float vertices[] = {
		// λ��              // ��ɫ				//����
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f,	0.0f,0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,	1.0f,0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,	1.0f,1.0f,
		-0.5f, 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,	0.0f,1.0f,
		-0.5f, -0.5f,-0.5f,  1.0f, 0.0f, 0.0f,	1.0f,0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,	0.0f,0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,	0.0f,1.0f,
		-0.5f, 0.5f, -0.5f,  1.0f, 1.0f, 1.0f,	1.0f,1.0f
	};
	//unsigned int indices[] = {
	//	0,2,1,0,2,3,
	//	1,6,5,1,6,2,
	//	3,6,2,3,6,7,
	//	0,7,3,0,7,4,
	//	1,4,0,1,4,5,
	//	4,6,5,4,6,7
	//};
	unsigned int indices0[] = {0,3,2,1,5,4,7,3};
	unsigned int indices1[] = {6,7,4,5,1,2,3,7};
	unsigned int EBOs[2];
	glGenBuffers(2, EBOs);

	unsigned int VBO, VAO;
	//1.��VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//2.�Ѷ������鸴�Ƶ�������
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//3.���������鸴�Ƶ�������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices0), indices0, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
	//4.���ö���λ��ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//5.���ö�����ɫָ��
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//����ͼƬ
	int width, height, nrChannels;
	unsigned char *data = stbi_load("zwj.jpg", &width, &height, &nrChannels, 0);
	//��������
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	//��������ָ��
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//ȡ����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	//�߿�ģʽ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	
	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		//����
		processInput(window);

		//��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//ʹ����ɫ������
		shader.use();
		//�任����
		glm::mat4 trans(1.0f);//GLM0.9.9������Ҫ�������ʼ��Ϊ��λ����
		float timeValue =(float) glfwGetTime();
		trans = glm::rotate(trans, timeValue, glm::vec3(1.0f, 1.0f, 1.0f));
		int transformLocation = glGetUniformLocation(shader.getID(), "model");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));
		//�۲����
		glm::mat4 view(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		int viewLocation = glGetUniformLocation(shader.getID(), "view");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		//ͶӰ����
		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH / SCR_HEIGHT*1.0f, 0.1f, 100.0f);
		int proLocation = glGetUniformLocation(shader.getID(), "projection");
		glUniformMatrix4fv(proLocation, 1, GL_FALSE, glm::value_ptr(projection));
		//������
		glBindTexture(GL_TEXTURE_2D, texture);
		//����������
		glBindVertexArray(VAO);
		//glDrawArrays(GL_LINE_LOOP, 0, 3);
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
		glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
		glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
		
		//��鲢�����¼�����������
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
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}