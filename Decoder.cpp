#include "Decoder.h"
#include "MyConst.h"
#include <cmath>
#include <iostream>
using namespace std;
double Sum_Inverse_2D_DCT(int* mtx, int i, int j, int N, int M)
{
	double sum = 0;
	double Pu, Pv;
	for (int u = 0; u < N; u++)
		for (int v = 0; v < M; v++)
		{
			if (u == 0)  Pu = 1.0 / sqrt(2.0);
			else  Pu = 1.0;
			if (v == 0)  Pv = 1.0 / sqrt(2.0);
			else Pv = 1.0;
			//cout << mtx[u * M + v] << endl;
			sum += Pu * Pv * double(mtx[u * M + v]) * double(Constants::yqt[u * M + v]) * cos(M_PI * (2.0 * i + 1.0) * u / (2.0 * N)) * cos(M_PI * (2.0 * j + 1.0) * v / (2.0 * M));
		}
	return sum;
}


void idct(int* Matrix, int* DCTMatrix, int N, int M)
{

	double calc = 0;
	double qt = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{

			calc = sqrt(2.0 / N) * sqrt(2.0 / M) * Sum_Inverse_2D_DCT(DCTMatrix, i, j, N, M);

			/*if (calc > 127)
				calc = 127;
			else if (calc < -128)
				calc = -128;*/

			Matrix[i * M + j] = (int)round(calc + 128);//8bit 128
		}

}


void Izigzag_scan(char* Decoded_bytestream, int* Izigzag, int Bwidth, int Bheight)
{

	int c, i, j = 1;
	int x = 0, y = 0, n = 0;
	int flag = 1;// true- up , false - down

	char even[2] = { 0,1 }; //go right
	char odd[2] = { 1,0 };// go donw
	char sw[2][2] = { {1,-1},{-1,1} };// rightup, downleft

	while (1) {
		Izigzag[y * Bwidth + x] = Decoded_bytestream[n++];
		//cout << "Y = " << y << "X=" << x << endl;
		if ((x == Bwidth - 1 && y == Bheight - 1) || (n == last_data(Decoded_bytestream, Bwidth, Bheight))) break;
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
}

void VLD()
{
	char flag_end=0, flag_found=0;
	int i, iR, iC;
	int64 Basecode=0, data=0;
	char zeroes=0, category, coefficient, AC_index=1;

	memset(Decoded_bytestream,0,sizeof(Decoded_bytestream));

	dec_bits_buf = 0;
	dec_bits_len = 0;

	//DC coefficient
	coefficient = get_DC_coef();
	Decoded_bytestream[0] = coefficient + pre_DC_coef;

	for(i=1;i<Bsize*Bsize;i++)
	{
		if(flag_end == 1) break;

		flag_found = 0;

		//AC coefficient
		for(iR=0;iR<16;iR++)
		{
			//Skip invalid index
			if((iR>0)&&(iR<15)) iC=1;
			else iC=0;

			for(;iC<11;iC++)
			{
				//Read bitstream data into buffer
				if(dec_bits_len < AC_length[iR][iC]) pop_bits();

				//Basecode
				Basecode = dec_bits_buf>>64-(AC_length[iR][iC]-iC);

				if(Basecode == EOB){ //End of Block
					flag_end=1;
					break;
				}

				//Data
				data = dec_bits_buf>>64-(AC_length[iR][iC]);

				//Compare
				if((Basecode==(AC_basecode[iR][iC]>>iC)))
				{
					zeroes = iR;
					category = iC;
					coefficient = get_coef_value(data, category);

					Decoded_bytestream[AC_index+zeroes] = coefficient;
					AC_index += zeroes+1;


					dec_bits_buf = dec_bits_buf<<AC_length[iR][iC];
					dec_bits_len -= AC_length[iR][iC];
					
					flag_found = 1;
					break;
				}
			}
			if((flag_found == 1) || (flag_end == 1)) break;
		}
	}
}



int get_DC_coef()
{
	int i;
	char category, coefficient;
	int64 Basecode, data;

	pop_bits();
	for(i=0;i<=5;i++)
	{
		Basecode = dec_bits_buf>>64-(DC_length[i]-i);
		data = dec_bits_buf>>64-(DC_length[i]);
		if((Basecode==(DC_basecode[i]>>i)))
		{
			category = i;
			coefficient = get_coef_value(data, category);
			
			dec_bits_buf = dec_bits_buf<<DC_length[i];
			dec_bits_len -= DC_length[i];

			return coefficient;
		}
	}
	pop_bits();
	for(i=6;i<=9;i++)
	{
		Basecode = dec_bits_buf>>64-(DC_length[i]-i);
		data = dec_bits_buf>>64-(DC_length[i]);
		if((Basecode==(DC_basecode[i]>>i)))
		{
			category = i;
			coefficient = get_coef_value(data, category);

			dec_bits_buf = dec_bits_buf<<DC_length[i];
			dec_bits_len -= DC_length[i];

			return coefficient;
		}
	}
	pop_bits();
	for(i=10;i<=11;i++)
	{
		Basecode = dec_bits_buf>>64-(DC_length[i]-i);
		data = dec_bits_buf>>64-(DC_length[i]);
		if((Basecode==(DC_basecode[i]>>i)))
		{
			category = i;
			coefficient = get_coef_value(data, category);
			
			dec_bits_buf = dec_bits_buf<<DC_length[i];
			dec_bits_len -= DC_length[i];
			
			return coefficient;
		}
	}
}

char get_coef_value(int64 code, char category)
{
	int64 mask = (int)pow(2.0,category)-1;
	int val;

	code &= mask;
	val = (int)code;
	if((val >= coef_min_Range[category]) && (val <= coef_max_Range[category])){
		return (char)code;
	}else{
		return (char)-(mask&~code);
	}
}
void pop_bits()
{
	int64 mask = 0x00000000000000ff;
	dec_bits_buf |= (mask&Encoded_bitstream[bytes_index]) << 64 - (8+dec_bits_len);
	bytes_index++;
	dec_bits_len += 8;
}