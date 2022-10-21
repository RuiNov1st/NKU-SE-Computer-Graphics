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


//初始化
void init()
{
	glClearColor(1, 1, 1, 1);	//设置窗口背景色为白色
	glColor3f(0.0f, 0.0f, 1.0f);	//设置绘制的颜色为蓝色
	glPointSize(5.0);	//点的大小
	glLineWidth(2.0);	//线的宽度
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, (GLdouble)screenWidth, 0, (GLdouble)screenHeight);	//投影参数
}

// 参数计算
void parameters_compute() {
	//采样：每0.005采样一个点：
	vector<GLdouble>x_value;
	vector<GLdouble>y_value;
	GLdouble step = 0.005; //采样的步长
	for (GLdouble x = x_begin; x <= x_end; x+=step) {
		x_value.push_back(x);
		y_value.push_back(sin(x) / x);
	}
	//找到函数的最大值和最小值
	auto maxPosition = max_element(y_value.begin(), y_value.end());
	auto minPosition = min_element(y_value.begin(), y_value.end());
	y_bottom = *minPosition;
	y_top = *maxPosition;
	//进行A B C D的计算：
	A = (GLdouble)screenWidth / (x_end - x_begin);
	B = -1 * (GLdouble)screenWidth * x_begin / (x_end - x_begin);
	C = (GLdouble)screenHeight / (y_top - y_bottom);
	D = -1 * y_bottom * (GLdouble)screenHeight / (y_top - y_bottom);
	
	cout << "A: " << A <<" B: "<< B << " C: " << C << " D: " << D << endl;
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);//清屏
	//glBegin(GL_POINTS);//画点
	glBegin(GL_LINE_STRIP);//画线多好看些

	GLdouble step = 0.005; //采样的步长
	for (GLdouble x = x_begin; x <= x_end; x += step) {
		GLdouble func = sin(x) / x;
		glVertex2d(A * x + B, C * func + D);
	}
	glEnd();

	//画坐标轴
	glColor3f(0.0f,1.0f, 0.0f);	//设置绘制的颜色为蓝色
	glLineWidth(1.0);	//线的宽度
	glBegin(GL_LINE_STRIP);//画线多好看些
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
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	//初始化窗口的显示模式,单缓冲
	glutInitWindowPosition(100, 0);		//设置窗口的位置
	glutInitWindowSize(screenWidth, screenHeight);	//设置窗口的位置
	glutCreateWindow("lab01");	//创建窗口
	init();							//窗口初始化
	parameters_compute();			//参数计算
	glutDisplayFunc(&myDisplay);	//当前窗口的回调函数
	glutMainLoop();		//启动消息循环，显示窗口
	return 0;
}
