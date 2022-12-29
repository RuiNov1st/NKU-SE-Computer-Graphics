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

//��
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

//����
class Vector3 {
public:
	GLdouble x;
	GLdouble y;
	GLdouble z;

	Vector3();

	Vector3(GLdouble xx, GLdouble yy, GLdouble zz);
	//��ֵ
	void set(GLdouble xx, GLdouble yy, GLdouble zz);

	//������ˣ�
	double dot(Vector3 v);
	//������ˣ�
	Vector3* cross(Vector3 v);
	//������һ����
	void normalize();

};

//��Դ����
class Light {
public:
	//��Դ����
	int lightname;
	GLfloat lightPosition[4];
	GLfloat amb[4];//����
	GLfloat diff[4];//������
	GLfloat spec[4];//���淴��
	GLfloat dir[4];//����
	bool isMove;

	Light();
	Light(int name);
	//���ù�Դλ��
	void setPosition(float x, float y, float z, float dis);
	//���ù�Դ
	void setAmb(float r, float g, float b, float alpha);
	void setDiff(float r, float g, float b, float alpha);
	void setSpec(float r, float g, float b, float alpha);
	//����Ϊ̽�յ�
	void setTorch(float cutoff, float exponent, float x, float y, float z);
	//ȡ��̽�յ�ģʽ
	void setNormal();

};

//�����
class Camera
{
public:
	//�������ã�
	//eye, look ,up������
	Point3 eye, look, up;
	// u,v,n��������
	Vector3 u, v, n;
	//�Ӿ������
	double ViewAngle, aspect, nearDist, farDist;

	//���캯��
	Camera();

	void Camera_init(double eyex, double eyey, double eyez, double lookx, double looky, double lookz, double upx, double upy, double upz);
		

	//ģ���ӵ��������
	void setModelViewMatrix();

	//����u.v.n
	void set();

	//�����Ӿ���
	void setShape(double viewAngle, double aspect, double nearDist, double farDist);

	//����ƶ�
	void slide(float delU, float delV, float delN);

	//�����ת����n����ת
	void roll(float angle);
	//���ƫ������v����ת
	void yaw(float angle);
	//�����������u����ת
	void pitch(float angle);
};

//����
class Material {
public:
	GLfloat mat_ambient[4];//������
	GLfloat mat_diffuse[4]; //��ɢ��
	GLfloat mat_specular[4];//���淴��
	GLfloat mat_shininess[1];//ָ��

	//���ò���
	void setMeterial(unsigned char key);
};

//�������
class GemObj {
public:
	//�������
	Material mat;

	//��ͼ����
	void drawObj();

	//�����������
	void TellMaterial(unsigned char key);
};

class Table :public GemObj {
public:
	unsigned char key = 'S';

	void setKey(unsigned char m_key);

	//����
	void tableLeg(double thick, double len);

	//������
	void drawObj(double topWid, double topThick, double legThick, double legLen);

};


