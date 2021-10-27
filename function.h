#pragma once

#include "Cdefine.h"
#include "Cglobal.h"

void Initialize();

char coef_category(char coef);


int last_data(int* data, int Bwidth, int Bheight);
int last_data(char* data, int Bwidth, int Bheight);
//int round(double n);

double PSNR(int* src_data, int* restored, int Bheight, int Bwidht);

void print_bin(unsigned char n);