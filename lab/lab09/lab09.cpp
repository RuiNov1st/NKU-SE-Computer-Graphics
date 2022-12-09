/*
lab09:Mesh:
Meshes Stored in Files
Polygonalize a sphere with some control arguments to specify the resolution.
The result should be written to a file which can also be read for mesh display.
*/

#include <windows.h>
#include<math.h>
#include<gl/GL.h>
#include<gl/glut.h>
#include<iostream>
#include<vector>
#include<algorithm>
#include <ctime>
#include <fstream>
using namespace std;

// window size:
int screenWidth = 640;
int screenHeight = 480;

//��������ݽṹ
class point3 {
public:
	GLdouble x;
	GLdouble y;
	GLdouble z;

	point3() {

	}

	point3(GLdouble x, GLdouble y, GLdouble z) {
		x = x;
		y = y;
		z = z;
	}

};

//�����������ݽṹ
class vector3 {
public:	
	GLdouble x;
	GLdouble y;
	GLdouble z;

	vector3() {

	}

	vector3(GLdouble x, GLdouble y, GLdouble z) {
		x = x;
		y = y;
		z = z;
	}
	
};

//����ͷ�������������ϵ
class VertexID {
public:
	GLint vertIndex;
	GLint normIndex;
};

//��Ƭ�����ݽṹ
class Face {
public:
	int nVerts; //��Ƭ���еĶ�����
	VertexID* vert;//����ͷ���������ɵ��б�

	//���캯��
	Face() {
		nVerts = 0;
		vert = NULL;
	}
	//��������
	~Face() {
		delete[] vert;
		nVerts = 0;
	}
};


//����ε����ݽṹ
class Mesh {
public:
	int nFaces; //�ж��ٸ���
	Face* face;//��Ƭ����
	int npoints;//������ܹ��Ķ�����
	point3* pt;//���������
	int nNorms;//�ж��ٸ�������
	vector3* norm;//������������

	//��������
	int readMesh(string filepath) {
		fstream infile;
		infile.open(filepath, ios::in);

		//����
		if (infile.fail()) {
			cout << "�޷����ļ�" << endl;
			return -1;
		}
		//�ļ�Ϊ��
		if (infile.eof()) {
			cout << "�ļ�Ϊ��" << endl;
			return -1;
		}

		//���붥��������������������
		infile >> npoints >> nNorms >> nFaces;

		//�������ݽṹ
		pt = new point3[npoints];
		norm = new vector3[nNorms];
		face = new Face[nFaces];

		//����Ƿ񴴽��ɹ�
		if (!pt || !norm || !face) {
			cout << "�ڴ����ʧ��" << endl;
			return -1;
		}

		//��ȡ����
		for (int i = 0; i < npoints; i++) {
			infile >> pt[i].x >> pt[i].y >> pt[i].z;
		}
		//��ȡ������
		for (int i = 0; i < nNorms; i++) {
			infile >> norm[i].x >> norm[i].y >> norm[i].z;
		}
		//��ȡ�����Ϣ
		for (int i = 0; i < nFaces; i++) {
			infile >> face[i].nVerts;//������ж��ٸ�����
			face[i].vert = new VertexID[face[i].nVerts];
			//���������Ķ���ͷ������������г�ʼ��
			for (int j = 0; j < face[i].nVerts; j++) {
				infile >> face[i].vert[j].vertIndex;
			}
			for (int j = 0; j < face[i].nVerts; j++) {
				infile >> face[i].vert[j].normIndex;
			}
		}
		cout << "�������ݳɹ�" << endl;
		return 0;//�ļ���ȡ�ɹ�
	}

	//д������
	int writeMesh(string filepath) {
		fstream outfile;
		outfile.open(filepath, ios::out);

		//����
		if (outfile.is_open()) {
			if (!pt || !norm || !face) {
				cout << "��δ��ʼ��" << endl;
				return -1;
			}

			//д�붥��������������������
			outfile << npoints << " "<<nNorms << " " << nFaces << endl;



			//д�붥��
			for (int i = 0; i < npoints; i++) {
				outfile << pt[i].x << " " << pt[i].y << " " << pt[i].z <<endl;
			}
			//д�뷨����
			for (int i = 0; i < nNorms; i++) {
				outfile << norm[i].x << " " << norm[i].y << " " << norm[i].z << endl;
			}
			outfile << endl;
			//д�������Ϣ
			for (int i = 0; i < nFaces; i++) {
				outfile << face[i].nVerts << " ";//������ж��ٸ�����

				//д���������Ķ���ͷ���������
				for (int j = 0; j < face[i].nVerts; j++) {
					outfile << face[i].vert[j].vertIndex << " " ;
				}
				for (int j = 0; j < face[i].nVerts; j++) {
					outfile<< face[i].vert[j].normIndex << " ";
				}
				outfile << endl;
			}
			cout << "д�����ݳɹ�" << endl;
			return 0;//�ļ�д��ɹ�
		}
		else {
			cout << "�ļ���ʧ��" << endl;
			return -1;//�ļ�д��ʧ��
		}
	}
	//���ƶ����
	void draw(bool line) {
		
		if (!line) {
			//�������ʽ���л��ƣ�
			for (int i = 0; i < nFaces; i++) {
				glBegin(GL_POLYGON);

				for (int j = 0; j < face[i].nVerts; j++) {
					//��������
					int vertexid = face[i].vert[j].vertIndex;
					//��������
					int normid = face[i].vert[j].normIndex;
					cout << "face" << i << "vert" << j << endl;
					//���Ʒ�����
					glNormal3f(norm[normid].x, norm[normid].y, norm[normid].z);
					//cout << norm[normid].x << norm[normid].y << norm[normid].z << endl;
					//���ƶ���
					glVertex3f(pt[vertexid].x, pt[vertexid].y, pt[vertexid].z);
					//cout << pt[vertexid].x<<pt[vertexid].y<<pt[vertexid].z << endl;

				}

				glEnd();
				glFlush();
			}
		}else{
		
		//���ߵ���ʽ���ƣ�
		for (int i = 0; i < nFaces; i++) {
			glBegin(GL_LINE_LOOP);

			for (int j = 0; j < face[i].nVerts; j++) {
				//��������
				int vertexid = face[i].vert[j].vertIndex;
				//���ƶ���
				glVertex3f(pt[vertexid].x, pt[vertexid].y, pt[vertexid].z);
			}

			glEnd();
			glFlush();
		}
		}
	}

};

//��ʼ����
void init()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0 * 64 / 48.0, 2.0 * 64 / 48.0, -2.0, 2.0, -2, 2);//ͶӰ����
	glClearColor(0, 0, 0, 1);	//���ô��ڱ���ɫΪ��ɫ
	glRotatef(30, 1, 1, 1);

	glColor3f(1.0f, 1.0f, 1.0f);	//���û��Ƶ���ɫΪ��ɫ
	//glPointSize(5.0);	//��Ĵ�С
	glLineWidth(2.0);	//�ߵĿ��
}


//�������᣺
void axis(double length) {
	glColor3f(1.0f, 0.0f, 1.0f);	//���û��Ƶ���ɫΪ��ɫ
	//x y z
	//z�᣺
	glBegin(GL_LINES);
	glVertex3d(0, 0,-1*length); 
	glVertex3d(0, 0, length);
	glEnd();
	//y��
	glBegin(GL_LINES);
	glVertex3d(0, -1 * length,0);
	glVertex3d(0, length,0);
	glEnd();
	//x��
	glBegin(GL_LINES);
	glVertex3d(-1 * length,0, 0);
	glVertex3d(length, 0,  0);
	glEnd();

	glFlush();
}

//����������̣�
void display_fun1() {
	glClear(GL_COLOR_BUFFER_BIT);
	//�������᣺
	axis(50);

	glColor3f(1.0f, 1.0f, 1.0f);
	string input_path = "D:\\��ѧ\\ѧϰ\\���\\�����ͼ��ѧ\\lab\\lab9\\lab9.txt";
	string output_path = "D:\\��ѧ\\ѧϰ\\���\\�����ͼ��ѧ\\lab\\lab9\\lab9_new.txt";
	Mesh mesh;
	mesh.readMesh(input_path);//��������
	mesh.draw(TRUE);//��ͼ
	mesh.writeMesh(output_path);//�������
}



int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	//��ʼ�����ڵ���ʾģʽ,������
	glutInitWindowPosition(100, 0);		//���ô��ڵ�λ��
	glutInitWindowSize(screenWidth, screenHeight);	//���ô��ڵ�λ��
	glutCreateWindow("lab09");	//��������
	init();							//���ڳ�ʼ��
	glutDisplayFunc(&display_fun1);	//��ǰ���ڵĻص�����
	glutMainLoop();		//������Ϣѭ������ʾ����
	return 0;
}