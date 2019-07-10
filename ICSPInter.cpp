#include "pch.h"


void ReadFrames(const char * filePath, int pIndex, int cIndex)
{
	HANDLE hFile;
	DWORD dwRead;
	//얻고자 하는 프레임의 시작 부분의 위치 계산
	pIndex = pIndex * (FRAME_SIZE + GARBAGE_AREA);
	cIndex = cIndex * (FRAME_SIZE + GARBAGE_AREA);

	hFile = CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(hFile, pFrame, FRAME_SIZE, &dwRead, NULL);

	SetFilePointer(hFile, cIndex, 0, FILE_BEGIN);
	//SetFilePointer(hFile, cIndex, 0, FILE_END); 이어쓰기
	ReadFile(hFile, cFrame, FRAME_SIZE, &dwRead, NULL);
	CloseHandle(hFile);

}

void makeFirstRFrame(const char * filePath) {

	DWORD dwRead;
	HANDLE hFile;

	hFile = CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if ((unsigned int)hFile == 0xffffffff)		return;
	ReadFile(hFile, rFrame, FRAME_SIZE, &dwRead, NULL);
	CloseHandle(hFile);
	return;

}

void InitFrames()
{
	int i = 0;
	int j = 0;
	for (; i < WIDTH; i++)
	{
		for (; j < HEIGHT; j++)
		{
			pFrame[i][j] = 1;
		}
	}
	for (i = 0; i < WIDTH; i++)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			cFrame[i][j] = 1;
		}
	}
}

void MotionEstimationExc(const char * filePath, int inputFrames) {
	makeFirstRFrame(filePath);
	for (int i = 0; i < inputFrames; i++)
	{
		ReadFrames(filePath, i, i + 1);
		CopyMargin(i + 1);
		MotionEstimation(i + 1);
	}
}

void CopyMargin(int now)
{
	//위쪽과 아래쪽
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < BLOCK_SIZE; j++)//위
		{
			rFrame[now][j][i] = cFrame[j][i];
		}
		for (int j = HEIGHT - BLOCK_SIZE; j < HEIGHT; j++)//아래
		{
			rFrame[now][j][i] = cFrame[j][i];
		}
	}
	//왼쪽과 오른쪽
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < BLOCK_SIZE; j++)//좌
		{
			rFrame[now][i][j] = cFrame[i][j];
		}
		for (int j = WIDTH - BLOCK_SIZE; j < WIDTH; j++)//우
		{
			rFrame[now][i][j] = cFrame[i][j];
		}
	}
}

void MotionEstimation(int now)
{
	//현재 조사할 블럭을 담을 배열
	byte cBlock[BLOCK_SIZE][BLOCK_SIZE];
	//이전 프레임에서 검색 영역을 담을 배열
	byte pSerchRange[SERCH_RANGE + BLOCK_SIZE][SERCH_RANGE + BLOCK_SIZE];
	for (int i = 0; i < WIDTH; i += BLOCK_SIZE)
	{
		for (int j = 0; j < HEIGHT; j += BLOCK_SIZE)
		{
			int x, y;
			//블럭을 만든다
			MakeBlock(i, j, cBlock);
			//SerchRange를 만든다;
			MakeSearchRange(i, j, pSerchRange);
			//블럭을 SerchRange에서 탐색한다.			
			Serch(cBlock, pSerchRange, &x, &y);
			//찾아온 가장 일치하는 블럭과 cBlock를 빼서 rFreme에 저장한다.
			MakerBlock(now, i, j, pSerchRange, x, y, cBlock, true);
		}
	}
}

void MotionEstimationFalse(int now)
{
	//현재 조사할 블럭을 담을 배열
	byte cBlock[BLOCK_SIZE][BLOCK_SIZE];
	//이전 프레임에서 검색 영역을 담을 배열
	byte pSerchRange[SERCH_RANGE + BLOCK_SIZE][SERCH_RANGE + BLOCK_SIZE];
	for (int i = BLOCK_SIZE; i < WIDTH - BLOCK_SIZE; i += BLOCK_SIZE)
	{
		for (int j = BLOCK_SIZE; j < HEIGHT - BLOCK_SIZE; j += BLOCK_SIZE)
		{
			int x, y;
			//블럭을 만든다
			MakeBlock(i, j, cBlock);
			//SerchRange를 만든다;
			MakeSearchRange(i, j, pSerchRange);
			//블럭을 SerchRange에서 탐색한다.			
			Serch(cBlock, pSerchRange, &x, &y);
			//찾아온 가장 일치하는 블럭과 cBlock를 빼서 rFreme에 저장한다.
			MakerBlock(now, i, j, pSerchRange, x, y, cBlock, false);
		}
	}
}

//현재 프레임에서 블럭을 만든다.
void MakeBlock(int wid, int hei, byte(*cBlock)[BLOCK_SIZE])
{
	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		for (int j = 0; j < BLOCK_SIZE; j++)
		{
			cBlock[j][i] = cFrame[hei + j][wid + i];
		}
	}
}

//이전 프레임에서 검색 영역을 만든다
void MakeSearchRange(int wid, int hei, byte(*pSerchRange)[SERCH_RANGE + BLOCK_SIZE])
{
	int indexX = hei - SERCH_UNIT;
	int indexY = wid - SERCH_UNIT;

	for (int i = 0; i < SERCH_RANGE + BLOCK_SIZE; i++)
	{
		for (int j = 0; j < SERCH_RANGE + BLOCK_SIZE; j++)
		{
			pSerchRange[j][i] = pFrame[indexX + j][indexY + i];
		}
	}
}


void Serch(byte(*cBlock)[BLOCK_SIZE], byte(*pSerchRange)[SERCH_RANGE + BLOCK_SIZE], int *x, int* y)
{
	unsigned int sum = 0;
	//serch 결과를 담을 배열
	unsigned int temp[SERCH_RANGE][SERCH_RANGE];
	//SERCH_RANGE 검색
	for (int i = 0; i < SERCH_RANGE; i++)
	{
		for (int j = 0; j < SERCH_RANGE; j++)
		{
			sum = 0;
			//각 기준점마다 BLOCK_SIZE 만큼 검색
			for (int k = 0; k < BLOCK_SIZE; k++)
			{
				for (int m = 0; m < BLOCK_SIZE; m++)
				{
					sum += abs(((pSerchRange[j + m][i + k]) - (cBlock[m][k])));
				}
			}
			temp[j][i] = sum;
			sum = 0;
		}
	}
	//제일 작은 값이 있는 좌표를 찾는다;
	FindMiniumPosition(temp, x, y);
}

//SERCH결과 배열에서 가장 작은 곳의 위치를 알아온다.
void FindMiniumPosition(unsigned int(*temp)[SERCH_RANGE], int* x, int* y)
{
	unsigned int minimum = 2147483647;
	for (int i = 0; i < SERCH_RANGE; i++)
	{
		for (int j = 0; j < SERCH_RANGE; j++)
		{
			if (temp[j][i] < minimum)
			{
				minimum = temp[j][i];
			}
		}
	}

	for (int i = 0; i < SERCH_RANGE; i++)
	{
		for (int j = 0; j < SERCH_RANGE; j++)
		{
			if (temp[j][i] == minimum)
			{
				*x = i;
				*y = j;
				return;
			}
		}
	}
}
//찾아온 가장 일치하는 블럭과 cBlock를 빼서 rFreme에 저장한다.
void MakerBlock(int now, int wid, int hei, byte(*pSerchRange)[SERCH_RANGE + BLOCK_SIZE], int x, int y, byte(*cBlock)[BLOCK_SIZE], bool caseVal)
{
	if (caseVal) {
		for (int i = 0; i < BLOCK_SIZE; i++)
		{
			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				rFrame[now][hei + j][wid + i] = abs(pSerchRange[y + j][x + i] - cBlock[j][i]);
			}
		}
	}
	else {
		for (int i = 0; i < BLOCK_SIZE; i++)
		{
			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				rFrame[now][hei + j][wid + i] = rFrame[now - 1][hei + j][wid + i] - cFrame[hei + j][wid + i];
			}
		}
	}

}