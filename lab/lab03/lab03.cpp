/*
lab03:
Cohen-Sutherland Clipper
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
//rectangle size:
int rectWidth = 500;
int rectHeight = 250;

//点的对象
struct GLintPoint {
	GLint x;
	GLint y;
	int code = 0;
};

//点对
struct GLintPointPair {
	GLintPoint* p1;
	GLintPoint* p2;
	GLdouble delx;
	GLdouble  dely;
	int status;// 1 接受;-1拒绝;0未完成
};

//矩形对象
struct Rect {
	GLint l = 0;
	GLint r = 0;
	GLint t = 0;
	GLint b = 0;
};

//一些全局对象
vector<GLintPointPair*>lines;
Rect r;
int point_num = 100;//线段的数量
int color = 0;

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


//点和矩阵的随机初始化
void lines_rect() {
	glClear(GL_COLOR_BUFFER_BIT);//清屏
	unsigned seed;
	seed = time(0);
	srand(seed);
	//首先随机生成一个点作为矩阵的左下角
	GLintPoint g1;
	g1.x = (rand() % ((screenWidth - 1 - rectWidth) - 1 + 1)) + 1;
	g1.y = (rand() % ((screenHeight - 1 - rectHeight) - 1 + 1)) + 1;
	//计算矩阵
	r.l = g1.x;
	r.r = g1.x + rectWidth;
	r.b = g1.y;
	r.t = g1.y + rectHeight;
	//画矩阵
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(r.l, r.b);
	glVertex2i(r.l, r.t);
	glVertex2i(r.r, r.t);
	glVertex2i(r.r, r.b);
	glEnd();
	glFlush();

	//开始随机生成点构成直线
	for (int i = 0; i < 2 * point_num; i+=2) {
		GLintPoint *p1 = new GLintPoint();
		p1->x = (rand() % (screenWidth - 1  - 1 + 1)) + 1;
		p1->y = (rand() % (screenHeight - 1 - 1 + 1)) + 1;
		GLintPoint* p2 = new GLintPoint();
		p2->x = (rand() % (screenWidth - 1 - 1 + 1)) + 1;
		p2->y = (rand() % (screenHeight - 1 - 1 + 1)) + 1;
		GLintPointPair* p = new GLintPointPair();
		p->p1 = p1;
		p->p2 = p2;
		lines.push_back(p);
	}


}
//画线段
void myDisplay() {
	
	if (color == 0) {
		glColor3f(0.0f, 0.0f, 1.0f);
	}
	else {
		glColor3f(0.0f, 1.0f, 0.0f);
		glLineWidth(3.0);

	}
	glBegin(GL_LINES);
	//画线段
	for (int i = 0; i < lines.size(); i++) {
		if (color == 0) {
			glVertex2i(lines[i]->p1->x, lines[i]->p1->y);
			glVertex2i(lines[i]->p2->x, lines[i]->p2->y);
		}
		else {
			//只画在里面的
			if (lines[i]->status == 1) {
				glVertex2i(lines[i]->p1->x, lines[i]->p1->y);
				glVertex2i(lines[i]->p2->x, lines[i]->p2->y);
			}
		}
	}
	color++;
	glEnd();
	glFlush();
}
//对每个点进行编码的计算
void code_compute(GLintPointPair* line) {
		//计算delx dely：
	line->delx = double(line->p2->x - line->p1->x);
	line->dely = double(line->p2->y - line->p1->y);
	GLintPoint* temp;
		for (int j = 0; j < 2; j++) {
			if (j == 0) {
				temp = line->p1;
			}
			else {
				temp = line->p2;
			}
			temp->code = 0;
			//判断位置
			//左：
			if (temp->x < r.l) {
				temp->code |= 8;
			}
			//上
			if (temp->y > r.t) {
				temp->code |= 4;
			}
			//右
			if (temp->x > r.r) {
				temp->code |= 2;
			}
			if (temp->y < r.b) {
				temp->code |= 1;
			}
		}
		
	}

//具体截取函数
void ChopLine(GLintPointPair* pair, GLintPoint* p) {
	//根据点的位置进行处理
	//左
	if (p->code & 8) {
		p->y = p->y + (r.l-p->x) * (pair->dely / pair->delx);
		p->x = r.l;
	}
	else {
		//上
		if (p->code & 4) {
			p->x = p->x + (r.t-p->y) * (pair->delx / pair->dely);
			p->y = r.t;
		}
		else {
			//右
			if (p->code & 2) {
				p->y = p->y + (r.r-p->x) * (pair->dely/pair->delx);
				p->x = r.r;
			}
			else {
				//下
				if (p->code & 1) {
					p->x = p->x + (r.b-p->y) * (pair->delx / pair->dely);
					p->y = r.b;
				}
			}
		}
	}
}




//截取全流程函数
void process() {
	//对每个线段进行判断
	for (int i = 0; i < lines.size(); i++) {
		int count = 0;
		do {
			//首先计算每个点的编码
			code_compute(lines[i]);
			//先对trival accept和trival reject进行判断
			//trival accept：两个都是0
			if ((lines[i]->p1->code | lines[i]->p2->code) == 0) {
				lines[i]->status = 1;
				break;
			}
			//trival reject：存在某一位都是1
			if((lines[i]->p1->code & lines[i]->p2->code) != 0||count>=4) {
				lines[i]->status = -1;
				break;
			}
			//截取
			if (lines[i]->p1->code != 0) {
				ChopLine(lines[i], lines[i]->p1);
			}
			if (lines[i]->p2->code != 0) {
				ChopLine(lines[i], lines[i]->p2);
			}
			count++;
		} while (1);
	}
	myDisplay();
}

//定义键盘操作
void keyboard_fun(unsigned char key, int mouse_x, int mouse_y) {
	switch (key)
	{
	case 'r':
		process();
		break;
	case 'b':
		//重新随机生成重新开始
		color = 0;
		lines.clear();
		lines_rect();
		myDisplay();
		break;
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
	glutCreateWindow("lab03");	//创建窗口
	init();							//窗口初始化
	lines_rect();
	glutDisplayFunc(&myDisplay);	//当前窗口的回调函数
	glutKeyboardFunc(&keyboard_fun);
	glutMainLoop();		//启动消息循环，显示窗口
	return 0;
}