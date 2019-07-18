
#include "pch.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#define PI 3.141592654 


unsigned char **UCalloc(int width, int height)
{
	int i, j;
	unsigned char **ptr;
	if ((ptr = (unsigned char**)malloc(height * sizeof(unsigned char*))) == NULL)
	{
		printf("\nMemory allocation failure\n");
		exit(1);
	}
	for (i = 0; i < height; i++)
	{
		if ((ptr[i] = (unsigned char*)malloc(width * sizeof(unsigned char))) == NULL)
		{
			printf("\nMemory allocation failure\n");
			exit(1);
		}
	}
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			ptr[i][j] = 0;
	printf("\nMEMORY ALLOCATION(UNSIGNED CHAR) OK!\n");
	return ptr;
}

void UCfree(unsigned char **ptr, int height)
{
	int i;
	for (i = 0; i < height; i++)
		free(ptr[i]);
	free(ptr);
	printf("\nMEMORY FREE(UNSIGNED CHAR) OK!\n");
}

void Readfile(char *filename, unsigned char **source, int width, int height)
{
	int i, j;
	FILE *readf;
	if ((readf = fopen(filename, "rb")) == NULL)
	{
		fprintf(stderr, "ERROR : File cannot open : %s \n", filename);
		exit(-1);
	}
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			source[i][j] = (unsigned char)getc(readf);
	printf("\n%sFILE READING OK!\n", filename);
	fclose(readf);
}

void Writefile(const char *filename, unsigned char **result, int width, int height)
{
	int i, j;
	FILE *writef;
	if ((writef = fopen(filename, "wb")) == NULL)
	{
		fprintf(stderr, "ERROR : File cannot open : %s \n", filename);
		exit(-1);
	}
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			putc((unsigned char)result[i][j], writef);
	printf("\n%sFILE WRITING OK!\n", filename);
	fclose(writef);
}
double trans[512][512];

void ICSPFowardDct(int nowFrameCount) {
	int u, v,y, x;
	double cu, cv, result, MSE, SUM, PSNR;
	// 8x8 split
	
		for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
			for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			// u,v에 
			for (v = 0; v < BLOCK_8; v++)
			{
				for (u = 0; u < BLOCK_8; u++)
				{
					result = 0.0;
					if (u == 0) cu = 1 / sqrt(2);
					else cu = 1.0;
					if (v == 0) cv = 1 / sqrt(2);
					else cv = 1.0;
					for (x = 0; x < BLOCK_8; x++)
					{
						for (y = 0; y < BLOCK_8; y++)
						{
							result += cos(((2 * (double)x + 1)*(double)u*PI) / 16)*cos(((2 * (double)y + 1)*(double)v*PI) / 16)*(double)rFrame[nowFrameCount][BLOCK_8 * splitY + y][BLOCK_8 * splitX + x];
						}
					}
					tempFrame[BLOCK_8 * splitY + v][BLOCK_8 * splitX + u] = cu * cv / 4 * result;
				}
			}
		}
	}
}

void ICSPFowardDct_test(int nowFrameCount) {
	int u, v, y, x;
	double cu, cv, result, MSE, SUM, PSNR;
	// 8x8 split

	for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			// u,v에 
			for (v = 0; v < BLOCK_8; v++)
			{
				for (u = 0; u < BLOCK_8; u++)
				{
					result = 0.0;
					if (u == 0) cu = 1 / sqrt(2);
					else cu = 1.0;
					if (v == 0) cv = 1 / sqrt(2);
					else cv = 1.0;
					for (x = 0; x < BLOCK_8; x++)
					{

						for (y = 0; y < BLOCK_8; y++)
						{
							result += cos(((2 * (double)x + 1)*(double)u*PI) / 16)*cos(((2 * (double)y + 1)*(double)v*PI) / 16)*(double)tempFrame[BLOCK_8 * splitY + y][BLOCK_8 * splitX + x];
						}
					}
					tempFrame[BLOCK_8 * splitY + v][BLOCK_8 * splitX + u] = cu * cv / 4 * result;
				}
			}
		}
	}
}

void ICSPInverseDct(int nowFrameCount) {
	int u, v, y, x;
	double cu, cv, result, MSE, SUM, PSNR;
	
		for (int splitV = 0; splitV < HEIGHT / BLOCK_8; splitV++) {
			for (int splitU = 0; splitU < WIDTH / BLOCK_8; splitU++) {
			for (x = 0; x < BLOCK_8; x++)
			{
				for (y = 0; y < BLOCK_8; y++)
				{
					result = 0.0;
					for (v = 0; v < BLOCK_8; v++)
					{
						for (u = 0; u < BLOCK_8; u++)
						{
							if (u == 0) cu = 1 / sqrt(2);
							else cu = 1.0;
							if (v == 0) cv = 1 / sqrt(2);
							else cv = 1.0;
							result += cu * cv * cos(((2 * (double)x + 1)*(double)u*PI) / 16)*cos(((2 * (double)y + 1)*(double)v*PI) / 16)*(double)rFrame[nowFrameCount][BLOCK_8 * splitV + v][BLOCK_8 * splitU + u];
						}
					}
					tempFrame[BLOCK_8 * splitV + y][BLOCK_8 * splitU + x] = result / 4;
				}
			}
		}
	}
}

void ICSPInverseDct_test(int nowFrameCount) {
	int u, v, y, x;
	double cu, cv, result, MSE, SUM, PSNR;

	for (int splitV = 0; splitV < HEIGHT / BLOCK_8; splitV++) {
		for (int splitU = 0; splitU < WIDTH / BLOCK_8; splitU++) {
			for (x = 0; x < BLOCK_8; x++)
			{
				for (y = 0; y < BLOCK_8; y++)
				{
					result = 0.0;
					for (v = 0; v < BLOCK_8; v++)
					{
						for (u = 0; u < BLOCK_8; u++)
						{
							if (u == 0) cu = 1 / sqrt(2);
							else cu = 1.0;
							if (v == 0) cv = 1 / sqrt(2);
							else cv = 1.0;
							result += cu * cv * cos(((2 * (double)x + 1)*(double)u*PI) / 16)*cos(((2 * (double)y + 1)*(double)v*PI) / 16)*(double)rFrame[nowFrameCount][BLOCK_8 * splitV + v][BLOCK_8 * splitU + u];
						}
					}
					tempFrame[BLOCK_8 * splitV + y][BLOCK_8 * splitU + x] = result / 4;
				}
			}
		}
	}
}
/*
int main(void) {

	char imagename[100];
	char imagenames[100];
	char imagefullname[100];
	unsigned char **image, **dct;
	int i, j, k, l, m, n;
	double cu, cv, result, MSE, SUM, PSNR;
	sprintf(imagename, "lena"); // 파일명입력
	sprintf(imagefullname, "%s.img", imagename); // 파일의 확장자 입력
	image = UCalloc(512, 512);
	dct = UCalloc(512, 512);
	Readfile(imagefullname, image, 512, 512); //Forward DCT

	for (i = 0; i < 64; i++)
	{
		for (j = 0; j < 64; j++)
		{
			for (k = 0; k < 8; k++)
			{
				for (l = 0; l < 8; l++)
				{
					result = 0.0;
					for (m = 0; m < 8; m++)
					{
						for (n = 0; n < 8; n++)
						{
							result += cos((((2 * (double)m + 1)*(double)k*PI) / 16))*cos((((2 * (double)n + 1)*(double)l*PI) / 16))*(double)image[(i * 8) + m][(j * 8) + n];
						}
					}
					if (k == 0) cu = 1 / sqrt(2);
					else cu = 0.0;
					if (l == 0) cv = 1 / sqrt(2);
					else cv = 0.0;
					trans[(i * 8) + k][(j * 8) + l] = result * ((cu*cv) / 4);
				}
			}
		}
	} // DCT된 상태 출력
	for (i = 0; i < 512; i++) {
		for (j = 0; j < 512; j++)
		{
			dct[i][j] = (unsigned char)floor(trans[i][j] + 0.5);
		}
	}
	sprintf(imagenames, "%s_ForDCT.raw", imagename);
	Writefile(imagenames, dct, 512, 512);
	UCfree(dct, 512); //Inverse DCT
	for (i = 0; i < 64; i++)
	{
		for (j = 0; j < 64; j++)
		{
			for (n = 0; m < 8; m++)
			{
				for (m = 0; n < 8; n++)
				{
					result = 0.0;
					for (l = 0; k < 8; k++)
					{
						for (k = 0; l < 8; l++)
						{
							if (k == 0) cu = 1 / sqrt(2);
							else cu = 0.0; if (l == 0) cv = 1 / sqrt(2); else cv = 0.0; result += ((cu*cv) / 4)*cos((((2 * (double)m + 1)*(double)k*PI) / 16))*cos((((2 * (double)n + 1)*(double)l*PI) / 16))*trans[(i * 8) + k][(j * 8) + l];
						}
					}
					image[(i * 8) + m][(j * 8) + n] = (unsigned char)floor(result + 0.5);
				}
			}
		}
	}
	sprintf(imagenames, "%s_InvDCT.raw", imagename);
	Writefile(imagenames, image, 512, 512);
	UCfree(image, 512); // PSNR 계산
	image = UCalloc(512, 512);
	dct = UCalloc(512, 512);
	Readfile(imagefullname, image, 512, 512);
	sprintf(imagenames, "%s_InvDCT.raw", imagename);
	Readfile(imagenames, dct, 512, 512);
	MSE = 0.0; SUM = 0.0; for (i = 0; i < 512; i++)
	{
		for (j = 0; j < 512; j++)
		{
			SUM += ((int)image[i][j] - (int)dct[i][j])*((int)image[i][j] - (int)dct[i][j]);
		}
	}
	MSE = SUM / 262144.0;
	PSNR = 10.0*log10((255.0*255.0) / MSE);
	printf("PSNR : %f\n\n", PSNR); // 원본과 복구한 그림 비교
	k = 1;
	for (i = 0; i < 512; i++)
	{
		for (j = 0; j < 512; j++)
		{
			if (image[i][j] != dct[i][j])
			{
				printf("두 파일의 %d행 %d열 픽셀이 틀립니다.\n", i + 1, j + 1);
				k = 0;
			}
		}
	} // 비교 후 무결성 여부 발표
	if (k == 0) printf("원복과 복구한 이미지가 같지 않습니다.\n\n");
	else printf("원복과 복구한 이미지가 동일합니다.\n\n");
	return 0;
}
*/