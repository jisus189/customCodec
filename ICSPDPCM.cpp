#include "pch.h"
#include "ICSPDPCM.h"
#include <vector>
#include <algorithm>

using namespace std;

byte DCBlock[HEIGHT / BLOCK_8][WIDTH / BLOCK_8]; 

//tempFrame�� ���� �Ŀ�!
void ICSPDpcmSelector(int nowFrameCount, int mode, int stage) {
	if (stage == DC) {
		makeDCBlock();
		memcpy(calculatedFrames[nowFrameCount], tempFrame, sizeof(tempFrame));
	}
	//��� ����
	for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			if (mode == MEDIAN) medianPrediction(nowFrameCount, splitY*8, splitX*8, stage);
			else if (mode == MEAN) meanPrediction(nowFrameCount, splitY*8, splitX*8, stage);
			else if (mode == LEFT) leftPrediction(nowFrameCount, splitY*8, splitX*8, stage);
		}
	}
}
//dc��� �����
void makeDCBlock() {
	for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			DCBlock[splitY][splitX] = tempFrame[splitY*BLOCK_8][splitX*BLOCK_8];
		}
	}
}
//vector������ block ���� �� ó������ �� �� return
double calculatedValue(int posY, int posX, int maxX, int maxY, int mode, int target) {
	vector<double> tempVector;
	if (target ==DC) {
		/*0*/tempVector.push_back(DCBlock[posY][posX]);
		//�� ������ �ƴ� ���
		if (posX - 1 > 0) {
			/*1*/tempVector.push_back(DCBlock[posY][posX - 1]);
			if (posY + 1 < maxY) {
				/*2*/tempVector.push_back(DCBlock[posY + 1][posX - 1]);
				/*3*/tempVector.push_back(DCBlock[posY + 1][posX]);
				/*4*/
				if (posX + 1 < maxX) {
					tempVector.push_back(DCBlock[posY + 1][posX + 1]);
				}
				else {
					tempVector.push_back(DCBlock[posY + 1][posX - 1]);
				}
			}
			else {
				//�� ������ ��� left 3���� ��ü
				/*1*/tempVector.push_back(DCBlock[posY][posX - 1]);
				/*2*/tempVector.push_back(DCBlock[posY][posX - 1]);
				/*3*/tempVector.push_back(DCBlock[posY][posX - 1]);
				/*4*/tempVector.push_back(DCBlock[posY][posX - 1]);
			}
		}
		else if (posX - 1 < 0) {
			//�� ������ ���
			/*1*/tempVector.push_back(DCBlock[posY + 1][posX]);
			/*2*/tempVector.push_back(DCBlock[posY + 1][posX]);
			/*3*/tempVector.push_back(DCBlock[posY + 1][posX]);
			/*4*/tempVector.push_back(DCBlock[posY + 1][posX + 1]);
		}
	}
	else if (target == PIXEL) {
		/*0*/tempVector.push_back(tempFrame[posY][posX]);
		//�� ������ �ƴ� ���
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
				//�� ������ ��� left 3���� ��ü
				/*1*/tempVector.push_back(tempFrame[posY][posX - 1]);
				/*2*/tempVector.push_back(tempFrame[posY][posX - 1]);
				/*3*/tempVector.push_back(tempFrame[posY][posX - 1]);
				/*4*/tempVector.push_back(tempFrame[posY][posX - 1]);
			}
		}
		else if (posX - 1 < 0) {
			//�� ������ ���
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

void medianPrediction(int nowFrameCount, int startY, int startX, int stage){
	int x, y;
	int predictedVal = 0;

	if (stage == DC) {
		predictedVal = 1024;
		startX /=  BLOCK_8;
		startY /=  BLOCK_8;
		for (y = startY, x = startX; y >= startY && y < startY + BLOCK_8; y++) {
			calculatedFrames[nowFrameCount][y* BLOCK_8][startX* BLOCK_8] = predictedVal;
			if (y != startY) {
				calculatedFrames[nowFrameCount][y* BLOCK_8][startX* BLOCK_8] = calculatedValue(y, x, startY + BLOCK_8, startX + BLOCK_8, MEDIAN, stage);
			}
			//x�� �ι�°����  ����
			for (x = startX + 1; x >= startX && x < startX + BLOCK_8; x++)
			{
				calculatedFrames[nowFrameCount][y* BLOCK_8][x* BLOCK_8] = calculatedValue(y, x, startY + BLOCK_8, startX + BLOCK_8, MEDIAN, stage);
			}
		}
	}
	else {
		predictedVal = 128;
		for (y = startY, x = startX; y >= startY && y < startY + BLOCK_8; y++) {
			calculatedFrames[nowFrameCount][y][startX] = predictedVal;
			if (y != startY) {
				calculatedFrames[nowFrameCount][y][startX] = calculatedValue(y, x, startY + BLOCK_8, startX + BLOCK_8, MEDIAN, stage);
			}
			//x�� �ι�°����  ����
			for (x = startX + 1; x >= startX && x < startX + BLOCK_8; x++)
			{
				calculatedFrames[nowFrameCount][y][x] = calculatedValue(y, x, startY + BLOCK_8, startX + BLOCK_8, MEDIAN, stage);
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
		//x�� �ι�°����  ����
		for (x = startX + 1; x >= startX && x < startX + BLOCK_8; x++)
		{
			calculatedFrames[nowFrameCount][y][x] = 11111;
		}
	}
}

void meanPrediction(int nowFrameCount, int startY, int startX, int stage) {
	int x, y;
	int predictedVal = 0;
	if (stage == DC) {
		predictedVal = 1024;
		startX /= BLOCK_8;
		startY /= BLOCK_8;
		for (y = startY, x = startX; y >= startY && y < startY + BLOCK_8; y++) {
			calculatedFrames[nowFrameCount][y* BLOCK_8][startX* BLOCK_8] = predictedVal;
			if (y != startY) {
				calculatedFrames[nowFrameCount][y* BLOCK_8][startX* BLOCK_8] = calculatedValue(y, x, startY + BLOCK_8, startX + BLOCK_8, MEAN, stage);
			}
			//x�� �ι�°����  ����
			for (x = startX + 1; x >= startX && x < startX + BLOCK_8; x++)
			{
				calculatedFrames[nowFrameCount][y* BLOCK_8][x* BLOCK_8] = calculatedValue(y, x, startY + BLOCK_8, startX + BLOCK_8, MEAN, stage);
			}
		}
	}
	else {
		predictedVal = 128;
		for (y = startY; y >= startY && y < startY + BLOCK_8; y++) {
			calculatedFrames[nowFrameCount][y][startX] = predictedVal;
			if (y != startY) {
				calculatedFrames[nowFrameCount][y][startX] = calculatedValue(y, x, startY + BLOCK_8, startX + BLOCK_8, MEAN, stage);
			}
			//x�� �ι�°����  ����
			for (x = startX + 1; x >= startX && x < startX + BLOCK_8; x++)
			{
				calculatedFrames[nowFrameCount][y][x] = calculatedValue(y, x, startY + BLOCK_8, startX + BLOCK_8, MEAN, stage);
			}
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
		//x�� �ι�°����  ����
		for (x = startX + 1; x >= startX && x < startX + BLOCK_8; x++)
		{
			calculatedFrames[nowFrameCount][y][x] = 11111;
		}
	}
}

void leftPrediction(int nowFrameCount, int startY, int startX, int stage) {
	int x, y;
	if (stage == DC) {
		//�ѹ� �� �����Ͽ� ����
		//�� ó���̰� 0�� �ƴ� ��� �� �����ӿ��� ��
		for (startY = 0; startY < HEIGHT / BLOCK_8* BLOCK_8; startY++) {
			for (startX = 0; startX < WIDTH / BLOCK_8* BLOCK_8; startX++) {
				//�� ó���̰� 0�� �ƴ� ��� �� �����ӿ��� ��
				for (y = startY; y >= startY && y < startY + BLOCK_8; y++) {
					calculatedFrames[nowFrameCount][y*BLOCK_8][startX*BLOCK_8] = DCBlock[y][startX];
					if (y != startY) calculatedFrames[nowFrameCount][y*BLOCK_8][startX*BLOCK_8] = DCBlock[y][startX] - DCBlock[y - 1][startX];
					//x�� �ι�°����  ����
					for (x = startX + 1; x >= startX && x < startX + BLOCK_8; x++)
					{
						calculatedFrames[nowFrameCount][y*BLOCK_8][x*BLOCK_8] = DCBlock[y][x] - DCBlock[y][x - 1];
					}
				}
			}
		}
	}
	else if (stage == PIXEL) {
		//�� ó���̰� 0�� �ƴ� ��� �� �����ӿ��� ��
		for (y = startY; y >= startY && y < startY + BLOCK_8; y++) {
			calculatedFrames[nowFrameCount][y][startX] = tempFrame[y][startX];
			if (y != startY) calculatedFrames[nowFrameCount][y][startX] = tempFrame[y][startX] - tempFrame[y - 1][startX];
			//x�� �ι�°����  ����
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
		//�� ó���̰� 0�� �ƴ� ��� �� �����ӿ��� ��
		for (y = startY; y >= startY && y < startY + BLOCK_8; y++) {
			calculatedFrames[nowFrameCount][y][startX] = tempFrame[y][startX];
			if (y != startY) calculatedFrames[nowFrameCount][y][startX] = tempFrame[y][startX] - tempFrame[y - 1][startX];
			//x�� �ι�°����  ����
			for (x = startX + 1; x >= startX && x < startX + BLOCK_8; x++)
			{
				calculatedFrames[nowFrameCount][y][x] = tempFrame[y][x] - tempFrame[y][x - 1];
			}
		}
	}
}