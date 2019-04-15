#include"main.h"
/*
 * 世界坐标系定义方式：左下角（0,0）,右上角（100,100）
 * 窗口坐标系定义方式：左上角（0,0）,右下角（1000,1000）
 * 设备标准坐标系定义方式：左下角（-1,-1）,右上角（1,1）
 * */
int main(int argc,char *argv[]) {
	LTWindow window=LTWindow("Draw",SCR_WIDTH,SCR_HEIGHT);
	window.setFramebufferSizeFun(framebuffer_size_callback);
	//绑定输入处理函数
	window.setMouseButtonFun(mouse_button_callback);
	window.setMouseMoveFun(mouse_move_callback);
	window.setMouseScrollFun(mouse_scroll_callback);
	//window.setKeyboardFun(key_callback);
	glfwSetInputMode(window.getGLFWwindow(), GLFW_STICKY_KEYS, 1);
	crossCursor=glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	handCursor=glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	draw.init();
    if(argc>1 and argc<=6){
        if(!strcmp(argv[1],"BH")){
            draw.drawLine(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));
        }
    }
	Shader shader("shaders/shader.vs", "shaders/shader.fs");
    shader.use();
    shader.setInt("WORLD_WIDTH",WORLD_WIDTH);
    shader.setInt("WORLD_HEIGHT",WORLD_HEIGHT);
    shader.setMat4("projection", projection);
	//渲染循环
	while (!window.shouldClose()) {
		//每一帧渲染时间
		/*float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;*/
		//渲染指令
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//使用着色器程序
        key_callback(window.getGLFWwindow());

		//观察矩阵
		// glm::mat4 view = camera.GetViewMatrix();
		// shader.setMat4("view", view);
		// //投影矩阵
		// glm::mat4 projection(1.0f);
		// projection = glm::perspective(glm::radians(camera.getZoom()), SCR_WIDTH / SCR_HEIGHT * 1.0f, 0.1f, 100.0f);
		// shader.setMat4("projection", projection);
		shader.setMat4("model", modelMatrix);
        draw.loop();
		//检查并调用事件，交换缓存
		window.swapBuffers();
		glfwPollEvents();
	}

	//glfwTerminate();
	return 0;
}

