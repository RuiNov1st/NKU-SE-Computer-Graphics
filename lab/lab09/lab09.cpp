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

//顶点的数据结构
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

//法向量的数据结构
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

//顶点和法向量的索引关系
class VertexID {
public:
	GLint vertIndex;
	GLint normIndex;
};

//面片的数据结构
class Face {
public:
	int nVerts; //面片的中的顶点数
	VertexID* vert;//顶点和法线索引组成的列表

	//构造函数
	Face() {
		nVerts = 0;
		vert = NULL;
	}
	//析构函数
	~Face() {
		delete[] vert;
		nVerts = 0;
	}
};


//多边形的数据结构
class Mesh {
public:
	int nFaces; //有多少个面
	Face* face;//面片数组
	int npoints;//多边形总共的顶点数
	point3* pt;//顶点的数组
	int nNorms;//有多少个法向量
	vector3* norm;//法向量的数组

	//读入数据
	int readMesh(string filepath) {
		fstream infile;
		infile.open(filepath, ios::in);

		//报错
		if (infile.fail()) {
			cout << "无法打开文件" << endl;
			return -1;
		}
		//文件为空
		if (infile.eof()) {
			cout << "文件为空" << endl;
			return -1;
		}

		//读入顶点数、法向量数、面数
		infile >> npoints >> nNorms >> nFaces;

		//构建数据结构
		pt = new point3[npoints];
		norm = new vector3[nNorms];
		face = new Face[nFaces];

		//检查是否创建成功
		if (!pt || !norm || !face) {
			cout << "内存分配失败" << endl;
			return -1;
		}

		//读取顶点
		for (int i = 0; i < npoints; i++) {
			infile >> pt[i].x >> pt[i].y >> pt[i].z;
		}
		//读取法向量
		for (int i = 0; i < nNorms; i++) {
			infile >> norm[i].x >> norm[i].y >> norm[i].z;
		}
		//读取面的信息
		for (int i = 0; i < nFaces; i++) {
			infile >> face[i].nVerts;//这个面有多少个顶点
			face[i].vert = new VertexID[face[i].nVerts];
			//对这个面里的顶点和法向量索引进行初始化
			for (int j = 0; j < face[i].nVerts; j++) {
				infile >> face[i].vert[j].vertIndex;
			}
			for (int j = 0; j < face[i].nVerts; j++) {
				infile >> face[i].vert[j].normIndex;
			}
		}
		cout << "读入数据成功" << endl;
		return 0;//文件读取成功
	}

	//写入数据
	int writeMesh(string filepath) {
		fstream outfile;
		outfile.open(filepath, ios::out);

		//报错
		if (outfile.is_open()) {
			if (!pt || !norm || !face) {
				cout << "尚未初始化" << endl;
				return -1;
			}

			//写入顶点数、法向量数、面数
			outfile << npoints << " "<<nNorms << " " << nFaces << endl;



			//写入顶点
			for (int i = 0; i < npoints; i++) {
				outfile << pt[i].x << " " << pt[i].y << " " << pt[i].z <<endl;
			}
			//写入法向量
			for (int i = 0; i < nNorms; i++) {
				outfile << norm[i].x << " " << norm[i].y << " " << norm[i].z << endl;
			}
			outfile << endl;
			//写入面的信息
			for (int i = 0; i < nFaces; i++) {
				outfile << face[i].nVerts << " ";//这个面有多少个顶点

				//写入这个面里的顶点和法向量索引
				for (int j = 0; j < face[i].nVerts; j++) {
					outfile << face[i].vert[j].vertIndex << " " ;
				}
				for (int j = 0; j < face[i].nVerts; j++) {
					outfile<< face[i].vert[j].normIndex << " ";
				}
				outfile << endl;
			}
			cout << "写入数据成功" << endl;
			return 0;//文件写入成功
		}
		else {
			cout << "文件打开失败" << endl;
			return -1;//文件写入失败
		}
	}
	//绘制多边形
	void draw(bool line) {
		
		if (!line) {
			//以面的形式进行绘制：
			for (int i = 0; i < nFaces; i++) {
				glBegin(GL_POLYGON);

				for (int j = 0; j < face[i].nVerts; j++) {
					//顶点索引
					int vertexid = face[i].vert[j].vertIndex;
					//法线索引
					int normid = face[i].vert[j].normIndex;
					cout << "face" << i << "vert" << j << endl;
					//绘制法向量
					glNormal3f(norm[normid].x, norm[normid].y, norm[normid].z);
					//cout << norm[normid].x << norm[normid].y << norm[normid].z << endl;
					//绘制顶点
					glVertex3f(pt[vertexid].x, pt[vertexid].y, pt[vertexid].z);
					//cout << pt[vertexid].x<<pt[vertexid].y<<pt[vertexid].z << endl;

				}

				glEnd();
				glFlush();
			}
		}else{
		
		//以线的形式绘制：
		for (int i = 0; i < nFaces; i++) {
			glBegin(GL_LINE_LOOP);

			for (int j = 0; j < face[i].nVerts; j++) {
				//顶点索引
				int vertexid = face[i].vert[j].vertIndex;
				//绘制顶点
				glVertex3f(pt[vertexid].x, pt[vertexid].y, pt[vertexid].z);
			}

			glEnd();
			glFlush();
		}
		}
	}

};

//初始化：
void init()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0 * 64 / 48.0, 2.0 * 64 / 48.0, -2.0, 2.0, -2, 2);//投影参数
	glClearColor(0, 0, 0, 1);	//设置窗口背景色为白色
	glRotatef(30, 1, 1, 1);

	glColor3f(1.0f, 1.0f, 1.0f);	//设置绘制的颜色为蓝色
	//glPointSize(5.0);	//点的大小
	glLineWidth(2.0);	//线的宽度
}


//画坐标轴：
void axis(double length) {
	glColor3f(1.0f, 0.0f, 1.0f);	//设置绘制的颜色为蓝色
	//x y z
	//z轴：
	glBegin(GL_LINES);
	glVertex3d(0, 0,-1*length); 
	glVertex3d(0, 0, length);
	glEnd();
	//y轴
	glBegin(GL_LINES);
	glVertex3d(0, -1 * length,0);
	glVertex3d(0, length,0);
	glEnd();
	//x轴
	glBegin(GL_LINES);
	glVertex3d(-1 * length,0, 0);
	glVertex3d(length, 0,  0);
	glEnd();

	glFlush();
}

//画多边形流程：
void display_fun1() {
	glClear(GL_COLOR_BUFFER_BIT);
	//画坐标轴：
	axis(50);

	glColor3f(1.0f, 1.0f, 1.0f);
	string input_path = "D:\\大学\\学习\\编程\\计算机图形学\\lab\\lab9\\lab9.txt";
	string output_path = "D:\\大学\\学习\\编程\\计算机图形学\\lab\\lab9\\lab9_new.txt";
	Mesh mesh;
	mesh.readMesh(input_path);//读入数据
	mesh.draw(TRUE);//画图
	mesh.writeMesh(output_path);//输出数据
}



int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	//初始化窗口的显示模式,单缓冲
	glutInitWindowPosition(100, 0);		//设置窗口的位置
	glutInitWindowSize(screenWidth, screenHeight);	//设置窗口的位置
	glutCreateWindow("lab09");	//创建窗口
	init();							//窗口初始化
	glutDisplayFunc(&display_fun1);	//当前窗口的回调函数
	glutMainLoop();		//启动消息循环，显示窗口
	return 0;
}