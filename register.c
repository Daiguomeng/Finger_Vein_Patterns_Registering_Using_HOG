#include"register.h"
char* string_joint(char* str1, char* str2)
{
	char *name = (char *)malloc(strlen(str1) + strlen(str2));
	strcpy(name, str1);
	strcat(name, str2);
	return name;
}
void registration_batching_process(char* in_file, char* out_file, const INFO* parameter, const int* Window)
{
	//注册函数
	/*******前期裁剪*******/
	/*******预处理，高斯模糊+平均曲率图像生成******/
	int i, j;
	FILE *fp = fopen(out_file, "ab");
	char *in_file1, *in_file2, *in_file3;
	char num1[4];
	for (int i = 1; i <= 106; i++)
	{
		itoa(i, num1, 10);
		if (i < 10)
		{
			in_file1 = string_joint(in_file, "00");
		}
		else
		{
			if (i < 100)
			{
				in_file1 = string_joint(in_file, "0");
			}
			else
			{
				in_file1 = in_file;
			}
		}
		in_file2 = string_joint(in_file1, num1);
		in_file3 = string_joint(in_file2, "\\left\\index_1.bmp");
		ClImage* img = previous_cutter(in_file3, parameter, "roi.bmp", Window);
		if (img == NULL)
		{
			continue;
		}
		/******图像重采样，减少图像大小********/
		uchar *mat = HOG_hist(img, parameter->m1, parameter->m2, parameter->h);
		fwrite(mat, sizeof(uchar), parameter->m1*parameter->m1*parameter->h, fp);
	}
	fclose(fp);
}
int registration_single_process(char* in_file, char* out_file, const INFO* parameter, const int* Window)
{
	//注册函数
	/*******前期裁剪*******/
	/*******预处理，高斯模糊+平均曲率图像生成******/
	int i, j;
	FILE *fp = fopen(out_file, "ab");
	ClImage* img = previous_cutter(in_file, parameter, "roi.bmp", Window);
	if (img == NULL)
	{
		fclose(fp);
		return 0;
	}
	/******图像重采样，减少图像大小********/
	uchar *mat = HOG_hist(img, parameter->m1, parameter->m2, parameter->h);
	fwrite(mat, sizeof(uchar), parameter->m1*parameter->m1*parameter->h, fp);
	free(mat);
	fclose(fp);
	return 0;
}
INFO* Read_Log(char* file, int *Window)
{
	INFO* parameter = (INFO*)malloc(sizeof(INFO));
	FILE* fp = fopen(file, "r");
	if (!fp)
	{
		printf("请输入参数文件initial_message.txt!");
		return 0;
	}
	char str[100];
	fgets(str, 100, fp);
	//fgets(str, 100, fp);
	fscanf(fp, "%d %d %d\n", &parameter->m1, &parameter->m2, &parameter->h);
	fgets(str, 100, fp);
	fscanf(fp, "%d %d %lf\n", &parameter->size.height, &parameter->size.width, &parameter->sig_diff);
	fgets(str, 100, fp);
	fscanf(fp, "%d %d %d %d\n", Window, Window + 1, Window + 2, Window + 3);
	fgets(str, 100, fp);
	fscanf(fp, "%d\n", &parameter->finalHeight);
	fgets(str, 100, fp);
	fscanf(fp, "%d\n", &parameter->threshold);
	fclose(fp);
	return parameter;
}