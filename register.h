#pragma once
#include "HOG.h"
char* string_joint(char* str1, char* str2);
INFO* Read_Log(char* file, int *Window);
void registration_batching_process(char* in_file, char* out_file, const INFO* parameter, const int* Window);
int registration_single_process(char* in_file, char* out_file, const INFO* parameter, const int* Window);