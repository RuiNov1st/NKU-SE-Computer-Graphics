/*
照明模型：
设置一个点光源，给网格设置各种材质属性，
包括环境光，漫射光和镜面反射光分量。
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
#include<cmath>
#include"Light.h"

using namespace std;

//点类
Point3::Point3(){}
Point3::Point3(GLdouble xx, GLdouble yy, GLdouble zz) {
		x = xx;
		y = yy;
		z = zz;
}
void Point3::set(Point3 p) {
		x = p.x;
		y = p.y;
		z = p.z;
}
void Point3::set(GLdouble xx, GLdouble yy, GLdouble zz) {
		x = xx;
		y = yy;
		z = zz;
}

//向量的数据结构
Vector3::Vector3() {}

Vector3::Vector3(GLdouble xx, GLdouble yy, GLdouble zz) {
		x = xx;
		y = yy;
		z = zz;
}
	//赋值
void Vector3::set(GLdouble xx, GLdouble yy, GLdouble zz) {
		x = xx;
		y = yy;
		z = zz;

}

//向量点乘：
double Vector3::dot(Vector3 v) {
		return x * v.x + y * v.y + z * v.z;
}
//向量叉乘：
Vector3* Vector3::cross(Vector3 v) {
	Vector3* t = new Vector3();
	t->x = y * v.z - z * v.y;
	t->y = x * v.z - z * v.x;
	t->z = x * v.y - y * v.x;

	return t;
}
//向量归一化：
void Vector3::normalize() {
	double n = pow(x * x + y * y + z * z, 0.5);
	x = x / n;
	y = y / n;
	z = z / n;
}


//光源设置
Light::Light() {}
Light::Light(int name) {
		lightname = name;
		//默认光源
		diff[0] = 1.0;
		diff[1] = 1.0;
		diff[2] = 1.0;
		diff[3] = 1.0;
		glLightfv(lightname, GL_DIFFUSE, diff);
		glEnable(lightname);
		isMove = true;
	}
	//设置光源位置
	void Light::setPosition(float x, float y, float z, float dis) {
		lightPosition[0] = x;
		lightPosition[1] = y;
		lightPosition[2] = z;
		lightPosition[3] = dis;
		glLightfv(lightname, GL_POSITION, lightPosition);

	}
	//设置光源
	void Light::setAmb(float r, float g, float b, float alpha) {
		amb[0] = r;
		amb[1] = g;
		amb[2] = b;
		amb[3] = alpha;
		glLightfv(lightname, GL_AMBIENT, amb);
	}
	void Light::setDiff(float r, float g, float b, float alpha) {
		diff[0] = r;
		diff[1] = g;
		diff[2] = b;
		diff[3] = alpha;
		glLightfv(lightname, GL_DIFFUSE, diff);
	}
	void Light::setSpec(float r, float g, float b, float alpha) {
		spec[0] = r;
		spec[1] = g;
		spec[2] = b;
		spec[3] = alpha;
		glLightfv(lightname, GL_SPECULAR, spec);
	}
	//设置为探照灯
	void Light::setTorch(float cutoff, float exponent, float x, float y, float z) {
		glLightf(lightname, GL_SPOT_CUTOFF, cutoff);//光源覆盖角度的一半
		glLightf(lightname, GL_SPOT_EXPONENT, exponent);//聚光的 程度
		dir[0] = x;
		dir[1] = y;
		dir[2] = z;
		glLightfv(lightname, GL_SPOT_DIRECTION, dir);
	}
	//取消探照灯模式
	void Light::setNormal() {
		glLightf(lightname, GL_SPOT_CUTOFF, 180);//光源覆盖角度的一半
		glLightf(lightname, GL_SPOT_EXPONENT, 0);//聚光的 程度
		dir[0] = 0;
		dir[1] = 0;
		dir[2] = -1;
		glLightfv(lightname, GL_SPOT_DIRECTION, dir);
}

	//全局对象
Light l = Light(GL_LIGHT0);

//相机类
Camera::Camera() {}

void Camera::Camera_init(double eyex, double eyey, double eyez, double lookx, double looky, double lookz, double upx, double upy, double upz) {
	eye.set(eyex, eyey, eyez);
	look.set(lookx, looky, lookz);
	up.set(upx, upy, upz);
}

//模型视点矩阵设置
void Camera::setModelViewMatrix() {
	//根据参数计算Viewing Matrix
	float v_m[16]; //先行再列
	v_m[0] = u.x;
	v_m[1] = v.x;
	v_m[2] = n.x;
	v_m[3] = 0;
	v_m[4] = u.y;
	v_m[5] = v.y;
	v_m[6] = n.y;
	v_m[7] = 0;
	v_m[8] = u.z;
	v_m[9] = v.z;
	v_m[10] = n.z;
	v_m[11] = 0;
	//最右边一列是： -M.t .* eye 
	Vector3 eyev = Vector3(eye.x, eye.y, eye.z);
	v_m[12] = -1 * eyev.dot(u);
	v_m[13] = -1 * eyev.dot(v);
	v_m[14] = -1 * eyev.dot(n);
	v_m[15] = 1;

	//载入modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(v_m);

	if (l.isMove) {
		l.setPosition(eye.x, eye.y, eye.z, 1.0);
	}

}

//计算u.v.n
void Camera::set() {
	//n
	n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);

	//u
	Vector3 up_vec = Vector3(up.x, up.y, up.z);
	Vector3* t = up_vec.cross(n);
	u.set(t->x, t->y, t->z);

	//v
	Vector3* tt = n.cross(u);
	v.set(tt->x, tt->y, tt->z);

	delete t, tt;

	//归一化
	n.normalize();
	u.normalize();
	v.normalize();

	//调整模型视点矩阵
	setModelViewMatrix();
}

//设置视景体
void Camera::setShape(double viewAngle, double aspect, double nearDist, double farDist) {
	ViewAngle = viewAngle;
	aspect = aspect;
	nearDist = nearDist;
	farDist = farDist;

	//设置投影矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(viewAngle, aspect, nearDist, farDist);

}

//相机移动
void Camera::slide(float delU, float delV, float delN) {
	//改变视点
	eye.x += delU * u.x + delV * v.x + delN * n.x;
	eye.y += delU * u.y + delV * v.y + delN * n.y;
	eye.z += delU * u.z + delV * v.z + delN * n.z;
	//同步改变look
	look.x += delU * u.x + delV * v.x + delN * n.x;
	look.y += delU * u.y + delV * v.y + delN * n.y;
	look.z += delU * u.z + delV * v.z + delN * n.z;
	//调整模型视点矩阵
	setModelViewMatrix();
}

//相机旋转：绕n轴旋转
void Camera::roll(float angle) {
	float cosv = cos(3.14159265 / 180 * angle);
	float sinv = sin(3.14159265 / 180 * angle);

	//保存u的值，之后要修改
	Vector3 t;
	t.set(u.x, u.y, u.z);

	u.set(cosv * t.x + sinv * v.x, cosv * t.y + sinv * v.y, cosv * t.z + sinv * v.z);
	v.set(-1.0 * sinv * t.x + cosv * v.x, -1.0 * sinv * t.y + cosv * v.y, -1.0 * sinv * t.z + cosv * v.z);

	//设置视点模型矩阵
	setModelViewMatrix();
}
//相机偏航：绕v轴旋转
void Camera::yaw(float angle) {
	float cosv = cos(3.14159265 / 180 * angle);
	float sinv = sin(3.14159265 / 180 * angle);
	//保存u的值，之后要修改
	Vector3 t;
	t.set(n.x, n.y, n.z);

	n.set(cosv * t.x + sinv * u.x, cosv * t.y + sinv * u.y, cosv * t.z + sinv * u.z);
	u.set(-1.0 * sinv * t.x + cosv * u.x, -1.0 * sinv * t.y + cosv * u.y, -1.0 * sinv * t.z + cosv * u.z);

	//设置视点模型矩阵
	setModelViewMatrix();
}
//相机俯仰：绕u轴旋转
void Camera::pitch(float angle) {
	float cosv = cos(3.14159265 / 180 * angle);
	float sinv = sin(3.14159265 / 180 * angle);
	//保存u的值，之后要修改
	Vector3 t;
	t.set(v.x, v.y, v.z);

	v.set(cosv * t.x + sinv * n.x, cosv * t.y + sinv * n.y, cosv * t.z + sinv * n.z);
	n.set(-1.0 * sinv * t.x + cosv * n.x, -1.0 * sinv * t.y + cosv * n.y, -1.0 * sinv * t.z + cosv * n.z);
	//设置视点模型矩阵
	setModelViewMatrix();
}

//相机全局对象
Camera cam;


//材质

//设置材质
void Material::setMeterial(unsigned char key) {
	switch (key)
	{
		//silver
	case 'S':
		mat_ambient[0] = 0.19225;
		mat_ambient[1] = 0.19225;
		mat_ambient[2] = 0.19225;
		mat_ambient[3] = 1.0;
		mat_diffuse[0] = 0.50754;
		mat_diffuse[1] = 0.50754;
		mat_diffuse[2] = 0.50754;
		mat_diffuse[3] = 1.0;
		mat_specular[0] = 0.508273;
		mat_specular[1] = 0.508273;
		mat_specular[2] = 0.508273;
		mat_specular[3] = 1.0;
		mat_shininess[0] = 51.2;
		break;
		//铜
	case 'C':
		mat_ambient[0] = 0.19125;
		mat_ambient[1] = 0.0735;
		mat_ambient[2] = 0.0225;
		mat_ambient[3] = 1.0;
		mat_diffuse[0] = 0.7038;
		mat_diffuse[1] = 0.27048;
		mat_diffuse[2] = 0.0828;
		mat_diffuse[3] = 1.0;
		mat_specular[0] = 0.256777;
		mat_specular[1] = 0.17622;
		mat_specular[2] = 0.06014;
		mat_specular[3] = 1.0;
		mat_shininess[0] = 12.8;
		break;
		//紫罗兰
	case 'V':
		mat_ambient[0] = 0.11;
		mat_ambient[1] = 0.06;
		mat_ambient[2] = 0.09;
		mat_ambient[3] = 1.0;
		mat_diffuse[0] = 0.43;
		mat_diffuse[1] = 0.47;
		mat_diffuse[2] = 0.54;
		mat_diffuse[3] = 1.0;
		mat_specular[0] = 0.33;
		mat_specular[1] = 0.33;
		mat_specular[2] = 0.52;
		mat_specular[3] = 1.0;
		mat_shininess[0] = 22.0;
		break;
		//红宝石
	case 'J':
		mat_ambient[0] = 0.1745;
		mat_ambient[1] = 0.01175;
		mat_ambient[2] = 0.01175;
		mat_ambient[3] = 0.55;
		mat_diffuse[0] = 0.614240;
		mat_diffuse[1] = 0.041360;
		mat_diffuse[2] = 0.041360;
		mat_diffuse[3] = 0.55;
		mat_specular[0] = 0.72811;
		mat_specular[1] = 0.626959;
		mat_specular[2] = 0.626959;
		mat_specular[3] = 0.55;
		mat_shininess[0] = 76.8;
		break;
		//灰色
	case 'G':
		mat_ambient[0] = 0.7;
		mat_ambient[1] = 0.7;
		mat_ambient[2] = 0.7;
		mat_ambient[3] = 1.0;
		mat_diffuse[0] = 0.6;
		mat_diffuse[1] = 0.6;
		mat_diffuse[2] = 0.6;
		mat_diffuse[3] = 1.0;
		mat_specular[0] = 1.0;
		mat_specular[1] = 1.0;
		mat_specular[2] = 1.0;
		mat_specular[3] = 1.0;
		mat_shininess[0] = 50.0;
		break;
	default:
		break;
	}

}



//物体对象
//设置物体材质
void GemObj::TellMaterial(unsigned char key) {
	mat.setMeterial(key);
	//使用颜色跟踪
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glColor4f(mat.mat_ambient[0], mat.mat_ambient[1], mat.mat_ambient[2], mat.mat_ambient[3]);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glColor4f(mat.mat_diffuse[0], mat.mat_diffuse[1], mat.mat_diffuse[2], mat.mat_diffuse[3]);
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	glColor4f(mat.mat_specular[0], mat.mat_specular[1], mat.mat_specular[2], mat.mat_specular[3]);

	glDisable(GL_COLOR_MATERIAL);
}

void Table::setKey(unsigned char m_key) {
	key = m_key;
}

//桌腿
void Table::tableLeg(double thick, double len) {
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}

//画桌子
void Table::drawObj(double topWid, double topThick, double legThick, double legLen) {
	//桌面
	glPushMatrix();
	TellMaterial('S');
	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);
	glutSolidCube(1.0);
	glPopMatrix();

	//四条腿
	double dist = 0.95 * topWid / 2.0 - legThick / 2.0;
	glPushMatrix();
	TellMaterial(key);
	glTranslated(dist, 0, dist);
	tableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	tableLeg(legThick, legLen);
	glTranslated(-2 * dist, 0, 2 * dist);
	tableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	tableLeg(legThick, legLen);
	glPopMatrix();

}

Table t;


//光源跟随相机移动
void displayMove() {
	//画图
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//桌子
	glPushMatrix();
	glTranslated(0.4, 0, 0.4);
	t.drawObj(0.6, 0.02, 0.02, 0.3);
	glPopMatrix();
	glFlush();

	// 画出网格线
	glColor3f(1.0f, 0.0f, 0.0f); //网格线红色
	for (int x = -100; x < 100; x++) {
		glBegin(GL_LINES);
		glVertex3d(x, 0, 100);
		glVertex3d(x, 0, -100);
		glEnd();
		glFlush();
	}
	for (int x = -100; x < 100; x++) {
		glBegin(GL_LINES);
		glVertex3d(100, 0, x);
		glVertex3d(-100, 0, x);
		glEnd();
		glFlush();
	}
	glutSwapBuffers();//双缓冲技术

}

//光源固定在场景中
void displayStill() {
	//画图
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	l.setPosition(2.0, 3.0, 1.0, 1.0);
	glPushMatrix();
	glTranslated(2.0, 3.0, 1.0);
	glutSolidSphere(0.1, 40, 50);
	glPopMatrix();


	//桌子
	glPushMatrix();
	glTranslated(0.4, 0, 0.4);
	t.drawObj(0.6, 0.02, 0.02, 0.3);
	glPopMatrix();
	glFlush();

	// 画出网格线
	glColor3f(1.0f, 0.0f, 0.0f); //网格线红色
	for (int x = -100; x < 100; x++) {
		glBegin(GL_LINES);
		glVertex3d(x, 0, 100);
		glVertex3d(x, 0, -100);
		glEnd();
		glFlush();
	}
	for (int x = -100; x < 100; x++) {
		glBegin(GL_LINES);
		glVertex3d(100, 0, x);
		glVertex3d(-100, 0, x);
		glEnd();
		glFlush();
	}
	glutSwapBuffers();//双缓冲技术

}

//键盘控制
void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
		//光源控制：
	case 'O': glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); break;//开灯
	case 'O' - 64:glDisable(GL_LIGHTING); break;//关灯
	//光源随着相机一起移动
	case 'M':l.isMove = true; l.setPosition(cam.eye.x, cam.eye.y, cam.eye.z, 1.0); glutDisplayFunc(displayMove); break;
		//光源固定在场景中
	case 'S':l.isMove = false; glutDisplayFunc(displayStill); break;

		//相机平移
	case 'L':cam.slide(-1 * 0.5, 0, 0); cout << "左" << endl; break; // 相机向右
	case 'L' - 64:cam.slide(0.5, 0, 0);  cout << "右" << endl; break; // 相机向左
	case 'U':cam.slide(0, 0.5, 0); cout << "上" << endl; break; //相机向下
	case 'D':cam.slide(0, -1 * 0.5, 0);  cout << "下" << endl; break;//相机向上
	case 'F':cam.slide(0, 0, -1 * 0.5); cout << "前" << endl; break;//相机向前
	case 'F' - 64:cam.slide(0, 0, 0.5);  cout << "后" << endl; break;//相机向后
	//相机偏航
	case 'Y':cam.yaw(-1.0 * 1.0); cout << "左偏" << endl; break;
	case 'Y' - 64:cam.yaw(1.0); cout << "右偏" << endl; break;
		//相机俯仰
	case 'P':cam.pitch(1.0); cout << "上仰" << endl; break;
	case 'P' - 64:cam.pitch(-1.0 * 1.0); cout << "下俯" << endl; break;
		//相机旋转
	case 'R':cam.roll(1.0); cout << "右旋" << endl; break;
	case 'R' - 64:cam.roll(-1.0 * 1.0); cout << "左旋" << endl; break;

		//调整材质
	case 'J':t.setKey('J'); break;
	case 'C':t.setKey('C'); break;
	case 'V':t.setKey('V'); break;
	case 'G':t.setKey('G'); break;
	case 's':t.setKey('S'); break;

	default:
		break;
	}
	glutPostRedisplay();//重画
}



int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Logic");
	glutDisplayFunc(displayMove);
	glutKeyboardFunc(myKeyboard);

	//开灯
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//着色
	glShadeModel(GL_SMOOTH);
	//隐藏
	glEnable(GL_DEPTH_TEST);
	//阴影
	glEnable(GL_NORMALIZE);

	//设置相机
	cam.Camera_init(5.0, 0.5, 0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	cam.set();
	l.setPosition(5.0, 0.5, 0.0, 1.0f);
	l.setAmb(0.9, 0.9, 0.9, 1.0);
	l.setDiff(0.9, 0.9, 0.9, 1.0);
	l.setSpec(0.9, 0.9, 0.9, 1.0);
	cam.setShape(-1 * 30.0f, 64.0f / 48.0f, 0.5f, 50.0f);

	glClearColor(0.7f, 0.7f, 0.7f, 0.0f); //背景是灰色
	glViewport(0, 0, 640, 480);
	glutMainLoop();

}