#pragma once

#include "Cdefine.h"
#include "Cglobal.h"
#include "function.h"

int64 dec_bits_buf;
char bytes_index;
char dec_bits_len;


void idct(int* Matrix, int* DCTMatrix, int N, int M);
double Sum_Inverse_2D_DCT(int* mtx, int i, int j, int N, int M);
//Decoding Functions
void VLD();


void Izigzag_scan(char* Decoded_bytestream, int* Izigzag, int Bwidth, int Bheight);

void IDCT();

int get_DC_coef();
char get_coef_value(int64 code, char category);
void pop_bits();