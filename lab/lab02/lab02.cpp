/*
lab02:
Using a mouse and keyboard to do polyline editing.
Begin  (��b��)  (create a new polyline)
Delete  (��d��)  (delete the next point pointed to)
Move  (��m��)  (drag the point pointed to to a new location)
Refresh  (��r��)  (erase the screen and redraw all the polylines)
Quit   (��q��)   (exit from the program)
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

//��������ݽṹ
struct GLintPoint {
	GLint x, y;
};
//��Ŷ�����������
class GLintPointArray {
public:
	int num;
	vector<GLintPoint> pt;

GLintPointArray(): pt(MAX_NUM){}
};

//���Ӵ�С��
//���ӵĲ���
GLintPoint peak;
GLint width = 300;
GLint height = 400;

//һЩȫ�ֱ���
boolean b_selected = FALSE;
boolean d_selected = FALSE;
boolean m_selected = FALSE;
boolean point_selected = FALSE;
vector<GLintPointArray*>house;//�̶��ķ���ͼ��
vector<GLintPointArray*>housee; //�����޸ĵķ���
vector<GLintPointArray*>polylines;
GLintPointArray* polyline; //��bʱ��������

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

	//���ӵĲ���
	peak.x = 480;
	peak.y = 400;
}

//������
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

//�����ӣ�
void display_fun1() {
	glClear(GL_COLOR_BUFFER_BIT);//����

	//�ֿ黭����Σ�
	for (int i = 0; i < house.size(); i++) {
		drawPolyLine(house[i], TRUE);
	}

}

//�ı�����ػ�
void display_fun2() {
	glClear(GL_COLOR_BUFFER_BIT);//����

	//�ֿ黭����Σ�
	for (int i = 0; i < housee.size(); i++) {
		drawPolyLine(housee[i], TRUE);
	}
	for (int i = 0; i < polylines.size(); i++) {
		drawPolyLine(polylines[i], TRUE);
	}
}

//ʲô������
void myMouse(int button, int state, int x, int y) {

}
//ʲô������
void myMoveMouse_0(int x, int y) {

}

//�������
void myMouse_1(int button, int state, int x, int y) {
	//static GLintPointArray* polyline = new GLintPointArray();
	//�����
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			polyline->pt[polyline->num].x = x;
			polyline->pt[polyline->num].y = screenHeight - y; //��תy������
			polyline->num++;
			//��һ�����ʶ
			glBegin(GL_POINT);
			glVertex2d(x, screenHeight - y);
			glEnd();
			glFlush();
		}
	}
	//���������㣬��ʼ����
	if (polyline->num >= 2) {
		drawPolyLine(polyline, FALSE);
	}
}

//ɾ���㣺
void myMouse_2(int button, int state, int x, int y) {
	y = screenHeight - y;
	//�����ҵ����ĸ��㱻ɾ�ˣ�
	//ģ�����ң���һ��������ĳ�����أ���������5�������ڡ�
	//�Ȳ鷿�ӣ�
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
							//ֱ��ɾ�����
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

//��סĳ���ƶ����
void myMoveMouse(int x, int y) {
	y = screenHeight - y; 
	static vector<GLintPointArray*> temp;
	static int i_fixed = 0;
	static int j_fixed = 0;
	//��ȷ�����ĸ�
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
		//����λ��
		temp[i_fixed]->pt[j_fixed].x = x;
		temp[i_fixed]->pt[j_fixed].y = y;
		//�ػ�
		display_fun2();
	}
}

//�ſ����
void myMouse_3(int button, int state, int x, int y) {
	y = screenHeight - y;
	if (state == GLUT_UP) {
		if (button == GLUT_LEFT_BUTTON) {
			point_selected = FALSE;
		}
	}
}

// �����ʼ�����ӵĸ���������
//���ṩ���ӵĶ�����������ӵĿ�͸�
void House_parameter(GLintPoint peak, GLint width, GLint height) {
	house.clear();
	//��ֵʱ˳ʱ��
	//����
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

	//����1
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

	//����2
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

	//�ſ�
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

	//�̴�
	GLintPointArray* chimney = new GLintPointArray();
	chimney->pt[0].x = peak.x + 1.0 / 6.0 * width;
	chimney->pt[0].y = peak.y + 1.0 / 8.0 * height;
	chimney->pt[1].x = peak.x + 1.0 / 3.0 * width;
	chimney->pt[1].y = peak.y + 1.0 / 8.0 * height;
	//ͨ���������Ӧб������
	double a = -3.0 / 4.0 * height / width;
	double b = peak.y + 3.0 / 4.0 * height / width * peak.x;
	//б�ߵ������㣺
	chimney->pt[2].x = peak.x + 1.0 / 3.0 * width;
	chimney->pt[2].y = a * chimney->pt[2].x + b;
	chimney->pt[3].x = peak.x + 1.0 / 6.0 * width;
	chimney->pt[3].y = a * chimney->pt[3].x + b;
	chimney->num = 4;
	house.push_back(chimney);

}

//������̲���
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
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	//��ʼ�����ڵ���ʾģʽ,������
	glutInitWindowPosition(100, 0);		//���ô��ڵ�λ��
	glutInitWindowSize(screenWidth, screenHeight);	//���ô��ڵ�λ��
	glutCreateWindow("lab02");	//��������
	init();							//���ڳ�ʼ��
	//���㷿���и���ͼ�εĲ�����
	House_parameter(peak, width, height);
	housee = house;
	glutDisplayFunc(&display_fun1);
	glutKeyboardFunc(&keyboard_fun);	//���̻ص�����
	glutMainLoop();		//������Ϣѭ������ʾ����
	return 0;
}
