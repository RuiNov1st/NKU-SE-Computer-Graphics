/*
Flying�� a Camera through a scene-1/2
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

using namespace std;

//����
class Point3 {
public:
	GLdouble x;
	GLdouble y;
	GLdouble z;

	Point3() {

	}

	Point3(GLdouble xx, GLdouble yy, GLdouble zz) {
		x = xx;
		y = yy;
		z = zz;
	}
	void set(Point3 p) {
		x = p.x;
		y = p.y;
		z = p.z;
	}
	void set(GLdouble xx, GLdouble yy, GLdouble zz) {
		x = xx;
		y = yy;
		z = zz;
	}

};

//���������ݽṹ
class Vector3 {
public:
	GLdouble x;
	GLdouble y;
	GLdouble z;

	Vector3() {

	}

	Vector3(GLdouble xx, GLdouble yy, GLdouble zz) {
		x = xx;
		y = yy;
		z = zz;
	}
	//��ֵ
	void set(GLdouble xx, GLdouble yy, GLdouble zz) {
		x = xx;
		y = yy;
		z = zz;

	}

	//������ˣ�
	double dot(Vector3 v) {
		return x * v.x + y * v.y + z * v.z;
	}
	//������ˣ�
	Vector3* cross(Vector3 v) {
		Vector3* t = new Vector3();
		t->x = y * v.z - z * v.y;
		t->y = x * v.z - z * v.x;
		t->z = x * v.y - y * v.x;
		
		return t;
	}
	//������һ����
	void normalize() {
		double n = pow(x * x + y * y + z * z,0.5);
		x = x / n;
		y = y / n;
		z = z / n;
	}

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

	//�������ã�

	//���캯��
	Camera() {

	}

	void Camera_init(double eyex, double eyey, double eyez, double lookx, double looky, double lookz, double upx, double upy, double upz) {
		eye.set(eyex, eyey, eyez);
		look.set(lookx, looky, lookz);
		up.set(upx, upy, upz);

		cout << "eye��" << eye.x << ' ' << eye.y << ' ' << eye.z << endl;
		cout << "look��" << look.x << ' ' << look.y << ' ' << look.z << endl;
		cout << "up��" << up.x << ' ' << up.y << ' ' << up.z << endl;

	}

	//ģ���ӵ��������
	void setModelViewMatrix() {
		//���ݲ�������Viewing Matrix
		float v_m[16]; //��������
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
		//���ұ�һ���ǣ� -M.t .* eye 
		Vector3 eyev = Vector3(eye.x, eye.y, eye.z);
		v_m[12] = -1 * eyev.dot(u);
		v_m[13] = -1 * eyev.dot(v);
		v_m[14] = -1 * eyev.dot(n);
		v_m[15] = 1;

		cout << "ModelView Matrix:" << ' ';
		for (int i = 0; i < 16; i++) {
			cout << v_m[i] << ' ';
		}
		cout << endl;

		//����modelview matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(v_m);
		
	}

	//����u.v.n
	void set() {
		//n
		n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);

		//u
		Vector3 up_vec = Vector3(up.x,up.y,up.z);
		Vector3* t = up_vec.cross(n);
		u.set(t->x, t->y, t->z);
		
		//v
		Vector3* tt= n.cross(u);
		v.set(tt->x, tt->y, tt->z);
		
		delete t, tt;

		cout << "n:" << n.x << " " << n.y << " " << n.z << endl;
		cout << "u:" << u.x << " " << u.y << " " << u.z << endl; 
		cout << "v:" << v.x << " " << v.y << " " << v.z << endl;

		//��һ��
		n.normalize();
		u.normalize();
		v.normalize();
		cout << "n:" << n.x << " " << n.y << " " << n.z << endl;
		cout << "u:" << u.x << " " << u.y << " " << u.z << endl;
		cout << "v:" << v.x << " " << v.y << " " << v.z << endl;
		//����ģ���ӵ����
		setModelViewMatrix();
	}

	//�����Ӿ���
	void setShape(double viewAngle, double aspect, double nearDist, double farDist) {
		ViewAngle = viewAngle;
		aspect = aspect;
		nearDist= nearDist;
		farDist = farDist;

		//����ͶӰ����
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(viewAngle, aspect, nearDist, farDist);
		cout << "�����Ӿ��壺" << viewAngle <<' ' << aspect << ' ' << nearDist << ' ' << farDist << endl;

	}

	//����ƶ�
	void slide(float delU, float delV, float delN) {
		//�ı��ӵ�
		eye.x += delU * u.x + delV * v.x + delN * n.x;
		eye.y += delU * u.y + delV * v.y + delN * n.y; 
		eye.z += delU * u.z + delV * v.z + delN * n.z;
		//ͬ���ı�look
		look.x += delU * u.x + delV * v.x + delN * n.x;
		look.y += delU * u.y + delV * v.y + delN * n.y;
		look.z += delU * u.z + delV * v.z + delN * n.z;
		//����ģ���ӵ����
		setModelViewMatrix();
	}

	//�����ת����n����ת
	void roll(float angle) {
		float cosv = cos(3.14159265 / 180 * angle);
		float sinv = sin(3.14159265 / 180 * angle);

		//����u��ֵ��֮��Ҫ�޸�
		Vector3 t;
		t.set(u.x, u.y, u.z);

		u.set(cosv * t.x + sinv * v.x, cosv * t.y + sinv * v.y, cosv * t.z + sinv * v.z);
		v.set(-1.0 * sinv * t.x + cosv * v.x, -1.0 * sinv * t.y + cosv * v.y, -1.0 * sinv * t.z + cosv * v.z);

		//�����ӵ�ģ�;���
		setModelViewMatrix();
	}
	//���ƫ������v����ת
	void yaw(float angle) {
		float cosv = cos(3.14159265 / 180 * angle);
		float sinv = sin(3.14159265 / 180 * angle);
		//����u��ֵ��֮��Ҫ�޸�
		Vector3 t;
		t.set(n.x, n.y, n.z);

		n.set(cosv * t.x + sinv * u.x, cosv * t.y + sinv * u.y, cosv * t.z + sinv * u.z);
		u.set(-1.0 * sinv * t.x + cosv * u.x, -1.0 * sinv * t.y + cosv * u.y, -1.0 * sinv * t.z + cosv * u.z);

		//�����ӵ�ģ�;���
		setModelViewMatrix();
	}
	//�����������u����ת
	void pitch(float angle) {
		float cosv = cos(3.14159265 / 180 * angle);
		float sinv = sin(3.14159265 / 180 * angle);
		//����u��ֵ��֮��Ҫ�޸�
		Vector3 t;
		t.set(v.x, v.y, v.z);
			
		v.set(cosv * t.x + sinv * n.x, cosv * t.y + sinv * n.y, cosv * t.z + sinv * n.z);
		n.set(-1.0 * sinv * t.x + cosv * n.x, -1.0 * sinv * t.y + cosv * n.y, -1.0 * sinv * t.z + cosv * n.z);
		//�����ӵ�ģ�;���
		setModelViewMatrix();
	}
};

//���ȫ�ֶ���
Camera cam;

//������
class Item {
public:
	float delU, delV, delN; //ƽ�ƾ���
	float angleU, angleV, angleN;//��ת�Ƕ�

	Item() {

	}

	Item(float x,float y, float z) {
		delU = x;
		delV = y;
		delN = z;
	}

	//ƽ��
	void slide(float U, float V, float N) {
		delU += U;
		delV += V;
		delN += N;
	}
	//��N����ת
	void roll(float anglee) {
		angleN += anglee;
	}
	//��U����ת
	void pitch(float anglee) {
		angleU += anglee;

	}
	//��V����ת
	void yaw(float anglee) {
		angleV += anglee;
	}
	
};

//����ȫ�ֶ���
Item item1;
Item item2 = Item(0,1,2);

//���̿���
void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	//���ƽ��
	case 'L':cam.slide(-1 * 0.5, 0, 0); cout << "��" << endl; break; // ����
	case 'L'-64 :cam.slide(0.5, 0, 0);  cout << "��" << endl; break; // ����
	case 'U':cam.slide(0, 0.5, 0); cout << "��" << endl; break; //����
	case 'D':cam.slide(0, -1 * 0.5, 0);  cout << "��" << endl; break;//����
	case 'F':cam.slide(0, 0, -1*0.5); cout << "ǰ" << endl; break;//��ǰ
	case 'F'-64:cam.slide(0, 0, 0.5);  cout << "��" << endl; break;//���
	//���ƫ��
	case 'Y':cam.yaw(-1.0*1.0); cout << "��ƫ" << endl; break;
	case 'Y'-64:cam.yaw(1.0); cout << "��ƫ" << endl; break;
	//�������
	case 'P':cam.pitch(1.0); cout << "����" << endl; break;
	case 'P'-64:cam.pitch(-1.0*1.0); cout << "�¸�" << endl; break;
	//�����ת
	case 'R':cam.roll(1.0); cout << "����" << endl; break;
	case 'R'-64:cam.roll(-1.0*1.0); cout << "����" << endl; break;

	//�������
	//����ƽ��
	case 'W':item1.slide( 0.5, 0, 0); cout << "ǰ" << endl; break;
	case 'W'-64:item1.slide(-1 * 0.5, 0, 0); cout << "��" << endl; break;
	case 'I' :item2.slide(0.5, 0, 0); cout << "ǰ" << endl; break;
	case 'I' - 64:item2.slide(-1 * 0.5, 0, 0); cout << "��" << endl; break;

	case 'A':item1.slide(0, 0, - 1 * 0.5); cout << "��" << endl; break;
	case 'A'-64:item1.slide(0, 0,0.5); cout << "��" << endl; break;
	case 'H':item2.slide(0, 0, -1 * 0.5); cout << "��" << endl; break;
	case 'H' - 64:item2.slide(0, 0, 0.5); cout << "��" << endl; break;

	case 'S':item1.slide(0, -1 * 0.5, 0); cout << "��" << endl; break;
	case 'S'-64:item1.slide(0, 0.5, 0); cout << "��" << endl; break;
	case 'J':item2.slide(0, -1 * 0.5, 0); cout << "��" << endl; break;
	case 'J' - 64:item2.slide(0, 0.5, 0); cout << "��" << endl; break;

	//������ת
	case 'Z':item1.roll(30); cout << "����" << endl; break;
	case 'Z'-64:item1.roll(-1*30); cout << "����" << endl; break;
	case 'B':item2.roll(30); cout << "����" << endl; break;
	case 'B' - 64:item2.roll(-1 * 30); cout << "����" << endl; break;

	case 'X':item1.pitch(30); cout << "����" << endl; break;
	case 'X'-64:item1.pitch(-1*30); cout << "����" << endl; break;
	case 'N':item2.pitch(30); cout << "����" << endl; break;
	case 'N' - 64:item2.pitch(-1 * 30); cout << "����" << endl; break;

	case 'C':item1.yaw(30); cout << "��ƫ" << endl; break;
	case 'C'-64:item1.yaw(-1*30); cout << "��ƫ" << endl; break;
	case 'M':item2.yaw(30); cout << "��ƫ" << endl; break;
	case 'M' - 64:item2.yaw(-1 * 30); cout << "��ƫ" << endl; break;

	default:
		break;
	}
	glutPostRedisplay();//�ػ�
}

//���Ʋ��
void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	//����1����ɫ
	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glTranslatef(item1.delU, item1.delV, item1.delN);
	glRotated(item1.angleU,1,0,0);
	glRotated(item1.angleV, 0, 1, 0);
	glRotated(item1.angleN, 0, 0, 1);
	glutWireTeapot(0.5);
	glPopMatrix();

	//����2����ɫ
	glColor3f(0.0f, 0.0f, 1.0f);
	glPushMatrix();
	glTranslatef(item2.delU, item2.delV, item2.delN);
	glRotated(item2.angleU, 1, 0, 0);
	glRotated(item2.angleV, 0, 1, 0);
	glRotated(item2.angleN, 0, 0, 1);
	glutWireTeapot(0.5);
	glPopMatrix();
	glFlush();

	//����������
	glColor3f(1.0f, 0.0f, 0.0f); //�����ߺ�ɫ
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



	glutSwapBuffers();//˫���弼��
	cout << "TeaPort" << endl;
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//ʹ��˫���弼��
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("lab10_11");

	//�������
	cam.Camera_init(8.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	cam.set();
	cam.setShape(-1*30.0f, 64.0f / 48.0f, 0.5f, 50.0f);

	glutKeyboardFunc(myKeyboard);
	glutDisplayFunc(myDisplay);
	glClearColor(1, 1, 1, 1);
	
	glViewport(0, 0, 640, 480);

	
	
	glutMainLoop();


}

