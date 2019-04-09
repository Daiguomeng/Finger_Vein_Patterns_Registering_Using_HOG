#pragma once
#include "io.h"
#include "cvtools.h"
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#define pi 3.141592653
char* string_joint(char* str1, char* str2);
bool Sobel(ClImage *img, int *line_out, float *gray_scale, int finalHeight);
void Normalization(ClImage *img, float *gray_scale);
ClImage* Image_cut(ClImage *img, const int* Window);
ClImage* Max_curvature(float *img, int Height, int Width);
ClImage* Mean_curvature(float *img, int Height, int Width);
//ClImage* Roi_extration(int* line, ClImage *img, int Width, int Height);
float* Img_Rotation(float* img, int* line, int Height, int Width, int finalHeight);
void getdata(uchar* img_cut, ClImage* img, int Hstart, int Wstart, int Height, int Width, int Width1);
ClImage* previous_cutter(char* filepath, const INFO* parameter, char* ROI_file, const int *Window);
void hist(float* Mat, uchar* img, int Height, int Width, int h);
uchar* HOG_hist(ClImage* img, int m1, int m2, int h);