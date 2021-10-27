#include "function.h"

void Initialize()
{
	int i, j;

	//Data
	unsigned char data[Bsize][Bsize] = {
		234 , 237 , 231 , 231 , 233 , 239 , 235 , 235 , 233 , 237 , 233 , 230 , 227 , 235 , 234 , 231 ,
231 , 232 , 232 , 232 , 229 , 229 , 230 , 232 , 234 , 233 , 234 , 233 , 233 , 233 , 233 , 233 ,
227 , 236 , 237 , 228 , 233 , 237 , 229 , 233 , 236 , 236 , 228 , 232 , 230 , 237 , 232 , 234 ,
237 , 238 , 237 , 233 , 231 , 225 , 219 , 216 , 210 , 207 , 175 , 140 , 127 , 137 , 146 , 156
	};
	//Quantization Table
	char yqt[Bsize*Bsize] = {
		16, 11, 10, 16, 24, 40, 51, 61,
		12, 12, 14, 19, 26, 58, 60, 66,
		14, 13, 16, 24, 40, 57, 69, 57,
		14, 17, 22, 29, 51, 87, 80, 62,
		18, 22, 37, 56, 68,109,103, 77,
		24, 36, 55, 64, 81,104,113, 92,
		49, 64, 78, 87,103,121,120,101,
		72, 92, 95, 98,112,100,103, 99};
	
	memcpy(src_data,data,sizeof(src_data));
	memcpy((char*)YQuanTable,yqt,sizeof(YQuanTable));

	puts("JPEG Algorithm Test\n");
	
	puts(" ---- Original Image Data ----   |      ------  Shifted Array  ------");
	for(j=0;j<Bsize;j++){
		for(i=0;i<Bsize;i++){
			printf("%3d ",src_data[j][i]);
		}
		printf(" | ");
		for(i=0;i<Bsize;i++){
			printf("%4d ",src_data[j][i]-128);
		}
		putchar('\n');
	}putchar('\n');

	puts("Luminance Quantization Table");
	for(j=0;j<Bsize;j++){
		for(i=0;i<Bsize;i++){
			printf("%3d ",YQuanTable[j][i]);
			if(i==7) putchar('\n');
		}
	}putchar('\n');
}

char coef_category(char coef)
{
	int i, category;
	for(i=0;i<16;i++)
	{
		if((coef >= coef_min_Range[i]) && (coef <= coef_max_Range[i])
			||(coef <= -coef_min_Range[i]) && (coef >= -coef_max_Range[i]))
		{
			category = i;
			break;
		}
	}
	return category;
}

int last_data(int* data, int Bwidth, int Bheight)
{
	int i, n;
	for (i = 0; i < Bwidth * Bheight; i++)
		if (data[i] != 0) n = i + 1;
	return n;
}
int last_data(char* data, int Bwidth, int Bheight)
{
	int i, n;
	for (i = 0; i < Bwidth * Bheight; i++)
		if (data[i] != 0) n = i + 1;
	return n;
}

//int round(double n)
//{
//	if(n<0.0){
//		if((int)abs(n*10.0)%10<5)
//			return (int)ceil(n);
//		else
//			return (int)floor(n);
//	}else{
//		if((int)(n*10.0)%10<5)
//			return (int)floor(n);
//		else
//			return (int)ceil(n);
//	}
//}

double PSNR(int* src_data, int* restored, int Bheight, int Bwidht)
{
	double mse = 0, psnr = 0;
	int j, i;

	int N = Bheight * Bwidht;

	for (j = 0; j < Bheight; j++)
		for (i = 0; i < Bwidht; i++)
			mse += double(src_data[j * Bwidht + i] - restored[j * Bwidht + i]) * double(src_data[j * Bwidht + i] - restored[j * Bwidht + i]);

	mse /= double(N);
	psnr = 10 * (double)log10(double(255 * 255) / mse);

	return psnr;
}


void print_bin(unsigned char n)
{
	int i;
	for(i=7;i>=0;i--)
	{
		printf("%d",(n&(int)pow(2.0,i))>>i);
	}
}

/*
	Data 1
		80, 84, 84, 86, 90, 90, 90, 89,
		78, 80, 80, 80, 84, 84, 84, 88,
		76, 81, 84, 88, 82, 80, 77, 77,
		71, 75, 74, 77, 76, 76, 80, 80,
		82, 85, 88, 90, 91, 91, 90, 90,
		90, 91, 99, 95, 90, 88, 84, 84,
		88, 88, 93, 95, 99,100,100,100,
		83, 82, 80, 77, 73, 70, 66, 66
	Data 2
		52, 55, 61, 66, 70, 61, 64, 73,
		63, 59, 66, 90,109, 85, 69, 72,
		62, 59, 68,113,144,104, 66, 73,
		63, 58, 71,122,154,106, 70, 69,
		67, 61, 68,104,126, 88, 68, 70,
		79, 65, 60, 70, 77, 68, 58, 75,
		85, 71, 64, 59, 55, 61, 65, 83,
		87, 79, 69, 68, 65, 76, 78, 94
*/