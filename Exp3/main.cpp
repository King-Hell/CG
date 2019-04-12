#include"global.h"

int main(int argc,char *argv[]) {
	keyPoints.reserve(10000);
	otherPoints.reserve(10000);
	if(argc>1 and argc<=6){
		if(!strcmp(argv[1],"BH")){
			IntegerBresenhamline(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));
		}
	}
	mWindow window=mWindow("Draw",SCR_WIDTH,SCR_HEIGHT);
	window.setFramebufferSizeFun(framebuffer_size_callback);
	//绑定输入处理函数
	window.setMouseButtonFun(mouse_button_callback);
	window.setMouseMoveFun(mouse_move_callback);
	window.setMouseScrollFun(mouse_scroll_callback);
	window.setKeyboardFun(key_callback);
	//设置鼠标指针为手形
	//window.setCursor(GLFW_CROSSHAIR_CURSOR);
	crossCursor=glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	handCursor=glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	Shader shader("shader.vs", "shader.fs");
	//VBO VAO
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, keyPoints.capacity()*sizeof(mPoint), &keyPoints[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(mPoint), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(1, 1, GL_INT,sizeof(mPoint), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, otherPoints.capacity()*sizeof(mPoint), &otherPoints[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(mPoint), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(1, 1, GL_INT,sizeof(mPoint), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);
	//取消绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	//线框模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//允许修改点大小
	glEnable(GL_PROGRAM_POINT_SIZE);

	//渲染循环
	while (!window.shouldClose()) {
		//每一帧渲染时间
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//使用着色器程序
		shader.use();

		//观察矩阵
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);
		// //投影矩阵
		// glm::mat4 projection(1.0f);
		// projection = glm::perspective(glm::radians(camera.getZoom()), SCR_WIDTH / SCR_HEIGHT * 1.0f, 0.1f, 100.0f);
		// shader.setMat4("projection", projection);
		glm::mat4 trans(1.0f);
		trans = glm::scale(trans, glm::vec3(SCALE, SCALE, 1.0f));
		shader.setMat4("model", trans);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_POINTS, 0, GLsizei(keyPoints.size()));
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_POINTS, 0, GLsizei(otherPoints.size()));
		//检查并调用事件，交换缓存
		window.swapBuffers();
		glfwPollEvents();
	}
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	//glfwTerminate();
	return 0;
}

