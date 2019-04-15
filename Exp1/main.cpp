#include <GL/glut.h>
#include<math.h>
GLfloat angle = 0;
#define M_PI 3.14159265358979323846

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0);
	//glBegin(GL_TRIANGLES);
	//glVertex3f(0.5, 0, 0);
	//glVertex3f(0, 0.5, 0);
	//glVertex3f(-0.5, 0, 0);
	//glEnd();
	//glColor3f(1.0, 0.0, 1.0);
	//glutWireCube(0.8);
	//glMatrixMode(GL_MODELVIEW);

	//glLoadIdentity();
	//glRotatef(angle, -1, 1, 0);
	GLfloat l = 0.8f;
	glBegin(GL_LINE_LOOP);
	int n = 1000;
	for (int i = 0; i < n; i++) {
		glVertex3f(l*sin(2*M_PI/n*i), l*cos(2 * M_PI / n * i), 0);
	}
	glEnd();
	glFlush();  

}

void processKeys(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		angle += 10;
	}
}

int main(int argc, char** argv)

{
	glutInit(&argc, argv);//初始化GLUT
	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);//配置GLUT
	glutInitWindowPosition(100, 100);//窗口位置
	glutInitWindowSize(320, 320);//窗口大小
	glutCreateWindow("Test");//窗口标题
	glutDisplayFunc(renderScene);
	glClearColor(1, 1, 1, 0);
	glutSpecialFunc(processKeys);
	glutMainLoop();
	return 0;
}
