#pragma once
#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

//�Զ�����ɫ����
class Shader {
public:
	//���캯��
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
	//ʹ����ɫ��
	void use();
	//uniform���ߺ���
	void setBool(const std::string &name, bool value)const;
	void setInt(const std::string &name, int value)const;
	void setFloat(const std::string &name, float value)const;

	unsigned int getID() { return ID; }
private:
	//����ID
	unsigned int ID;
};