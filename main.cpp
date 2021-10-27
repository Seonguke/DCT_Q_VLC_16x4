#include "function.h"
#include "MyConst.h"
#include "Encoder.h"
#include "Decoder.h"
#include <iostream>
using namespace std;
int  data2[Bsize*Bsize] = {234 , 237 , 231 , 231 , 233 , 239 , 235 , 235 , 233 , 237 , 233 , 230 , 227 , 235 , 234 , 231 ,
231 , 232 , 232 , 232 , 229 , 229 , 230 , 232 , 234 , 233 , 234 , 233 , 233 , 233 , 233 , 233 ,
227 , 236 , 237 , 228 , 233 , 237 , 229 , 233 , 236 , 236 , 228 , 232 , 230 , 237 , 232 , 234 ,
237 , 238 , 237 , 233 , 231 , 225 , 219 , 216 , 210 , 207 , 175 , 140 , 127 , 137 , 146 , 156
};
void main()
{

	int Bwidth = 16;
	int Bheight = 4;
	int i = 0, j = 0;
	int* int_in;
	int* int_out;
	int* dct_coef;
	int* zigzag;
	int* Izigzag;
	//char* Decoded_bytestream;
	int_in = (int*)calloc(Bwidth * Bheight, sizeof(int));// 1차원 array로 바로가야겠구만
	int_out = (int*)calloc(Bwidth * Bheight, sizeof(int));// 1차원 array로 바로가야겠구만

	dct_coef = (int*)calloc(Bwidth * Bheight, sizeof(int));
	zigzag = (int*)calloc(Bwidth * Bheight, sizeof(int));
	Izigzag = (int*)calloc(Bwidth * Bheight, sizeof(int));
	//Decoded_bytestream = (char*)calloc(Bwidth * Bheight, sizeof(char));
	int BitDepth = 8;
	Constants::DC_shift = (BitDepth > 8 ? 512 : 128);
	memcpy(int_in, data2, sizeof(data2));

	puts(" ---- Original Image Data ----   |      ------  Shifted Array  ------");
	for (j = 0; j < Bheight; j++) {
		for (i = 0; i < Bwidth; i++) {
			printf("%3d ", int_in[j * Bwidth + i]);
		}
		printf(" | ");
		for (i = 0; i < Bwidth; i++) {
			int_in[j * Bwidth + i] -= Constants::DC_shift;
			printf("%4d ", int_in[j * Bwidth + i]);
		}
		putchar('\n');
	}putchar('\n');

	puts("Luminance Quantization Table");
	for (j = 0; j < Bheight; j++) {
		for (i = 0; i < Bwidth; i++) {
			printf("%3d ", Constants::yqt[j * Bwidth + i]);
			if (i == Bwidth - 1) putchar('\n');
		}
	}putchar('\n');
	dct(dct_coef, int_in, Bheight, Bwidth);

	puts("Quantized DCT coefficients");
	for (j = 0; j < Bheight; j++) {
		for (i = 0; i < Bwidth; i++) {
			printf("%3d ", dct_coef[j * Bwidth + i]);
			if (i == Bwidth - 1) putchar('\n');
		}
	}putchar('\n');
	Zigzag_scan(zigzag, dct_coef, Bwidth, Bheight);
	int end = last_data(zigzag, Bwidth, Bheight);
	for (i = 0; i <= end; i++)
		Encoded_bytestream[i] = zigzag[i];
	puts("Data (bytes)");
	for (i = 0; i < end; i++) {
		printf("%d ", Encoded_bytestream[i]);
	}puts("EOB\n");
	VLC();
	printf("Encoded Bitstream [%d byte]\n", stream_index);
	for (i = 0; i < stream_index; i++)
	{
		putchar(' ');
		print_bin(Encoded_bitstream[i]);
		putchar(' ');
	}printf("\n\n");

	VLD();
	puts("Decoded Data");
	for (i = 0; i < last_data(Decoded_bytestream, Bwidth, Bheight); i++) {
		printf("%d ", Decoded_bytestream[i]);
		
	}printf("\n\n");

	Izigzag_scan(Decoded_bytestream, Izigzag, Bwidth, Bheight);

	printf("Reordered Data (Inverse Zigzag Scan)\n");
	for (j = 0; j < Bheight; j++) {
		for (i = 0; i < Bwidth; i++) {
			printf("%3d ", Izigzag[j * Bwidth + i]);
			if (i == Bwidth - 1) putchar('\n');
		}
	}putchar('\n');

	idct(int_out, Izigzag, Bheight, Bwidth);
	printf("Restored Data [PSNR: %.2lfdB]\n", PSNR(data2, int_out, Bheight, Bwidth));
	for (j = 0; j < Bheight; j++) {
		for (i = 0; i < Bwidth; i++) {
			printf("%3d ", int_out[j * Bwidth + i]);
			if (i == Bwidth - 1) putchar('\n');
		}
	}putchar('\n');
}
