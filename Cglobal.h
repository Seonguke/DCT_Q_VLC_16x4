#pragma once

#include "Cdefine.h"

char YQuanTable[Bsize][Bsize];

//Encode Data
unsigned char src_data[Bsize][Bsize];
char dct_coef[Bsize][Bsize];
//char zigzag[Bsize*Bsize];
char Encoded_bytestream[Bsize*Bsize];

//VLC data
char *Encoded_bitstream;

//Decode Data
char Decoded_bytestream[Bsize*Bsize];
char Izigzag[Bsize][Bsize];
unsigned char restored[Bsize][Bsize];

//DC coefficient of previous block
char pre_DC_coef =0;

//Table: JPEG coefficient coding categories
unsigned short coef_max_Range[16] = {
	0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767
};
unsigned short coef_min_Range[16] = {
	0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384
};

//Table: JPEG default DC code(luminance)
unsigned int DC_basecode[12] = {
	0x02, 0x06, 0x10, 0x00, 0x50, 0xC0, 0x0380, 0x0F00, 0x3E00, 0xFC00, 0x03F800, 0x0FF000
};
char DC_length[12] = {
	3, 4, 5, 5, 7, 8, 10, 12, 14, 16, 18, 20
};

//Table: JPEG default AC code(luminance)
unsigned int AC_basecode[16][11] = {
	{   EOB,     0x00,     0x04,     0x20,     0xB0,   0x0340,   0x0E00,   0x3C00, 0x03F600, 0x01FF0400, 0x03FE0C00},
	{   EOB,     0x18,     0xE4,   0x03C8,   0x1F60,   0xFEC0, 0x3FE100, 0x7FC280, 0xFF8600, 0x01FF0E00, 0x03FE2000},
	{   EOB,     0x36,   0x03E0,   0x1FB8, 0x0FF890, 0x1FF140, 0x3FE2C0, 0x7FC600, 0xFF8D00, 0x01FF1C00, 0x03FE3C00},
	{   EOB,     0x74,   0x07DC,   0x3FB8, 0x0FF900, 0x1FF220, 0x3FE480, 0x7FC980, 0xFF9400, 0x01FF2A00, 0x03FE5800},
	{   EOB,     0x76,   0x0FE0, 0x07FCB8, 0x0FF980, 0x1FF320, 0x3FE680, 0x7FCD80, 0xFF9C00, 0x01FF3A00, 0x03FE7800},
	{   EOB,     0xF4,   0x0FE4, 0x07FCF8, 0x0FFA00, 0x1FF420, 0x3FE880, 0x7FD180, 0xFFA400, 0x01FF4A00, 0x03FE9800},
	{   EOB,     0xF6,   0x1FE0, 0x07FD38, 0x0FFA80, 0x1FF520, 0x3FEA80, 0x7FD580, 0xFFAC00, 0x01FF5A00, 0x03FEB800},
	{   EOB,   0x01F2,   0x1FE4, 0x07FD78, 0x0FFB00, 0x1FF620, 0x3FEC80, 0x7FD980, 0xFFB400, 0x01FF6A00, 0x03FED800},
	{   EOB,   0x01F4, 0x01FF00, 0x07FDB8, 0x0FFB80, 0x1FF720, 0x3FEE80, 0x7FDD80, 0xFFBC00, 0x01FF7A00, 0x03FEF800},
	{   EOB,   0x03F0, 0x03FEFC, 0x07FE00, 0x0FFC10, 0x1FF840, 0x3FF0C0, 0x7FE200, 0xFFC500, 0x01FF8C00, 0x03FF1C00},
	{   EOB,   0x03F2, 0x03FF20, 0x07FE48, 0x0FFCA0, 0x1FF960, 0x3FF300, 0x7FE680, 0xFFCE00, 0x01FF9E00, 0x03FF4000},
	{   EOB,   0x03F4, 0x03FF44, 0x07FE90, 0x0FFD30, 0x1FFA80, 0x3FF540, 0x7FEB00, 0xFFD700, 0x01FFB000, 0x03FF6400},
	{   EOB,   0x07F4, 0x03FF68, 0x07FED8, 0x0FFDC0, 0x1FFBA0, 0x3FF780, 0x7FEF80, 0xFFE000, 0x01FFC200, 0x03FF8800},
	{   EOB,   0x0FF4, 0x03FF8C, 0x07FF20, 0x0FFE50, 0x1FFCC0, 0x3FF9C0, 0x7FF400, 0xFFE900, 0x01FFD400, 0x03FFAC00},
	{   EOB,   0x1FEC, 0x03FFB0, 0x07FF68, 0x0FFEE0, 0x1FFDE0, 0x3FFC00, 0x7FF880, 0xFFF200, 0x01FFE600, 0x03FFD000},
	{0x0FF7, 0x01FFEA, 0x03FFD8, 0x07FFB8, 0x0FFF80, 0x1FFF20, 0x3FFE80, 0x7FFD80, 0xFFFC00, 0x01FFFA00, 0x03FFF800}
};

char AC_length[16][11] = {
	{ 4,  3,  4,  6,  8, 10, 12, 14, 18, 25, 26},
	{ 0,  5,  8, 10, 13, 16, 22, 23, 24, 25, 26},
	{ 0,  6, 10, 13, 20, 21, 22, 23, 24, 25, 26},
	{ 0,  7, 11, 14, 20, 21, 22, 23, 24, 25, 26},
	{ 0,  7, 12, 19, 20, 21, 22, 23, 24, 25, 26},
	{ 0,  8, 12, 19, 20, 21, 22, 23, 24, 25, 26},
	{ 0,  8, 13, 19, 20, 21, 22, 23, 24, 25, 26},
	{ 0,  9, 13, 19, 20, 21, 22, 23, 24, 25, 26},
	{ 0,  9, 17, 19, 20, 21, 22, 23, 24, 25, 26},
	{ 0, 10, 18, 19, 20, 21, 22, 23, 24, 25, 26},
	{ 0, 10, 18, 19, 20, 21, 22, 23, 24, 25, 26},
	{ 0, 10, 18, 19, 20, 21, 22, 23, 24, 25, 26},
	{ 0, 11, 18, 19, 20, 21, 22, 23, 24, 25, 26},
	{ 0, 12, 18, 19, 20, 21, 22, 23, 24, 25, 26},
	{ 0, 13, 18, 19, 20, 21, 22, 23, 24, 25, 26},
	{12, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26}
};