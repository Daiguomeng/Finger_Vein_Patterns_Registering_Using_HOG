#include "main.h"
#include"register.h"
#define pi 3.141592653
ClImage* Roi_extration(int* line, ClImage *img, int Width, int Height)
{
	//int Height = 45;
	ClImage* img_out = (ClImage*)malloc(sizeof(ClImage));
	img_out->imageData = (uchar*)malloc(sizeof(uchar)*(2 * Height)*(Width - 6));
	img_out->height = 2 * Height;
	img_out->width = Width - 6;
	img_out->channels = 1;
	int center;
	for (int i = 0; i < Width - 6; i++)
	{
		center = line[i + 3];
		for (int j = 0; j < 2 * Height; j++)
		{
			img_out->imageData[j*(Width - 6) + i] = img->imageData[(center + j - Height)*Width + i + 3];
		}
	}
	return img_out;
}
float getCorrelate(uchar * M, uchar *S, int Height, int Width)
{
	//以上是严格算法，实用算法如下：
	float corr = 0.0f;
	float covMaSl = 0.0f; //相关系数
	float sum_mas = 0.0f;
	float sum_masSqu = 0.0f;
	float sum_sla = 0.0f;
	float sum_slaSqu = 0.0f;

	//为了提速，不使用数组访问，而使用指针访问
	uchar* pMas = &(M[0]);
	uchar* pSla = &(S[0]);
	for (int h = 0; h<Height; h++)
	{
		for (int w = 0; w<Width; w++)
		{
			covMaSl += (*pMas)*(*pSla);
			sum_mas += *pMas;
			sum_masSqu += (*pMas)*(*pMas);
			sum_sla += *pSla;
			sum_slaSqu += (*pSla)*(*pSla);
			pMas++;
			pSla++;

		}
	}
	float base = Height*Width;
	corr = (covMaSl - sum_mas*sum_sla / base) / (sqrt((sum_masSqu - sum_mas*sum_mas / base)*(sum_slaSqu - sum_sla*sum_sla / base)));
	return corr;
}
float norm(float * M, float *S, int Height, int Width)
{
	//以上是严格算法，实用算法如下：
	float corr = 0.0f;

	//为了提速，不使用数组访问，而使用指针访问
	float* pMas = &(M[0]);
	float* pSla = &(S[0]);
	for (int h = 0; h<Height; h++)
	{
		for (int w = 0; w<Width; w++)
		{
			corr += (*pMas - *pSla)*(*pMas - *pSla);
			pMas++;
			pSla++;

		}
	}
	return sqrt(corr / (Height*Width));
}
ClImage* Zoom_out(ClImage* img, int mul_num)
{
	ClImage* img_out = (ClImage*)malloc(sizeof(ClImage));
	img_out->height = img->height / mul_num;
	img_out->width = img->width / mul_num;
	img_out->channels = 1;
	img_out->imageData = (uchar*)malloc(sizeof(uchar)*img_out->height*img_out->width);
	int sum;
	for (int i = 0; i < img_out->height; i++)
	{
		for (int j = 0; j < img_out->width; j++)
		{
			sum = 0;
			for (int s = 0; s < mul_num; s++)
			{
				for (int t = 0; t < mul_num; t++)
				{
					sum += img->imageData[(i*mul_num + s)*img->width + j*mul_num + t];
				}
			}
			if (sum >= 3)
			{
				img_out->imageData[i*img_out->width + j] = 255;
			}
			else
			{
				img_out->imageData[i*img_out->width + j] = 0;
			}
		}
	}
	free(img->imageData);
	free(img);
	return img_out;
}
int detect(char* finger_file, char* input_file,INFO *parameter,int *Window,float max_corhence)
{
	int m1 = parameter->m1;
	int m2 = parameter->m2;
	int h = parameter->h;
	ClImage* img1 = previous_cutter(input_file, parameter, "roi.bmp", Window);
	if (img1 == NULL)
	{
		return 0;
	}
	uchar *mat1 = HOG_hist(img1, m1, m2, h);
	FILE *fp = fopen(finger_file, "rb");
	uchar *mat2 = (uchar*)malloc(m1*m2*h * sizeof(uchar));
	int match_index = 0;
	float corhence;
	bool flag = false;
	while (!feof(fp))
	{
		match_index++;
		fread(mat2, sizeof(uchar), m1*m2*h * sizeof(uchar), fp);
		corhence = getCorrelate(mat1, mat2, m1*m2, h);
		if (corhence > max_corhence)
		{
			flag = true;
			break;
		}
	}
	fclose(fp);
	if (flag)
	{
		return match_index;
	}
	else
	{
		return 0;
	}
}
int main(int argc, char* argv[])
{
	char* in_file = "C:\\Users\\daiguomeng\\Desktop\\Finger\\Finger Vein Database\\Finger Vein Database\\Finger Vein Database\\";
	char* message_log = "initial_message.txt";
	int Window[4];
	INFO* parameter = Read_Log(message_log,Window);
	char* out_file = "HOG_ALL_FINGRRS.dat";
	/*****手指信息录入*********/
	//批量录入
	//registration_batching_process(in_file, out_file, parameter,Window);
	//单个录入
	char* Single_file = "C:\\Users\\daiguomeng\\Desktop\\Finger\\Finger Vein Database\\Finger Vein Database\\Finger Vein Database\\001\\left\\middle_1.bmp";
	//registration_single_process(Single_file, out_file, parameter, Window);
	/*******手指检测***********/
	char* input_img = "C:\\Users\\daiguomeng\\Desktop\\Finger\\Finger Vein Database\\Finger Vein Database\\Finger Vein Database\\001\\left\\index_3.bmp";
	float corhence_threshold=0.6;
	int match_index = detect(out_file, input_img, parameter, Window,corhence_threshold);
	printf("匹配结果为：%d", match_index);
	//system("pause");
	return 1;
}