/*
将纹理贴到立方体上
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



//RGB类：存储RGB颜色三元组（256）
class RGB {
public:
	unsigned char r, g, b;
};

//位图类
class RGBpixmap {
public:
	int nrows, ncols;//位图的大小
	int channel;
	RGB* pixel;//RGB数值
	unsigned char* textureData;


	//读取图片生成纹理
	int readImgFile(const char* filename, GLuint textureName) {
		//载入图片
		textureData = stbi_load(filename, &ncols, &nrows, &channel,0);
		
		//更改尺寸
		int ow = 64;
		int oh = 64;
		auto* odata = (unsigned char*)malloc(ow * oh * channel);
		stbir_resize(textureData, ncols, nrows, 0, odata, ow, oh, 0, STBIR_TYPE_UINT8, channel, STBIR_ALPHA_CHANNEL_NONE, 0,
			STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
			STBIR_FILTER_BOX, STBIR_FILTER_BOX,
			STBIR_COLORSPACE_SRGB, nullptr
		);

		//设置纹理
		glBindTexture(GL_TEXTURE_2D, textureName);
		//放缩时的纹理设置
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ow, oh, 0, GL_RGB, GL_UNSIGNED_BYTE, odata);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ncols, nrows, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

		stbi_image_free(textureData);
		stbi_image_free(odata);
		return 0;
	}


	//生成棋盘纹理数组
	void makeCheckerboard() {
		//棋盘大小：64*64
		nrows = 64;
		ncols = 64;

		//位图数据生成
		pixel = new RGB[nrows * ncols];

		int count = 0;
		for (int i = 0; i < nrows; i++) {
			for (int j = 0; j < ncols; j++) {
				//棋盘纹理：每8个像素在0，255之间跳变一次
				int c=(((i & 0x8) == 0) ^((j & 0x8) == 0)) *255;
				pixel[count].r = c;
				pixel[count].g = c;
				pixel[count].b = c;
				count += 1;
			}
		}

	}

	//生成无规则纹理
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

	//创建纹理
	void setTexture(GLuint textureName) {
		//绑定纹理名字
		glBindTexture(GL_TEXTURE_2D, textureName);
		//放缩时的纹理设置
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//从纹理数组中生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ncols, nrows, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	}


	~RGBpixmap() {
		if (pixel) {
			delete pixel;
		}
	}

};


// 全局变量定义
RGBpixmap pix[6]; //6个面的纹理
float xAngle = 0.5, yAngle = 0.5; //旋转的角度

//初始化
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
	glTranslated(0.0, -0.2, -4);//相机位置


}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glPushMatrix();
	//正方体旋转的角度
	glRotated(xAngle, 1.0, 0.0, 0.0);
	glRotated(yAngle, 0.0, 1.0, 0.0);

	//上面纹理
	glBindTexture(GL_TEXTURE_2D, 2000);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();

	//右边纹理
	glBindTexture(GL_TEXTURE_2D, 2001);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0f, 1.0f, 1.0f);
	glEnd();

	//左边纹理
	glBindTexture(GL_TEXTURE_2D, 2002);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f, -1.0f, -1.0f);
	glEnd();

	//下边纹理
	glBindTexture(GL_TEXTURE_2D, 2003);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();

	//前边纹理
	glBindTexture(GL_TEXTURE_2D, 2004);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f, 1.0f, 1.0f);
	glEnd();

	//后边纹理
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

// 正方体的旋转
void spinner() {
	float xSpeed = 0.2, ySpeed = 0.2;
	xAngle += xSpeed;
	yAngle += ySpeed;
	myDisplay();
}

void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	//立方体是棋盘样式
	case 'C':{
		for (int i = 0; i < 6; i++) {
				pix[i].makeCheckerboard();
				pix[i].setTexture(2000 + i);
		}
		break;
		}
	//立方体是南开
	case 'N': {
		for (int i = 0; i < 6; i++) {
			string tempName = "../img/nku" + to_string(i) + ".jpg";
			pix[i].readImgFile(tempName.c_str(), 2000 + i);
		}
		break;
	}
	//立方体是混合
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

