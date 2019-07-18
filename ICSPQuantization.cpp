#include "pch.h"

double QLuminance[8][8] = { 
	{16,  11,  10,  16,  24,  40,  51,  61},
	{12,  12,  14,  19,  26,  58,  60,  55},
	{14,  13,  16,  24,  40,  57,  69,  56},
	{14,  17,  22,  29,  51,  87,  80,  62},
	{18,  22,  37,  56,  68, 109, 103,  77},
	{24,  35,  55,  64,  81, 104, 113,  92},
	{49,  64,  78,  87, 103, 121, 120,  101},
	{72,  92,  95,  98, 112, 100, 103,  99} };

double QChrominance[8][8] = {
	{17,18,	24,	47,	99,	99,	99,	99},
	{18,21,	26,	66,	99,	99,	99,	99},
	{24,26,	56,	99,	99,	99,	99,	99},
	{47,66,	99,	99,	99,	99,	99,	99},
	{99,99,	99,	99,	99,	99,	99,	99},
	{99,99,	99,	99,	99,	99,	99,	99},
	{99,99,	99,	99,	99,	99,	99,	99},
	{99,99,	99,	99,	99,	99,	99,	99} };

void ICSPQuantize(int nowFrameCount) {
	int u, v, y, x;
	double cu, cv, result, MSE, SUM, PSNR;
	
	// 8x8 split
	for (int splitY = 0; splitY < HEIGHT / 8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / 8; splitX++) {
			for (y = 0; y < 8; y++)
			{
				for (x = 0; x < 8; x++)
				{
					DCTFrames[nowFrameCount][8 * splitY + y][8 * splitX + x] = round((tempFrame[8 * splitY + y][8 * splitX + x] / 16));
					printf("%d ", DCTFrames[nowFrameCount][8 * splitY + y][8 * splitX + x]);
				}
				printf("\n");
			}
			printf("\n");
		}
	}
}

void ICSPInverseQuantization(int nowFrameCount) {
	int u, v, y, x;
	double cu, cv, result, MSE, SUM, PSNR;
	
	// 8x8 split
	for (int splitY = 0; splitY < HEIGHT / 8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / 8; splitX++) {

			for (y = 0; y < 8; y++)
			{
				for (x = 0; x < 8; x++)			
				{
					DCTFrames[nowFrameCount][8 * splitY + y][8 * splitX + x] =tempFrame[8 * splitY + y][8 * splitX + x] * 16;
				}
			}
		}
	}
}

/*
void ICSPQuantize(int nowFrameCount) {
	int u, v, y, x;
	double cu, cv, result, MSE, SUM, PSNR;

	// 8x8 split
	for (int splitY = 0; splitY < HEIGHT / 8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / 8; splitX++) {
			for (y = 0; y < 8; y++)
			{
				for (x = 0; x < 8; x++)
				{
					DCTFrames[nowFrameCount][8 * splitY + y][8 * splitX + x] = round((tempFrame[8 * splitY + y][8 * splitX + x] / 16));
					printf("%d ", DCTFrames[nowFrameCount][8 * splitY + y][8 * splitX + x]);
				}
				printf("\n");
			}
			printf("\n");
		}
	}
}

void ICSPInverseQuantization(int nowFrameCount) {
	int u, v, y, x;
	double cu, cv, result, MSE, SUM, PSNR;

	// 8x8 split
	for (int splitY = 0; splitY < HEIGHT / 8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / 8; splitX++) {

			for (y = 0; y < 8; y++)
			{
				for (x = 0; x < 8; x++)
				{
					DCTFrames[nowFrameCount][8 * splitY + y][8 * splitX + x] =tempFrame[8 * splitY + y][8 * splitX + x] * 16;
				}
			}
		}
	}
}
*/