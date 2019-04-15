#pragma once
#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include<iostream>
#include<my_util/camera.h>

class LTWindow{
    //窗口类
public:
    LTWindow(const char *title,int width,int height):width(width),height(height){
        glfwInit();
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//主版本号
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);//副版本号
	    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (window == NULL) {
		    std::cout << "Failed to create GLFW window" << std::endl;
		    glfwTerminate();
	    }
        glfwMakeContextCurrent(window);
	    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		    std::cout << "Failed to initialize GLAD" << std::endl;
	    }
        glViewport(0, 0, width, height);
        cursor=NULL;
    }

    ~LTWindow(){
        glfwDestroyWindow(window);
        if(cursor)
            glfwDestroyCursor(cursor);
    }

    GLFWwindow* getGLFWwindow(){return window;}
    bool shouldClose(){return glfwWindowShouldClose(window);}
    void setFramebufferSizeFun(GLFWframebuffersizefun fun){glfwSetFramebufferSizeCallback(window, fun);};
    void setMouseScrollFun(GLFWscrollfun fun){glfwSetScrollCallback(window,fun);}
    void setMouseButtonFun(GLFWmousebuttonfun fun){glfwSetMouseButtonCallback(window, fun);}
    void setMouseMoveFun(GLFWcursorposfun fun){glfwSetCursorPosCallback(window, fun);}
    void setKeyboardFun(GLFWkeyfun fun){glfwSetKeyCallback(window,fun);}
    void swapBuffers(){glfwSwapBuffers(window);}
    void setCursor(int shape){
        cursor=glfwCreateStandardCursor(shape);
        glfwSetCursor(window,cursor);
    }
private:
    GLFWwindow *window;
    GLFWcursor *cursor;
    int width;
    int height;
};

