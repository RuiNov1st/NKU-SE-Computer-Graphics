/*
lab02:
Using a mouse and keyboard to do polyline editing.
Begin  (‘b’)  (create a new polyline)
Delete  (‘d’)  (delete the next point pointed to)
Move  (‘m’)  (drag the point pointed to to a new location)
Refresh  (‘r’)  (erase the screen and redraw all the polylines)
Quit   (‘q’)   (exit from the program)
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

const int MAX_NUM = 100;

//顶点的数据结构
struct GLintPoint {
	GLint x, y;
};
//存放多个顶点的数组
class GLintPointArray {
public:
	int num;
	vector<GLintPoint> pt;

GLintPointArray(): pt(MAX_NUM){}
};

//房子大小：
//房子的参数
GLintPoint peak;
GLint width = 300;
GLint height = 400;

//一些全局变量
boolean b_selected = FALSE;
boolean d_selected = FALSE;
boolean m_selected = FALSE;
boolean point_selected = FALSE;
vector<GLintPointArray*>house;//固定的房子图形
vector<GLintPointArray*>housee; //用来修改的房子
vector<GLintPointArray*>polylines;
GLintPointArray* polyline; //按b时画的折线

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

	//房子的参数
	peak.x = 480;
	peak.y = 400;
}

//画折线
void drawPolyLine(GLintPointArray* poly, int closed) {
	if (closed) {
		glBegin(GL_LINE_LOOP);
	}
	else {
		glBegin(GL_LINE_STRIP);
	}
	for (int i = 0; i < poly->num; i++) {
		glVertex2i(poly->pt[i].x, poly->pt[i].y);
	}
	glEnd();
	glFlush();
}

//画房子：
void display_fun1() {
	glClear(GL_COLOR_BUFFER_BIT);//清屏

	//分块画多边形：
	for (int i = 0; i < house.size(); i++) {
		drawPolyLine(house[i], TRUE);
	}

}

//改变点后的重画
void display_fun2() {
	glClear(GL_COLOR_BUFFER_BIT);//清屏

	//分块画多边形：
	for (int i = 0; i < housee.size(); i++) {
		drawPolyLine(housee[i], TRUE);
	}
	for (int i = 0; i < polylines.size(); i++) {
		drawPolyLine(polylines[i], TRUE);
	}
}

//什么都不干
void myMouse(int button, int state, int x, int y) {

}
//什么都不干
void myMoveMouse_0(int x, int y) {

}

//画多边形
void myMouse_1(int button, int state, int x, int y) {
	//static GLintPointArray* polyline = new GLintPointArray();
	//按左键
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			polyline->pt[polyline->num].x = x;
			polyline->pt[polyline->num].y = screenHeight - y; //翻转y的坐标
			polyline->num++;
			//画一个点标识
			glBegin(GL_POINT);
			glVertex2d(x, screenHeight - y);
			glEnd();
			glFlush();
		}
	}
	//超过两个点，则开始画线
	if (polyline->num >= 2) {
		drawPolyLine(polyline, FALSE);
	}
}

//删除点：
void myMouse_2(int button, int state, int x, int y) {
	y = screenHeight - y;
	//首先找到是哪个点被删了：
	//模糊查找：不一定正好是某个像素，可以四周5个像素内。
	//先查房子：
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			vector<GLintPointArray*> temp;
			for (int v = 0; v < 2; v++) {
				if (v == 0) {
					temp = housee;
				}
				else {
					temp = polylines;
				}
				for (int i = 0; i < temp.size(); i++) {
					for (int j = 0; j < temp[i]->num; j++) {
						if (temp[i]->pt[j].x >= x - 5 && temp[i]->pt[j].x <= x + 5 && temp[i]->pt[j].y >= y - 5 && temp[i]->pt[j].y <= y + 5) {
							//直接删掉这个
							temp[i]->pt.erase(temp[i]->pt.begin() + j);
							temp[i]->num--;
							display_fun2();
							break;
						}
					}
				}
			}
		}
	}
}

//按住某键移动鼠标
void myMoveMouse(int x, int y) {
	y = screenHeight - y; 
	static vector<GLintPointArray*> temp;
	static int i_fixed = 0;
	static int j_fixed = 0;
	//先确定是哪个
	if (!point_selected) {
		for (int v = 0; v < 2; v++) {
			if (v == 0 && !point_selected) {
				temp = housee;
			}
			else {
				if ((!point_selected)) {
					temp = polylines;
				}
			}
			for (int i = 0; i < temp.size(); i++) {
				for (int j = 0; j < temp[i]->num; j++) {
					if (temp[i]->pt[j].x >= x - 5 && temp[i]->pt[j].x <= x + 5 && temp[i]->pt[j].y >= y - 5 && temp[i]->pt[j].y <= y + 5) {
						i_fixed = i;
						j_fixed = j;
						point_selected = TRUE;
						break;
					}
				}
			}
		}
	}
	
	else {
		//更改位置
		temp[i_fixed]->pt[j_fixed].x = x;
		temp[i_fixed]->pt[j_fixed].y = y;
		//重画
		display_fun2();
	}
}

//放开鼠标
void myMouse_3(int button, int state, int x, int y) {
	y = screenHeight - y;
	if (state == GLUT_UP) {
		if (button == GLUT_LEFT_BUTTON) {
			point_selected = FALSE;
		}
	}
}

// 计算初始化房子的各个参数：
//需提供房子的顶点参数，房子的宽和高
void House_parameter(GLintPoint peak, GLint width, GLint height) {
	house.clear();
	//赋值时顺时针
	//外形
	GLintPointArray* shape = new GLintPointArray();
	shape->pt[0].x = peak.x;
	shape->pt[0].y = peak.y;
	shape->pt[1].x = peak.x + 1.0 / 2.0 * width;
	shape->pt[1].y = peak.y - 3.0 / 8.0 * height;
	shape->pt[2].x = peak.x + 1.0 / 2.0 * width;
	shape->pt[2].y = peak.y - height;
	shape->pt[3].x = peak.x - 1.0 / 2.0 * width;
	shape->pt[3].y = peak.y - height;
	shape->pt[4].x = peak.x - 1.0 / 2.0 * width;
	shape->pt[4].y = peak.y - 3.0 / 8.0 * height;
	//shape.pt[5].x = peak.x;
	//shape.pt[6].x = peak.y;
	shape->num = 5;
	house.push_back(shape);

	//窗口1
	GLintPointArray* win1 = new GLintPointArray();
	win1->pt[0].x = peak.x - 3.0 / 8.0 * width;
	win1->pt[0].y = peak.y - 3.0 / 8.0 * height;
	win1->pt[1].x = peak.x - 1.0 / 8.0 * width;
	win1->pt[1].y = peak.y - 3.0 / 8.0 * height;
	win1->pt[2].x = peak.x - 1.0 / 8.0 * width;
	win1->pt[2].y = peak.y - 3.0 / 8.0 * height - 5.0 / 32.0 * height;
	win1->pt[3].x = peak.x - 3.0 / 8.0 * width;
	win1->pt[3].y = peak.y - 3.0 / 8.0 * height - 5.0 / 32.0 * height;
	win1->num = 4;
	house.push_back(win1);

	//窗口2
	GLintPointArray* win2 = new GLintPointArray();
	win2->pt[0].x = peak.x + 1.0 / 8.0 * width;
	win2->pt[0].y = peak.y - 3.0 / 8.0 * height;
	win2->pt[1].x = peak.x + 3.0 / 8.0 * width;
	win2->pt[1].y = peak.y - 3.0 / 8.0 * height;
	win2->pt[2].x = peak.x + 3.0 / 8.0 * width;
	win2->pt[2].y = peak.y - 3.0 / 8.0 * height - 5.0 / 32.0 * height;
	win2->pt[3].x = peak.x + 1.0 / 8.0 * width;
	win2->pt[3].y = peak.y - 3.0 / 8.0 * height - 5.0 / 32.0 * height;
	win2->num = 4;
	house.push_back(win2);

	//门口
	GLintPointArray* door = new GLintPointArray();
	door->pt[0].x = peak.x - 3.0 / 8.0 * width;
	door->pt[0].y = peak.y - 3.0 / 8.0 * height - 5.0 / 16.0 * height;
	door->pt[1].x = peak.x - 1.0 / 8.0 * width;
	door->pt[1].y = peak.y - 3.0 / 8.0 * height - 5.0 / 16.0 * height;
	door->pt[2].x = peak.x - 1.0 / 8.0 * width;
	door->pt[2].y = peak.y - height;
	door->pt[3].x = peak.x - 3.0 / 8.0 * width;
	door->pt[3].y = peak.y - height;
	door->num = 4;
	house.push_back(door);

	//烟囱
	GLintPointArray* chimney = new GLintPointArray();
	chimney->pt[0].x = peak.x + 1.0 / 6.0 * width;
	chimney->pt[0].y = peak.y + 1.0 / 8.0 * height;
	chimney->pt[1].x = peak.x + 1.0 / 3.0 * width;
	chimney->pt[1].y = peak.y + 1.0 / 8.0 * height;
	//通过方程求对应斜边坐标
	double a = -3.0 / 4.0 * height / width;
	double b = peak.y + 3.0 / 4.0 * height / width * peak.x;
	//斜边的两个点：
	chimney->pt[2].x = peak.x + 1.0 / 3.0 * width;
	chimney->pt[2].y = a * chimney->pt[2].x + b;
	chimney->pt[3].x = peak.x + 1.0 / 6.0 * width;
	chimney->pt[3].y = a * chimney->pt[3].x + b;
	chimney->num = 4;
	house.push_back(chimney);

}

//定义键盘操作
void keyboard_fun(unsigned char key, int mouse_x, int mouse_y) {
	mouse_y = height - mouse_y;
	switch (key)
	{
	//refresh
	case 'r':
		House_parameter(peak, width, height);
		housee = house;
		polylines.clear();
		glutPostRedisplay();
		b_selected = FALSE;
		d_selected = FALSE;
		m_selected = FALSE;
		point_selected = FALSE;
		break;
	// create a new polyline
	case 'b':
		if (!b_selected) {
			polyline = new GLintPointArray();
			glutMouseFunc(&myMouse_1);
			b_selected = TRUE;
		}
		else {
			drawPolyLine(polyline, TRUE);
			polylines.push_back(polyline);
			b_selected = FALSE;
			glutMouseFunc(&myMouse);
		}
		
		break;
	//delete
	case 'd':
		if (!d_selected) {
			glutMouseFunc(&myMouse_2);
			d_selected = TRUE;
		}
		else {
			glutMouseFunc(&myMouse);
			d_selected = FALSE;
		}
		break;
	//drag
	case 'm':
		if (!m_selected) {
			glutMouseFunc(&myMouse_3);
			glutMotionFunc(&myMoveMouse);
			m_selected = TRUE;
		}
		else {
			glutMotionFunc(&myMoveMouse_0);
			glutMouseFunc(&myMouse);
			m_selected = FALSE;
			point_selected = FALSE;
		}
		break;
	//exit
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
	glutCreateWindow("lab02");	//创建窗口
	init();							//窗口初始化
	//计算房子中各种图形的参数：
	House_parameter(peak, width, height);
	housee = house;
	glutDisplayFunc(&display_fun1);
	glutKeyboardFunc(&keyboard_fun);	//键盘回调函数
	glutMainLoop();		//启动消息循环，显示窗口
	return 0;
}
