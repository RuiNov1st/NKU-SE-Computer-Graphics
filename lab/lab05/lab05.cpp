/*
lab05:
Cyrus-Beck
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

//要初始化的直线数量
int num = 50;

//点的对象
struct GLintPoint {
	GLint x;
	GLint y;
};

//线的对象
struct GLintLine {
	GLintPoint* first;
	GLintPoint* second;
	GLintPoint* norm;//法向量
	GLintPoint* direction;//方向向量

	//构造函数
	GLintLine(GLintPoint*a,GLintPoint*b) {
		first = a;
		second = b;
		norm = new GLintPoint();
		direction = new GLintPoint();
		compute_direction();
		compute_norm();
	}

	//计算方向向量
	void compute_direction() {
		direction->x = second->x - first->x;
		direction->y = second->y - first->y;
	}
	//计算法向量
	void compute_norm() {
		norm->x = direction->y * -1;
		norm->y = direction->x;
	}

};

vector<GLintPoint*>polygon; //多边形的点
vector<GLintLine*>polygon_line; // 多边形的边
vector<GLintLine*>line; //要裁剪的直线

//初始化
void init()
{
	glClearColor(1, 1, 1, 1);	//设置窗口背景色为白色
	glColor3f(0.0f, 0.0f, 1.0f);	//设置绘制的颜色为蓝色
	glPointSize(5.0);	//点的大小
	glLineWidth(4.0);	//线的宽度
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, (GLdouble)screenWidth, 0, (GLdouble)screenHeight);	//投影参数

}

//画出凸多边形
void drawPolyLine(vector<GLintPoint*>poly, int closed) {
	glLineWidth(4.0);	//线的宽度
	glColor3f(0.0f, 0.0f, 1.0f);
	if (closed) {
		glBegin(GL_LINE_LOOP);
	}
	else {
		glBegin(GL_LINE_STRIP);
	}
	for (int i = 0; i < poly.size(); i++) {
		glVertex2i(poly[i]->x, poly[i]->y);
	}
	glEnd();
	glFlush();
}

//画多边形
void myMouse_1(int button, int state, int x, int y) {
	//static GLintPointArray* polyline = new GLintPointArray();
	//按左键
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			GLintPoint* pt = new GLintPoint();
			pt->x = x;
			pt->y = screenHeight - y; //翻转y的坐标
			polygon.push_back(pt);
			//画一个点标识
			glBegin(GL_POINT);
			glVertex2d(x, screenHeight - y);
			glEnd();
			glFlush();
		}
	}
	//超过两个点，则开始画线
	if (polygon.size()>= 2) {
		drawPolyLine(polygon, FALSE);
	}
}

void myMouse_2(int button, int state, int x, int y) {
}

//直线初始化
void init_line() {
	unsigned seed;
	seed = time(0);
	srand(seed);
	for (int i = 0; i < num; i++) {
		GLintPoint* a = new GLintPoint();
		a->x = (rand() % (screenWidth - 1 - 1 + 1)) + 1;
		a->y = (rand() % (screenHeight - 1 - 1 + 1)) + 1;
		GLintPoint* b = new GLintPoint();
		b->x = (rand() % (screenWidth - 1 - 1 + 1)) + 1;
		b->y = (rand() % (screenHeight - 1 - 1 + 1)) + 1;
		GLintLine* l = new GLintLine(a,b);
		line.push_back(l);
	}

	glLineWidth(2.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	//画线
	for (int i = 0; i < line.size(); i++) {
		glVertex2i(line[i]->first->x, line[i]->first->y);
		glVertex2i(line[i]->second->x, line[i]->second->y);
	}
	glEnd();
	glFlush();
}


void display_fun1() {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);//清屏
	glFlush();
}

//初始化多边形的边
void polygonline_init() {
	for (int i = 0; i < polygon.size() - 1; i++) {
		GLintLine* line = new GLintLine(polygon[i], polygon[i + 1]);
		polygon_line.push_back(line);
	}
}

//计算叉乘：
int cross_product(GLintPoint* a, GLintPoint* b) {
	return a->x * b->y - a->y * b->x;
}

//计算点乘
int dot_product(GLintPoint*a, GLintPoint* b) {
	return a->x * b->x + a->y * b->y;

}

//判断是否为凸多边形
bool is_polygon() {
	int flag = 0;//判断正负号
	//计算n次叉乘并判断这些叉乘结果的k分量是否有相同的正负号
	for (int i = 0; i < polygon_line.size() - 1; i++) {
		int temp = cross_product(polygon_line[i]->direction, polygon_line[i + 1]->direction);
		cout <<"flag"<<flag<<"temp"<< temp << endl;
		//flag的初始化
		if (i == 0) {
			flag = temp;
		}
		else {
			//不同号
			if (double(flag) * double(temp) < 0) {
				return FALSE;
			}
			//同号
			else {
				flag = temp;
			}
		}
	}
	//相同方向，说明是凸多边形
	//判断方向向量的方向
	if (flag>0){
		//向外，则方向反了
		polygon_line.clear();
		//点逆向
		reverse(polygon.begin(), polygon.end());
		//重新计算polyine
		polygonline_init();
	}
	return TRUE;
}

int chopCI(double& tin, double& tout, double numer, double denom) {
	double thit;
	//射入
	if (denom < 0) {
		thit = numer / denom;
		if (thit > tout) {
			return 0;
		}
		//更新入射点
		if (thit > tin) {
			tin = thit;
		}

	}
	else {
		//射出
		if (denom > 0) {
			thit = numer / denom;
			if (thit < tin) {
				return 0;
			}
			//更新出射点
			if (thit < tout) {
				tout = thit;
			}
		}
		//平行
		else {
			//在外面的空间
			if (numer <= 0) {
				return 0;
			}
			return 1;
		}
	}
}



//凸多边形的裁剪流程
void process() {
	//画线
	glLineWidth(2.0);
	glColor3f(0.0f, 1.0f, 0.0f);

	for (int i = 0; i < line.size(); i++) {
		double numer, denom;
		double tin = 0.0, tout = 1.0;
		bool flag = TRUE; // 是否改变的指示变量

		GLintPoint* c = new GLintPoint(); // 待裁剪的直线的方向向量 
		GLintPoint* tmp = new GLintPoint();
		c->x = line[i]->second->x - line[i]->first->x;
		c->y = line[i]->second->y - line[i]->first->y;

		//用多边形的每一条边进行裁剪
		for (int j = 0; j < polygon_line.size(); j++) {
			//B-A:
			tmp->x = polygon_line[j]->first->x - line[i]->first->x;
			tmp->y = polygon_line[j]->first->y - line[i]->first->y;
			//法向量 dot (B-A)
			numer = dot_product(polygon_line[j]->norm, tmp);
			// 法向量 dot c
			denom = dot_product(polygon_line[j]->norm, c);

			//计算出入点
			//提前退出
			if (!chopCI(tin, tout, numer, denom)) {
				flag = FALSE;
				break;
			}	
		}

		//调整线段端点
		if (tout < 1.0) {
			line[i]->second->x = line[i]->first->x + c->x * tout;
			line[i]->second->y = line[i]->first->y + c->y * tout;
		}
		if (tin > 0.0) {
			line[i]->first->x = line[i]->first->x + c->x * tin;
			line[i]->first->y = line[i]->first->y + c->y * tin;
		}
		if (flag) {
			glBegin(GL_LINES);
			//画线
			glVertex2i(line[i]->first->x, line[i]->first->y);
			glVertex2i(line[i]->second->x, line[i]->second->y);
			glEnd();
			glFlush();
		}
	}


}



//定义键盘操作
void keyboard_fun(unsigned char key, int mouse_x, int mouse_y) {
	switch (key)
	{
	//停止
	case 'c':
		//把最后一个连线连上
		glBegin(GL_LINE_STRIP);
		glVertex2i(polygon[0]->x, polygon[0]->y);
		glVertex2i(polygon[polygon.size() - 1]->x, polygon[polygon.size() - 1]->y);
		glEnd();
		glFlush();

		//把第一个点加上，比较方便后续计算
		{
			GLintPoint* pt = new GLintPoint();
			pt->x = polygon[0]->x;
			pt->y = polygon[0]->y;
			polygon.push_back(pt);
		}
		//加入polyline
		polygonline_init();

		//判断是否为凸多边形
		if (!is_polygon())
		{
			cout << "这不是凸多边形，请重画！" << endl;
			glClear(GL_COLOR_BUFFER_BIT);//清屏
			polygon.clear();
			polygon_line.clear();
			break;
		}
		//是凸多边形，则开始画线
		init_line();
		//进行裁剪工作
		process();
		break;

	//刷新
	case 'r':
		polygon.clear();
		polygon_line.clear();
		line.clear();
		glClear(GL_COLOR_BUFFER_BIT);//清屏
		glFlush();
		//glutPostRedisplay();
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
	glutCreateWindow("lab05");	//创建窗口
	init();							//窗口初始化
	glutMouseFunc(&myMouse_1); // 鼠标函数
	glutKeyboardFunc(&keyboard_fun);//键盘函数
	glutDisplayFunc(&display_fun1);	//当前窗口的回调函数
	glutMainLoop();		//启动消息循环，显示窗口
	return 0;
}