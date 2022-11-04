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

//Ҫ��ʼ����ֱ������
int num = 50;

//��Ķ���
struct GLintPoint {
	GLint x;
	GLint y;
};

//�ߵĶ���
struct GLintLine {
	GLintPoint* first;
	GLintPoint* second;
	GLintPoint* norm;//������
	GLintPoint* direction;//��������

	//���캯��
	GLintLine(GLintPoint*a,GLintPoint*b) {
		first = a;
		second = b;
		norm = new GLintPoint();
		direction = new GLintPoint();
		compute_direction();
		compute_norm();
	}

	//���㷽������
	void compute_direction() {
		direction->x = second->x - first->x;
		direction->y = second->y - first->y;
	}
	//���㷨����
	void compute_norm() {
		norm->x = direction->y * -1;
		norm->y = direction->x;
	}

};

vector<GLintPoint*>polygon; //����εĵ�
vector<GLintLine*>polygon_line; // ����εı�
vector<GLintLine*>line; //Ҫ�ü���ֱ��

//��ʼ��
void init()
{
	glClearColor(1, 1, 1, 1);	//���ô��ڱ���ɫΪ��ɫ
	glColor3f(0.0f, 0.0f, 1.0f);	//���û��Ƶ���ɫΪ��ɫ
	glPointSize(5.0);	//��Ĵ�С
	glLineWidth(4.0);	//�ߵĿ��
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, (GLdouble)screenWidth, 0, (GLdouble)screenHeight);	//ͶӰ����

}

//����͹�����
void drawPolyLine(vector<GLintPoint*>poly, int closed) {
	glLineWidth(4.0);	//�ߵĿ��
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

//�������
void myMouse_1(int button, int state, int x, int y) {
	//static GLintPointArray* polyline = new GLintPointArray();
	//�����
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			GLintPoint* pt = new GLintPoint();
			pt->x = x;
			pt->y = screenHeight - y; //��תy������
			polygon.push_back(pt);
			//��һ�����ʶ
			glBegin(GL_POINT);
			glVertex2d(x, screenHeight - y);
			glEnd();
			glFlush();
		}
	}
	//���������㣬��ʼ����
	if (polygon.size()>= 2) {
		drawPolyLine(polygon, FALSE);
	}
}

void myMouse_2(int button, int state, int x, int y) {
}

//ֱ�߳�ʼ��
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
	//����
	for (int i = 0; i < line.size(); i++) {
		glVertex2i(line[i]->first->x, line[i]->first->y);
		glVertex2i(line[i]->second->x, line[i]->second->y);
	}
	glEnd();
	glFlush();
}


void display_fun1() {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);//����
	glFlush();
}

//��ʼ������εı�
void polygonline_init() {
	for (int i = 0; i < polygon.size() - 1; i++) {
		GLintLine* line = new GLintLine(polygon[i], polygon[i + 1]);
		polygon_line.push_back(line);
	}
}

//�����ˣ�
int cross_product(GLintPoint* a, GLintPoint* b) {
	return a->x * b->y - a->y * b->x;
}

//������
int dot_product(GLintPoint*a, GLintPoint* b) {
	return a->x * b->x + a->y * b->y;

}

//�ж��Ƿ�Ϊ͹�����
bool is_polygon() {
	int flag = 0;//�ж�������
	//����n�β�˲��ж���Щ��˽����k�����Ƿ�����ͬ��������
	for (int i = 0; i < polygon_line.size() - 1; i++) {
		int temp = cross_product(polygon_line[i]->direction, polygon_line[i + 1]->direction);
		cout <<"flag"<<flag<<"temp"<< temp << endl;
		//flag�ĳ�ʼ��
		if (i == 0) {
			flag = temp;
		}
		else {
			//��ͬ��
			if (double(flag) * double(temp) < 0) {
				return FALSE;
			}
			//ͬ��
			else {
				flag = temp;
			}
		}
	}
	//��ͬ����˵����͹�����
	//�жϷ��������ķ���
	if (flag>0){
		//���⣬������
		polygon_line.clear();
		//������
		reverse(polygon.begin(), polygon.end());
		//���¼���polyine
		polygonline_init();
	}
	return TRUE;
}

int chopCI(double& tin, double& tout, double numer, double denom) {
	double thit;
	//����
	if (denom < 0) {
		thit = numer / denom;
		if (thit > tout) {
			return 0;
		}
		//���������
		if (thit > tin) {
			tin = thit;
		}

	}
	else {
		//���
		if (denom > 0) {
			thit = numer / denom;
			if (thit < tin) {
				return 0;
			}
			//���³����
			if (thit < tout) {
				tout = thit;
			}
		}
		//ƽ��
		else {
			//������Ŀռ�
			if (numer <= 0) {
				return 0;
			}
			return 1;
		}
	}
}



//͹����εĲü�����
void process() {
	//����
	glLineWidth(2.0);
	glColor3f(0.0f, 1.0f, 0.0f);

	for (int i = 0; i < line.size(); i++) {
		double numer, denom;
		double tin = 0.0, tout = 1.0;
		bool flag = TRUE; // �Ƿ�ı��ָʾ����

		GLintPoint* c = new GLintPoint(); // ���ü���ֱ�ߵķ������� 
		GLintPoint* tmp = new GLintPoint();
		c->x = line[i]->second->x - line[i]->first->x;
		c->y = line[i]->second->y - line[i]->first->y;

		//�ö���ε�ÿһ���߽��вü�
		for (int j = 0; j < polygon_line.size(); j++) {
			//B-A:
			tmp->x = polygon_line[j]->first->x - line[i]->first->x;
			tmp->y = polygon_line[j]->first->y - line[i]->first->y;
			//������ dot (B-A)
			numer = dot_product(polygon_line[j]->norm, tmp);
			// ������ dot c
			denom = dot_product(polygon_line[j]->norm, c);

			//��������
			//��ǰ�˳�
			if (!chopCI(tin, tout, numer, denom)) {
				flag = FALSE;
				break;
			}	
		}

		//�����߶ζ˵�
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
			//����
			glVertex2i(line[i]->first->x, line[i]->first->y);
			glVertex2i(line[i]->second->x, line[i]->second->y);
			glEnd();
			glFlush();
		}
	}


}



//������̲���
void keyboard_fun(unsigned char key, int mouse_x, int mouse_y) {
	switch (key)
	{
	//ֹͣ
	case 'c':
		//�����һ����������
		glBegin(GL_LINE_STRIP);
		glVertex2i(polygon[0]->x, polygon[0]->y);
		glVertex2i(polygon[polygon.size() - 1]->x, polygon[polygon.size() - 1]->y);
		glEnd();
		glFlush();

		//�ѵ�һ������ϣ��ȽϷ����������
		{
			GLintPoint* pt = new GLintPoint();
			pt->x = polygon[0]->x;
			pt->y = polygon[0]->y;
			polygon.push_back(pt);
		}
		//����polyline
		polygonline_init();

		//�ж��Ƿ�Ϊ͹�����
		if (!is_polygon())
		{
			cout << "�ⲻ��͹����Σ����ػ���" << endl;
			glClear(GL_COLOR_BUFFER_BIT);//����
			polygon.clear();
			polygon_line.clear();
			break;
		}
		//��͹����Σ���ʼ����
		init_line();
		//���вü�����
		process();
		break;

	//ˢ��
	case 'r':
		polygon.clear();
		polygon_line.clear();
		line.clear();
		glClear(GL_COLOR_BUFFER_BIT);//����
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
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	//��ʼ�����ڵ���ʾģʽ,������
	glutInitWindowPosition(100, 0);		//���ô��ڵ�λ��
	glutInitWindowSize(screenWidth, screenHeight);	//���ô��ڵ�λ��
	glutCreateWindow("lab05");	//��������
	init();							//���ڳ�ʼ��
	glutMouseFunc(&myMouse_1); // ��꺯��
	glutKeyboardFunc(&keyboard_fun);//���̺���
	glutDisplayFunc(&display_fun1);	//��ǰ���ڵĻص�����
	glutMainLoop();		//������Ϣѭ������ʾ����
	return 0;
}