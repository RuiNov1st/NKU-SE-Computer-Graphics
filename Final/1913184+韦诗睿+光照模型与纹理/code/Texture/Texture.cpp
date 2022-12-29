/*
������������������
*/
#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#include <windows.h>
#include<math.h>
#include<gl/GL.h>
#include<gl/glut.h>
#include<iostream>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include <ctime>
#include <fstream>
#include<cmath>
#include<string>

using namespace std;



//RGB�ࣺ�洢RGB��ɫ��Ԫ�飨256��
class RGB {
public:
	unsigned char r, g, b;
};

//λͼ��
class RGBpixmap {
public:
	int nrows, ncols;//λͼ�Ĵ�С
	int channel;
	RGB* pixel;//RGB��ֵ
	unsigned char* textureData;


	//��ȡͼƬ��������
	int readImgFile(const char* filename, GLuint textureName) {
		//����ͼƬ
		textureData = stbi_load(filename, &ncols, &nrows, &channel,0);
		
		//���ĳߴ�
		int ow = 64;
		int oh = 64;
		auto* odata = (unsigned char*)malloc(ow * oh * channel);
		stbir_resize(textureData, ncols, nrows, 0, odata, ow, oh, 0, STBIR_TYPE_UINT8, channel, STBIR_ALPHA_CHANNEL_NONE, 0,
			STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
			STBIR_FILTER_BOX, STBIR_FILTER_BOX,
			STBIR_COLORSPACE_SRGB, nullptr
		);

		//��������
		glBindTexture(GL_TEXTURE_2D, textureName);
		//����ʱ����������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ow, oh, 0, GL_RGB, GL_UNSIGNED_BYTE, odata);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ncols, nrows, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

		stbi_image_free(textureData);
		stbi_image_free(odata);
		return 0;
	}


	//����������������
	void makeCheckerboard() {
		//���̴�С��64*64
		nrows = 64;
		ncols = 64;

		//λͼ��������
		pixel = new RGB[nrows * ncols];

		int count = 0;
		for (int i = 0; i < nrows; i++) {
			for (int j = 0; j < ncols; j++) {
				//��������ÿ8��������0��255֮������һ��
				int c=(((i & 0x8) == 0) ^((j & 0x8) == 0)) *255;
				pixel[count].r = c;
				pixel[count].g = c;
				pixel[count].b = c;
				count += 1;
			}
		}

	}

	//�����޹�������
	void makeRandom() {
		nrows = 64;
		ncols = 64;
		pixel = new RGB[nrows * ncols];
		
		int count = 0;
		for (int i = 0; i < nrows; i++) {
			for (int j = 0; j < ncols; j++) {
				pixel[count].r = (i * j) % 255;
				pixel[count].g = (i * i) % 255;
				pixel[count].b = (j * j) % 255;;
				count++;
			}
		}

	}

	//��������
	void setTexture(GLuint textureName) {
		//����������
		glBindTexture(GL_TEXTURE_2D, textureName);
		//����ʱ����������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//��������������������
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ncols, nrows, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	}


	~RGBpixmap() {
		if (pixel) {
			delete pixel;
		}
	}

};


// ȫ�ֱ�������
RGBpixmap pix[6]; //6���������
float xAngle = 0.5, yAngle = 0.5; //��ת�ĽǶ�

//��ʼ��
void myInit() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glViewport(0, 0, 640, 480);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 640.0 / 480.0, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0.0, -0.2, -4);//���λ��


}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glPushMatrix();
	//��������ת�ĽǶ�
	glRotated(xAngle, 1.0, 0.0, 0.0);
	glRotated(yAngle, 0.0, 1.0, 0.0);

	//��������
	glBindTexture(GL_TEXTURE_2D, 2000);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();

	//�ұ�����
	glBindTexture(GL_TEXTURE_2D, 2001);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0f, 1.0f, 1.0f);
	glEnd();

	//�������
	glBindTexture(GL_TEXTURE_2D, 2002);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f, -1.0f, -1.0f);
	glEnd();

	//�±�����
	glBindTexture(GL_TEXTURE_2D, 2003);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();

	//ǰ������
	glBindTexture(GL_TEXTURE_2D, 2004);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f, 1.0f, 1.0f);
	glEnd();

	//�������
	glBindTexture(GL_TEXTURE_2D, 2005);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f, -1.0f, -1.0f);
	glEnd();

	glFlush();
	glPopMatrix();
	glutSwapBuffers();
	glutPostRedisplay();
	
}

// ���������ת
void spinner() {
	float xSpeed = 0.2, ySpeed = 0.2;
	xAngle += xSpeed;
	yAngle += ySpeed;
	myDisplay();
}

void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	//��������������ʽ
	case 'C':{
		for (int i = 0; i < 6; i++) {
				pix[i].makeCheckerboard();
				pix[i].setTexture(2000 + i);
		}
		break;
		}
	//���������Ͽ�
	case 'N': {
		for (int i = 0; i < 6; i++) {
			string tempName = "../img/nku" + to_string(i) + ".jpg";
			pix[i].readImgFile(tempName.c_str(), 2000 + i);
		}
		break;
	}
	//�������ǻ��
	case 'R': {
		for (int i = 0; i < 6; i++) {
			if (i < 2) {
				string tempName = "../img/nku" + to_string(i) + ".jpg";
				pix[i].readImgFile(tempName.c_str(), 2000 + i);
			}
			else {
				if (i >= 2 && i < 4) {
					pix[i].makeCheckerboard();
					pix[i].setTexture(2000 + i);
				}
				else {
					pix[i].makeRandom();
					pix[i].setTexture(2000 + i);
				}
			}
		}
		break;
	}
	default:
		break;
	}
}


int main(int argc,char**argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 800);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Cube Texture");
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	myInit();
	glutIdleFunc(spinner);
	glutMainLoop();
}

