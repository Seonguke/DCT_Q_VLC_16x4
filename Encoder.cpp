#include "Encoder.h"
#include "MyConst.h"
#include <cmath>
#include <cmath>
#include <iostream>
using namespace std;
double Sum_2D_DCT(int* mtx, int u, int v, int N, int M)
{
	double sum = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)

		{
			sum += double(mtx[i * M + j]) * cos(M_PI * (2.0 * i + 1.0) * u / (2.0 * N)) * cos(M_PI * (2.0 * j + 1.0) * v / (2.0 * M));
		}
	return sum;
}
void dct(int* DCTMatrix, int* Matrix, int N, int M)//y x ?
{
	double Pu, Pv;
	double temp = 0;
	for (int u = 0; u < N; u++)
	{
		for (int v = 0; v < M; v++)
		{

			if (u == 0)  Pu = 1.0 / sqrt(2.0);
			else  Pu = 1.0;
			if (v == 0)  Pv = 1.0 / sqrt(2.0);
			else Pv = 1.0;
			temp = sqrt(2.0 / N) * sqrt(2.0 / M) * Pu * Pv * Sum_2D_DCT(Matrix, u, v, N, M);
			DCTMatrix[u * M + v] = (int)round(temp / Constants::yqt[u * M + v]);
			//cout << DCTMatrix[u * M + v] << endl;
		}
	}

}

void Zigzag_scan(int* zigzag, int* dct_coef, int Bwidth, int Bheight)
{
	int c, i, j = 1;
	int x = 0, y = 0, n = 0;
	int flag = 1;// true- up , false - down

	char even[2] = { 0,1 }; //go right
	char odd[2] = { 1,0 };// go donw
	char sw[2][2] = { {1,-1},{-1,1} };// rightup, downleft
	while (1) {
		zigzag[n++] = dct_coef[y * Bwidth + x];
		//cout << "Y = " << y << "X=" << x << ' '<<flag<<endl;
		if (x == Bwidth - 1 && y == Bheight - 1) break;
		if (flag == 1) {//R_U
			if (y + sw[flag][0] < 0 && x + sw[flag][1] < Bwidth) {
				if (x + even[1] < Bwidth) {
					y += even[0];
					x += even[1];
					flag = 0;
				}
				else {
					y += odd[0];
					x += odd[1];
					flag = 0;
				}
			}
			else {
				if (x + sw[flag][1] > Bwidth - 1) {
					y += odd[0];
					x += odd[1];
					flag = 0;
				}
				else {
					x += sw[flag][1];
					y += sw[flag][0];
				}

			}
		}
		else if (flag == 0) {
			if (x + sw[flag][1] < 0 && y + sw[flag][0] < Bheight - 1) {
				if (y + odd[0] < Bheight) {
					y += odd[0];
					x += odd[1];
					flag = 1;
				}
				else {
					y += even[0];
					x += even[1];
					flag = 1;
				}
			}
			else if (y + sw[flag][0] > Bheight - 1)
			{
				y += even[0];
				x += even[1];
				flag = 1;
			}
			else {
				x += sw[flag][1];
				y += sw[flag][0];
			}
		}


	}

	//for (c = 1; c < Bsize * 2; c++) {
	//	for (i = 0; i < j; i++) {
	//		// 지그재그 방향으로 데이터 정렬
	//		cout << "Y:" << y << " X:" << x << endl;
	//		zigzag[n++] = dct_coef[y* Bwidth+x];
	//		if (i < j - 1) {
	//			x += sw[j % 2][0];
	//			y += sw[j % 2][1];
	//		}
	//	}
	//	// 스캔 방향 설정
	//	if (c < Bwidth) {
	//		x += even[j % 2];
	//		y += odd[j % 2];
	//		j++;
	//	}
	//	else {
	//		x += odd[j % 2];
	//		y += even[j % 2];
	//		j--;
	//	}
	//}
	//


}
void get_bytestream()
{

	//int i, end;
	//end = last_data(zigzag,16,4);
	//memset(Encoded_bytestream,0,sizeof(Encoded_bytestream));
	//Encoded_bytestream[0] = zigzag[0] - 0;
	//for(i=1;i<=end;i++)
	//	Encoded_bytestream[i] = zigzag[i];
}

void VLC()
{
	unsigned int code;
	int run=0, category=0;
	int i, end = last_data(Encoded_bytestream,16,4);

	enc_bits_buf = 0;
	enc_bits_len = 0;
	stream_index = 0;
	
	//DC code
	category = get_category(Encoded_bytestream[0]);
	code = get_DC_code(Encoded_bytestream[0], category);
	
	insert_bits(code, enc_bits_len, DC_length[category]);
	enc_bits_len += DC_length[category];

	if(enc_bits_len >= 8){
		for(;enc_bits_len>7;enc_bits_len-=8) push_bits();
	}

	//AC code
	for(i=1;i<end;i++)
	{
		if(Encoded_bytestream[i]!=0)
		{
			// 테이블에 정의된 계수를 찾아 코드값 부여함
			category = get_category(Encoded_bytestream[i]);
			code = get_AC_code(Encoded_bytestream[i], run, get_category(Encoded_bytestream[i]));

			// 바이트열에 데이터 저장
			insert_bits(code, enc_bits_len, AC_length[run][category]);
			enc_bits_len += AC_length[run][category];

			if(enc_bits_len >= 8){
				for(;enc_bits_len>7;enc_bits_len-=8) push_bits();
			}
			run = 0;
		}else{
			run++;
		}
	}

	//End of Block
	code = EOB; run = 0; category = 0;
	insert_bits(code, enc_bits_len, AC_length[run][category]);
	enc_bits_len += AC_length[run][category];
	if(enc_bits_len >= 8){
		for(;enc_bits_len>7;enc_bits_len-=8) push_bits();
	}
	if(enc_bits_len > 0){
		push_bits();
	}
	Encoded_bitstream = (char*)malloc(stream_index);
	memcpy(Encoded_bitstream,bitstream,stream_index);
	flag_encoded = 1;
}

int get_category(char coefficient)
{
	int i, n = sizeof(coef_max_Range) / sizeof(short);
	for(i=0; i<n; i++){
		if((abs(coefficient) >= coef_min_Range[i]) && (abs(coefficient) <= coef_max_Range[i]))
			return i;
	}
}
int get_DC_code(char coefficient, int category)
{
	int code;
	unsigned int mask = (unsigned int)pow(2.0,category) - 1;
	code = DC_basecode[category];
	if(coefficient < 0) coefficient--;
	mask &= coefficient;
	code |= mask;
	return code;
}
int get_AC_code(char coefficient, int run, int category)
{
	int code;
	unsigned int mask = (unsigned int)pow(2.0,category) - 1;
	code = AC_basecode[run][category];
	if(coefficient < 0) coefficient--;
	mask &= coefficient;
	code |= mask;
	return code;
}

void insert_bits(int value, int offset, char length)
{
	int64 mask = (unsigned int)pow(2.0,length) - 1;
	mask = (mask&value)<<64-(offset+length);
	enc_bits_buf |= mask;
}
void push_bits()
{
	unsigned char value=0;
	value |= enc_bits_buf>>56;
	bitstream[stream_index] = value;
	enc_bits_buf = enc_bits_buf<<8;
	stream_index++;
}