#ifndef _IO_H_
#define _IO_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned char uchar;
typedef struct
{
	int x;
	int y;
}Point2d;

typedef struct
{
	float x;
	float y;
}Point2f;

typedef struct
{
	Point2f pt;
	float size;
	float angle;
	float response;
	int octave;
	int class_id;

}KeyPoint;
typedef struct
{
	int height;
	int width;
}Size;
typedef struct
{
	//unsigned short    bfType;
	unsigned long    bfSize;
	unsigned short    bfReserved1;
	unsigned short    bfReserved2;
	unsigned long    bfOffBits;
} ClBitMapFileHeader;

typedef struct
{
	unsigned long  biSize;
	long   biWidth;
	long   biHeight;
	unsigned short   biPlanes;
	unsigned short   biBitCount;
	unsigned long  biCompression;
	unsigned long  biSizeImage;
	long   biXPelsPerMeter;
	long   biYPelsPerMeter;
	unsigned long   biClrUsed;
	unsigned long   biClrImportant;
} ClBitMapInfoHeader;

typedef struct
{
	unsigned char rgbBlue; //����ɫ����ɫ����
	unsigned char rgbGreen; //����ɫ����ɫ����
	unsigned char rgbRed; //����ɫ�ĺ�ɫ����
	unsigned char rgbReserved; //����ֵ
} ClRgbQuad;

typedef struct
{
	int width;
	int height;
	int channels;
	unsigned char* imageData;
}ClImage;

typedef struct
{
	int width;
	int height;
	int channels;
	float* imageData;
}FtImage;
typedef struct
{
	int m1, m2, h;
	Size size;
	double sig_diff;
	int finalHeight;
	int threshold;
}INFO;
typedef FtImage Mat;

typedef FtImage Descriptor;

ClImage* clLoadImage(char* path);
bool clSaveImage(char* path, ClImage* bmpImg);



#endif