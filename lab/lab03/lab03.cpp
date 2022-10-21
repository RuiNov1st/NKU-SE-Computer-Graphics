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

//��Ķ���
struct GLintPoint {
	GLint x;
	GLint y;
	int code = 0;
};

//���
struct GLintPointPair {
	GLintPoint* p1;
	GLintPoint* p2;
	GLdouble delx;
	GLdouble  dely;
	int status;// 1 ����;-1�ܾ�;0δ���
};

//���ζ���
struct Rect {
	GLint l = 0;
	GLint r = 0;
	GLint t = 0;
	GLint b = 0;
};

//һЩȫ�ֶ���
vector<GLintPointPair*>lines;
Rect r;
int point_num = 100;//�߶ε�����
int color = 0;

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


//��;���������ʼ��
void lines_rect() {
	glClear(GL_COLOR_BUFFER_BIT);//����
	unsigned seed;
	seed = time(0);
	srand(seed);
	//�����������һ������Ϊ��������½�
	GLintPoint g1;
	g1.x = (rand() % ((screenWidth - 1 - rectWidth) - 1 + 1)) + 1;
	g1.y = (rand() % ((screenHeight - 1 - rectHeight) - 1 + 1)) + 1;
	//�������
	r.l = g1.x;
	r.r = g1.x + rectWidth;
	r.b = g1.y;
	r.t = g1.y + rectHeight;
	//������
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(r.l, r.b);
	glVertex2i(r.l, r.t);
	glVertex2i(r.r, r.t);
	glVertex2i(r.r, r.b);
	glEnd();
	glFlush();

	//��ʼ������ɵ㹹��ֱ��
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
//���߶�
void myDisplay() {
	
	if (color == 0) {
		glColor3f(0.0f, 0.0f, 1.0f);
	}
	else {
		glColor3f(0.0f, 1.0f, 0.0f);
		glLineWidth(3.0);

	}
	glBegin(GL_LINES);
	//���߶�
	for (int i = 0; i < lines.size(); i++) {
		if (color == 0) {
			glVertex2i(lines[i]->p1->x, lines[i]->p1->y);
			glVertex2i(lines[i]->p2->x, lines[i]->p2->y);
		}
		else {
			//ֻ���������
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
//��ÿ������б���ļ���
void code_compute(GLintPointPair* line) {
		//����delx dely��
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
			//�ж�λ��
			//��
			if (temp->x < r.l) {
				temp->code |= 8;
			}
			//��
			if (temp->y > r.t) {
				temp->code |= 4;
			}
			//��
			if (temp->x > r.r) {
				temp->code |= 2;
			}
			if (temp->y < r.b) {
				temp->code |= 1;
			}
		}
		
	}

//�����ȡ����
void ChopLine(GLintPointPair* pair, GLintPoint* p) {
	//���ݵ��λ�ý��д���
	//��
	if (p->code & 8) {
		p->y = p->y + (r.l-p->x) * (pair->dely / pair->delx);
		p->x = r.l;
	}
	else {
		//��
		if (p->code & 4) {
			p->x = p->x + (r.t-p->y) * (pair->delx / pair->dely);
			p->y = r.t;
		}
		else {
			//��
			if (p->code & 2) {
				p->y = p->y + (r.r-p->x) * (pair->dely/pair->delx);
				p->x = r.r;
			}
			else {
				//��
				if (p->code & 1) {
					p->x = p->x + (r.b-p->y) * (pair->delx / pair->dely);
					p->y = r.b;
				}
			}
		}
	}
}




//��ȡȫ���̺���
void process() {
	//��ÿ���߶ν����ж�
	for (int i = 0; i < lines.size(); i++) {
		int count = 0;
		do {
			//���ȼ���ÿ����ı���
			code_compute(lines[i]);
			//�ȶ�trival accept��trival reject�����ж�
			//trival accept����������0
			if ((lines[i]->p1->code | lines[i]->p2->code) == 0) {
				lines[i]->status = 1;
				break;
			}
			//trival reject������ĳһλ����1
			if((lines[i]->p1->code & lines[i]->p2->code) != 0||count>=4) {
				lines[i]->status = -1;
				break;
			}
			//��ȡ
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

//������̲���
void keyboard_fun(unsigned char key, int mouse_x, int mouse_y) {
	switch (key)
	{
	case 'r':
		process();
		break;
	case 'b':
		//��������������¿�ʼ
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
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	//��ʼ�����ڵ���ʾģʽ,������
	glutInitWindowPosition(100, 0);		//���ô��ڵ�λ��
	glutInitWindowSize(screenWidth, screenHeight);	//���ô��ڵ�λ��
	glutCreateWindow("lab03");	//��������
	init();							//���ڳ�ʼ��
	lines_rect();
	glutDisplayFunc(&myDisplay);	//��ǰ���ڵĻص�����
	glutKeyboardFunc(&keyboard_fun);
	glutMainLoop();		//������Ϣѭ������ʾ����
	return 0;
}