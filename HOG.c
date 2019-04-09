#include"HOG.h"
bool Sobel(ClImage *img, int *line_out, float *gray_scale, int finalHeight)
{
	int Height = img->height;
	int Width = img->width;
	int Sobel[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
	int Sum;
	int Index_min, Index_max;
	int min_value, max_value;
	gray_scale[0] = 255;
	gray_scale[1] = 0;
	for (int j = 1; j < Width - 1; j++)
	{
		Index_min = Height;
		min_value = 255 * 10;
		Index_max = 0;
		max_value = -255 * 10;
		for (int i = 1; i < Height - 1; i++)
		{
			Sum = 0;
			for (int s = -1; s <= 1; s++)
			{
				for (int t = -1; t <= 1; t++)
				{
					Sum += img->imageData[(i + s)*Width + j + t] * Sobel[(s + 1) * 3 + t + 1];
				}
			}
			if (Sum < min_value)
			{
				min_value = Sum;
				Index_min = i;
			}
			if (Sum > max_value)
			{
				max_value = Sum;
				Index_max = i;
			}
		}
		for (int k = Index_min; k <= Index_max; k++)
		{
			if (img->imageData[k*Width + j] < gray_scale[0])
			{
				gray_scale[0] = img->imageData[k*Width + j];
			}
			if (img->imageData[k*Width + j] > gray_scale[1])
			{
				gray_scale[1] = img->imageData[k*Width + j];
			}
		}
		if (Index_max <= Index_min)
		{
			return false;
		}
		line_out[j] = (Index_min + Index_max) / 2;
		//printf("%d\n", line_out[j]);
	}
	line_out[0] = line_out[1];
	line_out[Width - 1] = line_out[Width - 2];
	return true;
}
void Normalization(ClImage *img, float *gray_scale)
{
	int Height = img->height;
	int Width = img->width;
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			if (img->imageData[i*Width + j]<gray_scale[0] || img->imageData[i*Width + j]>gray_scale[1])
			{
				img->imageData[i*Width + j] = 0;
			}
			else
			{
				img->imageData[i*Width + j] = (uchar)(255 * ((img->imageData[i*Width + j] - gray_scale[0])) / (gray_scale[1] - gray_scale[0]));
			}
		}
	}
}
ClImage* Image_cut(ClImage *img, const int* Window)
{
	int Height = Window[1] - Window[0];
	int Width = Window[3] - Window[2];
	int width = img->width;
	ClImage* dst = (ClImage*)malloc(sizeof(ClImage));
	dst->imageData = (uchar*)malloc(sizeof(uchar)*Height*Width);
	dst->height = Height;
	dst->width = Width;
	dst->channels = 1;
	for (int i = Window[0]; i < Window[1]; i++)
	{
		for (int j = Window[2]; j < Window[3]; j++)
		{
			dst->imageData[(i - Window[0])*Width + j - Window[2]] = img->imageData[i*width + j];
		}
	}
	return dst;
}
ClImage* Max_curvature(float *img, int Height, int Width)
{
	ClImage* img_out = (ClImage*)malloc(sizeof(ClImage));
	img_out->imageData = (uchar*)malloc(sizeof(uchar)*Height*Width);
	img_out->height = Height;
	img_out->width = Width;
	img_out->channels = 1;
	double result;
	int i, j, m, n;
	float MC_kernel[9] = { -1.0 / 16,5.0 / 16,-1.0 / 16,5.0 / 16,-1.0,5.0 / 16,-1.0 / 16,5.0 / 16, -1.0 / 16 };
	float max = 0;
	for (i = 0; i < Height; i++)
	{
		for (j = 0; j < Width; j++)
		{
			if (i == 0 || i == Height - 1 || j == 0 || j == Width - 1)
			{
				img_out->imageData[i*Width + j] = 0;
			}
			else
			{
				result = 0;
				for (m = -1; m < 2; m++)
				{
					for (n = -1; n < 2; n++)
					{
						result += img[(i + m)*Width + j + n] * MC_kernel[(m + 1) * 3 + n + 1];
					}
				}
				if (result > max)
				{
					max = result;
				}
				if (result < 0)
				{
					result = 0;
				}
				img_out->imageData[i*Width + j] = (unsigned char)(256 * result);
			}
		}
	}
	return img_out;
}
ClImage* Mean_curvature(float *img, int Height, int Width)
{
	ClImage* img_out = (ClImage*)malloc(sizeof(ClImage));
	img_out->imageData = (uchar*)malloc(sizeof(uchar)*Height*Width);
	img_out->height = Height;
	img_out->width = Width;
	img_out->channels = 1;
	float fx, fy, fxx, fyy, fxy;
	double result;
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			if (i == 0 || i == Height - 1 || j == 0 || j == Width - 1)
			{
				img_out->imageData[i*Width + j] = 0;
			}
			else
			{
				fx = img[(i + 1)*Width + j] - img[i *Width + j];
				fy = img[i*Width + j + 1] - img[i *Width + j];
				fxx = img[(i + 1)*Width + j] + img[(i - 1)*Width + j] - 2 * img[i*Width + j];
				fyy = img[i *Width + j + 1] + img[i *Width + j - 1] - 2 * img[i*Width + j];
				fxy = img[(i + 1)*Width + j + 1] + img[i*Width + j] - img[(i + 1)*Width + j] - img[i*Width + j + 1];
				if (fx == 0 && fy == 0)
					result = 0;
				else
					result = 0.5*(fxx*fy*fy - 2 * fxy*fx*fy + fyy*fx*fx) / pow(fx*fx + fy*fy, 1.5);
				if (fabs(result) >= 1)
					result = 0;
				if (result<0)
					result = 0;
				img_out->imageData[i*Width + j] = (unsigned char)(256 * result);
			}
		}
	}
	return img_out;
}
void getdata(uchar* img_cut, ClImage* img, int Hstart, int Wstart, int Height, int Width, int Width1)
{
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			img_cut[i*Width + j] = img->imageData[(i + Hstart)*Width1 + j + Wstart];
		}
	}
}
float* Img_Rotation(float* img, int* line, int Height, int Width, int finalHeight)
{
	float angle = atan2((double)(line[Width - 1] - line[0]), (double)Width);
	float costheta = cos(angle);
	float sintheta = sin(angle);
	float* img_out = (float*)malloc((Width - 6) * 2 * finalHeight * sizeof(float));
	ClImage* img_out1 = (ClImage*)malloc(sizeof(ClImage));
	img_out1->imageData = (uchar*)malloc(sizeof(uchar)*(2 * Height)*(Width - 6));
	img_out1->height = 2 * finalHeight;
	img_out1->width = Width - 6;
	img_out1->channels = 1;
	float result;
	float X, Y;
	float p, q;
	for (int i = -finalHeight; i < finalHeight; i++)
	{
		for (int j = 3; j < Width - 3; j++)
		{
			X = line[0] + j*tan(angle) + i*costheta;
			Y = j + i*sintheta;
			if (X < 0 || X >= Height - 1 || Y < 0 || Y >= Width - 1)
			{
				result = 0;
			}
			else
			{
				p = X - (int)X;
				q = Y - (int)Y;
				result = (1 - p)*(1 - q)*img[(int)X*Width + (int)Y] + (1 - q)*p*img[((int)X + 1)*Width + (int)Y] + (1 - p) * q * img[(int)X*Width + (int)Y + 1] + p*q*img[((int)X + 1)*Width + (int)Y + 1];
			}
			img_out[(i + finalHeight)*(Width - 6) + j - 3] = result;
			img_out1->imageData[(i + finalHeight)*(Width - 6) + j - 3] = (uchar)result;
		}
	}
	//clSaveImage("rotate1.bmp", img_out1);
	free(img_out1->imageData);
	free(img_out1);
	return img_out;
}
ClImage* previous_cutter(char* filepath, const INFO* parameter, char* ROI_file, const int *Window)
{
	ClImage* img = clLoadImage(filepath);
	ClImage* img_cut = Image_cut(img, Window);
	free(img->imageData);
	free(img);
	float gray_scale[2];
	int * line = (int *)malloc(img_cut->width * sizeof(int));
	bool flag = Sobel(img_cut, line, gray_scale, parameter->finalHeight);
	if (!flag)
	{
		return NULL;
	}
	Normalization(img_cut, gray_scale);
	int Height = img_cut->height;
	int Width = img_cut->width;
	float* Blur_img = FloatImageGaussianBlur(img_cut, parameter->size, parameter->sig_diff, parameter->sig_diff);
	free(img_cut->imageData);
	free(img_cut);
	float* Blur_img_rotate = Img_Rotation(Blur_img, line, Height, Width, parameter->finalHeight);
	free(Blur_img);
	Width -= 6;
	ClImage *img_vein = Max_curvature(Blur_img_rotate, 2 * parameter->finalHeight, Width);
	free(Blur_img_rotate);
	//clSaveImage("vein1.bmp", img_vein);
	int threshold = parameter->threshold;
	for (int i = 0; i < img_vein->height; i++)
	{
		for (int j = 0; j < img_vein->width; j++)
		{
			if (img_vein->imageData[i*img_vein->width + j] < threshold)
			{
				img_vein->imageData[i*img_vein->width + j] = 0;
			}
		}
	}
	//clSaveImage(ROI_file, img_vein);
	free(line);
	return img_vein;
}
void hist(float* Mat, uchar* img, int Height, int Width, int h)
{
	//float *result = (float*)malloc(h * sizeof(float));
	for (int i = 0; i < h; i++)
	{
		Mat[i] = 0;
	}
	float fx, fy;
	float power, theta;
	for (int i = 1; i < Height - 1; i++)
	{
		for (int j = 1; j < Width - 1; j++)
		{
			fx = img[(i + 1)*Width + j] - img[(i - 1)*Width + j];
			fy = img[i*Width + j + 1] - img[i*Width + j - 1];
			power = sqrt(fx*fx + fy*fy);
			if (fx == 0 && fy == 0)
			{
				continue;
			}
			else
			{
				theta = atan2(fy, fx);
			}
			Mat[(int)((theta + pi)*h / (2 * pi))] += power;
		}
	}
	/*float min=INT_MAX, max=0;
	for (int i = 0; i < h; i++)
	{
	if (result[i] < min)
	{
	min = result[i];
	continue;
	}
	if (result[i] > max)
	{
	max = result[i];
	}
	}
	for (int i = 0; i < h; i++)
	{
	Mat[i] = (uchar)(255 * (result[i] - min) / (max - min));
	}*/
	//free(result);
}
uchar* HOG_hist(ClImage* img, int m1, int m2, int h)
{
	uchar* result = (uchar *)malloc((m1*m2*h) * sizeof(float));
	float* Mat = (float *)malloc((m1*m2*h) * sizeof(float));
	int Height = img->height / m1;
	int Width = img->width / m2;
	uchar* img_cut = (uchar*)malloc(Height*Width * sizeof(uchar));
	for (int i = 0; i < m1; i++)
	{
		for (int j = 0; j < m2; j++)
		{
			getdata(img_cut, img, i*Height, j*Width, Height, Width, img->width);
			hist(Mat + (i*m2 + j)*h, img_cut, Height, Width, h);
		}
	}
	float min = 300 * Height*Width;
	float max = 0;
	for (int i = 0; i < m1*m2; i++)
	{
		for (int j = 0; j < h; j++)
		{
			if (Mat[i*h + j] < min)
			{
				min = Mat[i*h + j];
			}
			if (Mat[i*h + j] > max)
			{
				max = Mat[i*h + j];
			}
		}
	}
	for (int i = 0; i < m1*m2; i++)
	{
		for (int j = 0; j < h; j++)
		{
			result[i*h + j] = (uchar)(255 * (Mat[i*h + j] - min) / (max - min));
		}
	}
	free(img_cut);
	free(Mat);
	return result;
}