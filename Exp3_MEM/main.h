#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<my_util/shader.h>
#include<vector>
#include <map>
#include"LTWindow.hpp"
#include"LTPoint.h"
#include"LTDraw.hpp"
#include "LTPolygon.hpp"

typedef std::pair<int, int> Point;

void IntegerBresenhamline(int x0, int y0, int x1, int y1) {}

void drawKeyPixel(int x, int y) {}

void updateBuffer() {}

#define GLM_ENABLE_EXPERIMENTAL

#include<glm/gtx/string_cast.hpp>

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
const unsigned int WORLD_WIDTH = 100;
const unsigned int WORLD_HEIGHT = 100;
std::vector<LTPolygon *> polygons;
//鼠标指针形状
GLFWcursor *crossCursor;
GLFWcursor *handCursor;
//用于标示是否在拖动点
bool pressed = false;
LTPolygon *selectedPolygon;//记录当前选中的多边形
int selectedPoint;//记录当前选中的顶点在多边形中的序号
//绘制函数
LTDraw draw(WORLD_WIDTH, WORLD_HEIGHT);

glm::ivec2 posToScreen(double x, double y);

//回调函数
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

//void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void key_callback(GLFWwindow *window);
void mouse_move_callback(GLFWwindow *window, double xpos, double ypos);

void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

//变换矩阵
glm::mat4 modelMatrix(1.0f);
glm::mat4 projection = glm::ortho(0.0f, float(WORLD_WIDTH), 0.0F, float(WORLD_HEIGHT), -1.0f, 1.0f);

//回调函数
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

/*void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {//键盘事件
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window,
                                 true);
    else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        draw.clear();
        polygons.clear();
        draw.updateBuffer();
    } else {
        if (action == GLFW_REPEAT) {
            switch (key) {
                case GLFW_KEY_W:
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
                    break;
                case GLFW_KEY_S:
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
                    break;
                case GLFW_KEY_A:
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.0f, 0.0f, 0.0f));
                    break;
                case GLFW_KEY_D:
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
                    break;
                case GLFW_KEY_Q:
                    modelMatrix = glm::rotate(modelMatrix, 0.05f, glm::vec3(0.0f, 0.0f, 1.0f));
                    break;
                case GLFW_KEY_E:
                    modelMatrix = glm::rotate(modelMatrix, -0.05f, glm::vec3(0.0f, 0.0f, 1.0f));
                    break;
                case GLFW_KEY_R:
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.1f, 1.1f, 1.0f));
                    break;
                case GLFW_KEY_F:
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.9f, 0.9f, 1.0f));
                    break;
            }
        }
    }
}*/

void key_callback(GLFWwindow *window) {//键盘事件
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        draw.clear();
        polygons.clear();
        draw.updateBuffer();
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.0f, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        modelMatrix = glm::rotate(modelMatrix, 0.03f, glm::vec3(0.0f, 0.0f, 1.0f));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        modelMatrix = glm::rotate(modelMatrix, -0.03f, glm::vec3(0.0f, 0.0f, 1.0f));
}

void mouse_move_callback(GLFWwindow *window, double xpos, double ypos) {//鼠标移动事件
    glm::ivec2 pos = posToScreen(xpos, ypos);
    /*std::cout<<pos[0]<<","<<pos[1]<<std::endl;
    std::cout<<draw.get(pos[0],pos[1])<<std::endl;*/
    if (pressed && selectedPolygon->isClose()) {
        draw.clear();
        selectedPolygon->set(selectedPoint, pos[0], pos[1]);
        for (auto i = polygons.begin(); i != polygons.end(); i++) {
            draw.fill(**i);
            draw.drawBorder(**i);
        }
        draw.updateBuffer();
    } else {
        if (draw.get(pos[0], pos[1]) == 1)
            glfwSetCursor(window, handCursor);
        else
            glfwSetCursor(window, crossCursor);
        //}

    }
}

void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {//滚轮事件
    if (yoffset == 1)
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.1f, 1.1f, 1.0f));
    else
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.9f, 0.9f, 0.9f));
}


void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {//鼠标点击事件
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        glm::ivec2 pos = posToScreen(x, y);
        int x1 = pos[0];
        int y1 = pos[1];
        if (draw.get(x1, y1) == 1) {
            //该位置有顶点
            Point p(x1, y1);
            if (!polygons.empty() && polygons.back()->getFirst() == p && !polygons.back()->isClose()) {
                //如果需要闭合
                Point last = polygons.back()->getLast();
                if (polygons.back()->add(p));
                draw.fill(*polygons.back());
                draw.drawBorder(*polygons.back());
                draw.updateBuffer();
            } else {
                for (auto i = polygons.begin(); i != polygons.end(); i++) {
                    LTPolygon *polygon = *i;
                    int index = 0;
                    for (auto j = polygon->begin(); j != polygon->end(); j++, index++) {
                        if (*j == p) {
                            selectedPolygon = polygon;
                            selectedPoint = index;
                            pressed = true;
                            return;
                        }
                    }
                }
            }
        } else if (polygons.empty() || polygons.back()->isClose()) {
            //该位置无顶点
            //此时绘制新的多边形
            LTPolygon *polygon = new LTPolygon();
            polygon->add(Point(x1, y1));
            polygons.push_back(polygon);
            draw.drawPixel(x1, y1, 0, 1);
            draw.updateBuffer();
        } else {
            Point last = polygons.back()->getLast();
            //std::cout << last.first << last.second << std::endl;
            if (polygons.back()->add(Point(x1, y1))) {
                draw.drawLine(last.first, last.second, x1, y1);
                draw.updateBuffer();
            }
        }
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        pressed = false;
    }

}

glm::ivec2 posToScreen(double x, double y) {
    //将鼠标坐标转换到世界坐标
    glm::vec4 result(x * WORLD_WIDTH / SCR_WIDTH, WORLD_HEIGHT - y * WORLD_HEIGHT / SCR_HEIGHT,1.0f,1.0f);
    result=glm::inverse(modelMatrix)*result;
    return glm::ivec2(result[0],result[1]);
}

