/*
lab01:
f(x) = sinx/x
x is in [-20,20]
Using the following transform:
sx = Ax+B
sy = Cy+D
*/

#include <windows.h>
#include<math.h>
#include<gl/GL.h>
#include<gl/glut.h>
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

// window size:
int screenWidth = 640;
int screenHeight = 480;
GLdouble x_begin = -20.0;
GLdouble x_end = 20.0;
GLdouble y_top;
GLdouble y_bottom;
// transform parameters:
GLdouble A, B, C, D; 


//��ʼ��
void init()
{
	glClearColor(1, 1, 1, 1);	//���ô��ڱ���ɫΪ��ɫ
	glColor3f(0.0f, 0.0f, 1.0f);	//���û��Ƶ���ɫΪ��ɫ
	glPointSize(5.0);	//��Ĵ�С
	glLineWidth(2.0);	//�ߵĿ��
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, (GLdouble)screenWidth, 0, (GLdouble)screenHeight);	//ͶӰ����
}

// ��������
void parameters_compute() {
	//������ÿ0.005����һ���㣺
	vector<GLdouble>x_value;
	vector<GLdouble>y_value;
	GLdouble step = 0.005; //�����Ĳ���
	for (GLdouble x = x_begin; x <= x_end; x+=step) {
		x_value.push_back(x);
		y_value.push_back(sin(x) / x);
	}
	//�ҵ����������ֵ����Сֵ
	auto maxPosition = max_element(y_value.begin(), y_value.end());
	auto minPosition = min_element(y_value.begin(), y_value.end());
	y_bottom = *minPosition;
	y_top = *maxPosition;
	//����A B C D�ļ��㣺
	A = (GLdouble)screenWidth / (x_end - x_begin);
	B = -1 * (GLdouble)screenWidth * x_begin / (x_end - x_begin);
	C = (GLdouble)screenHeight / (y_top - y_bottom);
	D = -1 * y_bottom * (GLdouble)screenHeight / (y_top - y_bottom);
	
	cout << "A: " << A <<" B: "<< B << " C: " << C << " D: " << D << endl;
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);//����
	//glBegin(GL_POINTS);//����
	glBegin(GL_LINE_STRIP);//���߶�ÿ�Щ

	GLdouble step = 0.005; //�����Ĳ���
	for (GLdouble x = x_begin; x <= x_end; x += step) {
		GLdouble func = sin(x) / x;
		glVertex2d(A * x + B, C * func + D);
	}
	glEnd();

	//��������
	glColor3f(0.0f,1.0f, 0.0f);	//���û��Ƶ���ɫΪ��ɫ
	glLineWidth(1.0);	//�ߵĿ��
	glBegin(GL_LINE_STRIP);//���߶�ÿ�Щ
	// y
	glVertex2d(A*(x_begin + (x_end - x_begin) / 2) + B, C * y_bottom + D);
	glVertex2d(A * (x_begin + (x_end - x_begin) / 2) + B, C * y_top + D);
	glEnd();

	glBegin(GL_LINE_STRIP);
	//x
	glVertex2d(A * x_begin + B, C * 0 + D);
	glVertex2d(A * x_end + B, C * 0 + D);


	glEnd();
	glFlush();
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	//��ʼ�����ڵ���ʾģʽ,������
	glutInitWindowPosition(100, 0);		//���ô��ڵ�λ��
	glutInitWindowSize(screenWidth, screenHeight);	//���ô��ڵ�λ��
	glutCreateWindow("lab01");	//��������
	init();							//���ڳ�ʼ��
	parameters_compute();			//��������
	glutDisplayFunc(&myDisplay);	//��ǰ���ڵĻص�����
	glutMainLoop();		//������Ϣѭ������ʾ����
	return 0;
}
