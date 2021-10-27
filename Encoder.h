#pragma once

#include "Cdefine.h"
#include "Cglobal.h"
#include "function.h"

char flag_encoded;
unsigned char bitstream[64];
int64 enc_bits_buf;
char enc_bits_len;
char stream_index;

//Encoding Functions
double Sum_2D_DCT(int* mtx, int u, int v, int N, int M);
void dct(int* DCTMatrix, int* Matrix, int N, int M);//x y ?;
void Zigzag_scan(int* zigzag, int* dct_coef, int Bwidth, int Bheight);
void get_bytestream();

void VLC();

int get_category(char coefficient);
int get_DC_code(char coefficient, int category);
int get_AC_code(char coefficient, int run, int category);

void insert_bits(int value, int offset, char length);
void push_bits();