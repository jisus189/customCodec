#include "pch.h"

void ICSPIntraMode(int nowFrame, int mode) {
	if (mode == VERTICAL) {
		verticalPrediction(nowFrame);
	}
	else if (mode == HORIZENTAL) {
		horizentalPrediction(nowFrame);
	}
	else if (mode == INTRA_MEAN) {
		meanPrediction(nowFrame);
	}
}

void verticalPrediction(int nowFrame) {
	for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			for (int x = splitX; x < splitY + BLOCK_8; x++)
			{
				for (int y = splitY+1; y < splitY + BLOCK_8; y++)
				{
					rFrame[nowFrame][y][x] = tempFrame[0][x] - tempFrame[y][x];
				}
			}
		}
	}
}

void verticalPrediction_R(int nowFrame) {
	for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			for (int x = splitX; x < splitY + BLOCK_8; x++)
			{
				for (int y = splitY + 1; y < splitY + BLOCK_8; y++)
				{
					rFrame[nowFrame][y][x] = tempFrame[0][x] + tempFrame[y][x];
				}
			}
		}
	}
}

void horizentalPrediction(int nowFrame) {
	for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			for (int y = splitY; y < splitY + BLOCK_8; y++)
			{
				for (int x = splitX+1; x < splitY + BLOCK_8; x++)
				{
					rFrame[nowFrame][y][x] = tempFrame[y][0] - tempFrame[y][x];
				}
			}
		}
	}
}

void horizentalPrediction_R(int nowFrame) {
	for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			for (int y = splitY; y < splitY + BLOCK_8; y++)
			{
				for (int x = splitX + 1; x < splitY + BLOCK_8; x++)
				{
					rFrame[nowFrame][y][x] = tempFrame[y][0] + tempFrame[y][x];
				}
			}
		}
	}
}

void meanPrediction(int nowFrame) {
	int tempVal = 0;
	for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			for (int y = splitY; y < splitY + BLOCK_8; y++)
			{
				for (int x = splitX; x < splitY + BLOCK_8; x++)
				{
					tempVal+=tempFrame[y][x];
				}
			}
			tempVal /= BLOCK_8* BLOCK_8;
			for (int y = splitY; y < splitY + BLOCK_8; y++)
			{
				for (int x = splitX; x < splitY + BLOCK_8; x++)
				{
					rFrame[nowFrame][y][x] = tempVal;
				}
			}
		}
	}
}

void meanPrediction_R(int nowFrame) {
	int tempVal = 0;
	for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			for (int y = splitY; y < splitY + BLOCK_8; y++)
			{
				for (int x = splitX; x < splitY + BLOCK_8; x++)
				{
					tempVal += tempFrame[y][x];
				}
			}
			tempVal /= BLOCK_8 * BLOCK_8;
			for (int y = splitY; y < splitY + BLOCK_8; y++)
			{
				for (int x = splitX; x < splitY + BLOCK_8; x++)
				{
					rFrame[nowFrame][y][x] = tempVal;
				}
			}
		}
	}
}