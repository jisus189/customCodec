#include "pch.h"
#include "ICSPDPCM.h"
#include <vector>
#include <algorithm>

#define BLOCK_8 8

using namespace std;

//tempFrame을 읽은 후에!
void ICSPDpcmSelector(int nowFrameCount, int mode, int stage) {

	//블록 나눔
	for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			if (mode == LEFT) leftPrediction(nowFrameCount, splitY * 8, splitX * 8, stage);
		}
	}
}
//dc일 경우 
//vector형태의 block 선택 후 처리까지 한 뒤 return
double calculatedValue(int posY, int posX, int maxX, int maxY, int mode, int target) {
	vector<double> tempVector;
	/*0*/tempVector.push_back(tempFrame[posY][posX]);
	if (target ==DC) {

	}
	else if (target == PIXEL) {
		//맨 왼쪽이 아닐 경우
		if (posX - 1 > 0) {
			/*1*/tempVector.push_back(tempFrame[posY][posX - 1]);
			if (posY + 1 < maxY) {
				/*2*/tempVector.push_back(tempFrame[posY+1][posX - 1]);
				/*3*/tempVector.push_back(tempFrame[posY + 1][posX]);
				/*4*/
				if (posX + 1 < maxX) {
					tempVector.push_back(tempFrame[posY + 1][posX + 1]);
				}
				else {
					tempVector.push_back(tempFrame[posY + 1][posX-1]);
				}
			}
			else {
				//맨 윗줄일 경우 left 3개로 대체
				/*1*/tempVector.push_back(tempFrame[posY][posX - 1]);
				/*2*/tempVector.push_back(tempFrame[posY][posX - 1]);
				/*3*/tempVector.push_back(tempFrame[posY][posX - 1]);
				/*4*/tempVector.push_back(tempFrame[posY][posX - 1]);
			}
		}
		else if (posX - 1 < 0) {
			//맨 왼쪽일 경우
			/*1*/tempVector.push_back(tempFrame[posY + 1][posX]);
			/*2*/tempVector.push_back(tempFrame[posY + 1][posX]);
			/*3*/tempVector.push_back(tempFrame[posY + 1][posX]);
			/*4*/tempVector.push_back(tempFrame[posY + 1][posX + 1]);
		}
		
	}
	if (mode == MEAN) {
		double sum;
		for (vector<double>::iterator i = tempVector.begin(); i != tempVector.end(); i++) {
			sum += *i;
		}
		return sum / tempVector.size();
	}
	else if (mode == MEDIAN) {
		sort(tempVector.begin(), tempVector.end());
		return tempVector[2];
	}
}

void medianPrediction(int nowFrameCount, int startY, int startX, int target){
	int x, y;
	int predictedVal = 0;
	if (target == DC) {
		predictedVal = 1024;
		
	}
	else {
		predictedVal = 128;
		for (y = startY, x = startX; y >= startY && y < startY + BLOCK_8; y++) {
			calculatedFrames[nowFrameCount][y][startX] = predictedVal;
			if (y != startY) {
				calculatedFrames[nowFrameCount][y][startX] = calculatedValue(y, x, startY + BLOCK_8, startX + BLOCK_8, MEDIAN, PIXEL);
			}
			//x의 두번째부터  수행
			for (x = startX + 1; x >= startX && x < startX + BLOCK_8; x++)
			{
				calculatedFrames[nowFrameCount][y][x] = calculatedValue(y, x, startY + BLOCK_8, startX + BLOCK_8, MEDIAN, PIXEL);
			}
		}
	}

	
}

void medianPrediction_R(int nowFrameCount, int startY, int startX, int stage) {
	int x, y;
	int predictedVal = 0;
	if (stage == DC) predictedVal = 1024;
	else predictedVal = 128;
	for (y = startY; y >= startY && y < startY + BLOCK_8; y++) {
		calculatedFrames[nowFrameCount][y][startX] = predictedVal;
		if (y != startY) {
			calculatedFrames[nowFrameCount][y][startX] = 1111;
		}
		//x의 두번째부터  수행
		for (x = startX + 1; x >= startX && x < startX + BLOCK_8; x++)
		{
			calculatedFrames[nowFrameCount][y][x] = 11111;
		}
	}
}

void meanPrediction(int nowFrameCount, int startY, int startX, int stage) {
	int x, y;
	int predictedVal = 0;
	if (stage == DC) predictedVal = 1024;
	else predictedVal = 128;
	for (y = startY; y >= startY && y < startY + BLOCK_8; y++) {
		calculatedFrames[nowFrameCount][y][startX] = predictedVal;
		if (y != startY) {
			calculatedFrames[nowFrameCount][y][startX] = calculatedValue(y, x, startY + BLOCK_8, startX + BLOCK_8, MEAN, PIXEL);
		}
		//x의 두번째부터  수행
		for (x = startX + 1; x >= startX && x < startX + BLOCK_8; x++)
		{
			calculatedFrames[nowFrameCount][y][x] = calculatedValue(y, x, startY + BLOCK_8, startX + BLOCK_8, MEAN, PIXEL);
		}
	}
}

void meanPrediction_R(int nowFrameCount, int startY, int startX, int stage) {
	int x, y;
	int predictedVal = 0;
	if (stage == DC) predictedVal = 1024;
	else predictedVal = 128;
	for (y = startY; y >= startY && y < startY + BLOCK_8; y++) {
		calculatedFrames[nowFrameCount][y][startX] = predictedVal;
		if (y != startY) {
			calculatedFrames[nowFrameCount][y][startX] = 1111;
		}
		//x의 두번째부터  수행
		for (x = startX + 1; x >= startX && x < startX + BLOCK_8; x++)
		{
			calculatedFrames[nowFrameCount][y][x] = 11111;
		}
	}
}

void leftPrediction(int nowFrameCount, int startY, int startX, int target) {
	int x, y;
	if (target == DC) {

	}
	else if (target == PIXEL) {
		//맨 처음이고 0이 아닐 경우 위 프레임에서 뺌
		for (y = startY; y >= startY && y < startY + BLOCK_8; y++) {
			calculatedFrames[nowFrameCount][y][startX] = tempFrame[y][startX];
			if (y != startY) calculatedFrames[nowFrameCount][y][startX] = tempFrame[y][startX] - tempFrame[y - 1][startX];
			//x의 두번째부터  수행
			for (x = startX + 1; x >= startX && x < startX + BLOCK_8; x++)
			{
				calculatedFrames[nowFrameCount][y][x] = tempFrame[y][x] - tempFrame[y][x - 1];
			}
		}
	}
}

void leftPrediction_R(int nowFrameCount, int startY, int startX, int target) {
	int x, y;
	if (target == DC) {

	}
	else if (target == PIXEL) {
		//맨 처음이고 0이 아닐 경우 위 프레임에서 뺌
		for (y = startY; y >= startY && y < startY + BLOCK_8; y++) {
			calculatedFrames[nowFrameCount][y][startX] = tempFrame[y][startX];
			if (y != startY) calculatedFrames[nowFrameCount][y][startX] = tempFrame[y][startX] - tempFrame[y - 1][startX];
			//x의 두번째부터  수행
			for (x = startX + 1; x >= startX && x < startX + BLOCK_8; x++)
			{
				calculatedFrames[nowFrameCount][y][x] = tempFrame[y][x] - tempFrame[y][x - 1];
			}
		}
	}
}