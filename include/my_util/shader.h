#pragma once
#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

//自定义着色器类
class Shader {
public:
	//构造函数
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
	//使用着色器
	void use();
	//uniform工具函数
	void setBool(const std::string &name, bool value)const;
	void setInt(const std::string &name, int value)const;
	void setFloat(const std::string &name, float value)const;

	unsigned int getID() { return ID; }
private:
	//程序ID
	unsigned int ID;
};