#pragma once
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

//点
class Point3 {
public:
	GLdouble x;
	GLdouble y;
	GLdouble z;

	Point3();
	Point3(GLdouble xx, GLdouble yy, GLdouble zz);
	void set(Point3 p);
	void set(GLdouble xx, GLdouble yy, GLdouble zz);
};

//向量
class Vector3 {
public:
	GLdouble x;
	GLdouble y;
	GLdouble z;

	Vector3();

	Vector3(GLdouble xx, GLdouble yy, GLdouble zz);
	//赋值
	void set(GLdouble xx, GLdouble yy, GLdouble zz);

	//向量点乘：
	double dot(Vector3 v);
	//向量叉乘：
	Vector3* cross(Vector3 v);
	//向量归一化：
	void normalize();

};

//光源设置
class Light {
public:
	//光源设置
	int lightname;
	GLfloat lightPosition[4];
	GLfloat amb[4];//环境
	GLfloat diff[4];//漫反射
	GLfloat spec[4];//镜面反射
	GLfloat dir[4];//朝向
	bool isMove;

	Light();
	Light(int name);
	//设置光源位置
	void setPosition(float x, float y, float z, float dis);
	//设置光源
	void setAmb(float r, float g, float b, float alpha);
	void setDiff(float r, float g, float b, float alpha);
	void setSpec(float r, float g, float b, float alpha);
	//设置为探照灯
	void setTorch(float cutoff, float exponent, float x, float y, float z);
	//取消探照灯模式
	void setNormal();

};

//相机类
class Camera
{
public:
	//参数设置：
	//eye, look ,up三个点
	Point3 eye, look, up;
	// u,v,n三个向量
	Vector3 u, v, n;
	//视景体参数
	double ViewAngle, aspect, nearDist, farDist;

	//构造函数
	Camera();

	void Camera_init(double eyex, double eyey, double eyez, double lookx, double looky, double lookz, double upx, double upy, double upz);
		

	//模型视点矩阵设置
	void setModelViewMatrix();

	//计算u.v.n
	void set();

	//设置视景体
	void setShape(double viewAngle, double aspect, double nearDist, double farDist);

	//相机移动
	void slide(float delU, float delV, float delN);

	//相机旋转：绕n轴旋转
	void roll(float angle);
	//相机偏航：绕v轴旋转
	void yaw(float angle);
	//相机俯仰：绕u轴旋转
	void pitch(float angle);
};

//材质
class Material {
public:
	GLfloat mat_ambient[4];//环境光
	GLfloat mat_diffuse[4]; //漫散射
	GLfloat mat_specular[4];//镜面反射
	GLfloat mat_shininess[1];//指数

	//设置材质
	void setMeterial(unsigned char key);
};

//物体对象
class GemObj {
public:
	//物体材质
	Material mat;

	//画图程序
	void drawObj();

	//设置物体材质
	void TellMaterial(unsigned char key);
};

class Table :public GemObj {
public:
	unsigned char key = 'S';

	void setKey(unsigned char m_key);

	//桌腿
	void tableLeg(double thick, double len);

	//画桌子
	void drawObj(double topWid, double topThick, double legThick, double legLen);

};


