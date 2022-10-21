/*
lab04:
Animation with tweening
*/

#include <windows.h>
#include<math.h>
#include<gl/GL.h>
#include<gl/glut.h>
#include<iostream>
#include<vector>
#include<algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// window size:
int screenWidth = 640;
int screenHeight = 480;

//点的对象
struct GLintPoint {
	GLint x;
	GLint y;
};

//操作符重载
bool operator<(const GLintPoint& a, const GLintPoint& b)
{
	if (a.x == b.x)
		return a.y < b.y;
	return a.x < b.x;
}

//两个多边形的点
vector<GLintPoint*>poly_a;
vector<GLintPoint*>poly_b;
vector<GLintPoint*>poly_p;

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

//对两个多边形的点进行初始化
void Poly_init_old() {
	unsigned seed;
	seed = time(0);
	srand(seed);
	//多边形的点数：4到10
	int n = (rand() % (10 - 4 + 1)) + 4;
	//A图形
	//位置：x:0-128;y:0-300
	for (int i = 0; i < n; i++) {
		GLintPoint* p = new GLintPoint();
		p->x = (rand() % (128 - 1 + 1)) + 1;
		p->y = (rand() % (300 - 1 + 1)) + 1;
		poly_a.push_back(p);
	}
	//B图形：
	//位置：x-512-640;y:0-300
	for (int i = 0; i < n; i++) {
		GLintPoint* p = new GLintPoint();
		p->x = (rand() % (639 - 513 + 1)) + 513;
		p->y = (rand() % (300 - 1 + 1)) + 1;
		poly_b.push_back(p);
	}
	//P初始化
	for (int i = 0; i < n; i++) {
		GLintPoint* p = new GLintPoint();
		p->x = 0;
		p->y = 0;
		poly_p.push_back(p);
	}
	//对每个点的位置进行排序
	sort(poly_a.begin(),poly_a.end());
	sort(poly_b.begin(), poly_b.end());

}

void Poly_init() {
	//A图形
	//位置：x:0-128;y:0-300
	GLintPoint* p1 = new GLintPoint();
	p1->x = 64;
	p1->y = 300;
	poly_a.push_back(p1);
	GLintPoint* p2 = new GLintPoint();
	p2->x = 100;
	p2->y = 225;
	poly_a.push_back(p2);
	GLintPoint* p3 = new GLintPoint();
	p3->x = 80;
	p3->y = 225;
	poly_a.push_back(p3);
	GLintPoint* p4 = new GLintPoint();
	p4->x = 128;
	p4->y = 150;
	poly_a.push_back(p4);
	GLintPoint* p5 = new GLintPoint();
	p5->x = 80;
	p5->y = 150;
	poly_a.push_back(p5);
	GLintPoint* p6 = new GLintPoint();
	p6->x = 80;
	p6->y = 0;
	poly_a.push_back(p6);
	GLintPoint* p7 = new GLintPoint();
	p7->x = 48;
	p7->y = 0;
	poly_a.push_back(p7);
	GLintPoint* p8 = new GLintPoint();
	p8->x = 48;
	p8->y = 150;
	poly_a.push_back(p8);
	GLintPoint* p9 = new GLintPoint();
	p9->x = 0;
	p9->y = 150;
	poly_a.push_back(p9);
	GLintPoint* p10 = new GLintPoint();
	p10->x = 48;
	p10->y = 225;
	poly_a.push_back(p10);
	GLintPoint* p11 = new GLintPoint();
	p11->x = 28;
	p11->y = 225;
	poly_a.push_back(p11);



	//B图形：
	//位置：x-512-640;y:0-300
	GLintPoint* p1b = new GLintPoint();
	p1b->x = 576;
	p1b->y = 300;
	poly_b.push_back(p1b);
	GLintPoint* p2b = new GLintPoint();
	p2b->x = 640;
	p2b->y = 300;
	poly_b.push_back(p2b);
	GLintPoint* p3b = new GLintPoint();
	p3b->x = 640;
	p3b->y = 240;
	poly_b.push_back(p3b);
	GLintPoint* p4b = new GLintPoint();
	p4b->x = 576;
	p4b->y = 240;
	poly_b.push_back(p4b);
	GLintPoint* p5b = new GLintPoint();
	p5b->x = 576;
	p5b->y = 180;
	poly_b.push_back(p5b);
	GLintPoint* p6b = new GLintPoint();
	p6b->x = 608;
	p6b->y = 180;
	poly_b.push_back(p6b);
	GLintPoint* p7b = new GLintPoint();
	p7b->x = 608;
	p7b->y = 120;
	poly_b.push_back(p7b);
	GLintPoint* p8b = new GLintPoint();
	p8b->x = 576;
	p8b->y = 120;
	poly_b.push_back(p8b);
	GLintPoint* p9b = new GLintPoint();
	p9b->x = 576;
	p9b->y = 0;
	poly_b.push_back(p9b);
	GLintPoint* p10b = new GLintPoint();
	p10b->x = 512;
	p10b->y = 0;
	poly_b.push_back(p10b);
	GLintPoint* p11b = new GLintPoint();
	p11b->x = 512;
	p11b->y = 300;
	poly_b.push_back(p11b);

	//P初始化
	for (int i = 0; i < 11; i++) {
		GLintPoint* p = new GLintPoint();
		p->x = 0;
		p->y = 0;
		poly_p.push_back(p);
	}

}

//画矩形
void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);//清屏

	//画A
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < poly_a.size(); i++) {
		glVertex2i(poly_a[i]->x, poly_a[i]->y);
	}
	glEnd();
	glFlush();

	//画B
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < poly_b.size(); i++) {
		glVertex2i(poly_b[i]->x, poly_b[i]->y);
	}
	glEnd();
	glFlush();

	//画P
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < poly_p.size(); i++) {
		glVertex2i(poly_p[i]->x, poly_p[i]->y);
	}
	glEnd();
	glFlush();
}


//画内插
void drawTween(float t) {
	//计算内插
	for (int i = 0; i < poly_a.size(); i++) {
		poly_p[i]->x = poly_a[i]->x + (poly_b[i]->x - poly_a[i]->x) * t;
		poly_p[i]->y = poly_a[i]->y + (poly_b[i]->y - poly_a[i]->y) * t;
	}
	//画图
	myDisplay();
	//glutPostRedisplay();
}

//插值动画
void process() {
	float delt = 0.1; //比例
	for (float t = 0;; t += delt) {
		//glClear(GL_COLOR_BUFFER_BIT);//清屏
		Sleep(100);
		if (t <= 1.1 && t >= 0) {
			drawTween(t);
		}
		//glutSwapBuffers();
		//反方向变化
		if (t > 1.1 || t < 0.0) {
			delt = -1 * delt;
		}
	}
}

//定义键盘操作
void keyboard_fun(unsigned char key, int mouse_x, int mouse_y) {
	switch (key)
	{
	case 'r':
		process();
		break;
	case 'q':
		exit(0);
	default:
		break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	//初始化窗口的显示模式,单缓冲
	glutInitWindowPosition(100, 0);		//设置窗口的位置
	glutInitWindowSize(screenWidth, screenHeight);	//设置窗口的位置
	glutCreateWindow("lab04");	//创建窗口
	init();							//窗口初始化
	Poly_init();
	glutDisplayFunc(&myDisplay);	//当前窗口的回调函数
	glutKeyboardFunc(&keyboard_fun);
	glutMainLoop();		//启动消息循环，显示窗口
	return 0;
}