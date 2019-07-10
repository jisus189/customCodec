#include "pch.h"


void ReadFrames(const char * filePath, int pIndex, int cIndex)
{
	HANDLE hFile;
	DWORD dwRead;
	//����� �ϴ� �������� ���� �κ��� ��ġ ���
	pIndex = pIndex * (FRAME_SIZE + GARBAGE_AREA);
	cIndex = cIndex * (FRAME_SIZE + GARBAGE_AREA);

	hFile = CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(hFile, pFrame, FRAME_SIZE, &dwRead, NULL);

	SetFilePointer(hFile, cIndex, 0, FILE_BEGIN);
	//SetFilePointer(hFile, cIndex, 0, FILE_END); �̾��
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
	//���ʰ� �Ʒ���
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < BLOCK_SIZE; j++)//��
		{
			rFrame[now][j][i] = cFrame[j][i];
		}
		for (int j = HEIGHT - BLOCK_SIZE; j < HEIGHT; j++)//�Ʒ�
		{
			rFrame[now][j][i] = cFrame[j][i];
		}
	}
	//���ʰ� ������
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < BLOCK_SIZE; j++)//��
		{
			rFrame[now][i][j] = cFrame[i][j];
		}
		for (int j = WIDTH - BLOCK_SIZE; j < WIDTH; j++)//��
		{
			rFrame[now][i][j] = cFrame[i][j];
		}
	}
}

void MotionEstimation(int now)
{
	//���� ������ ���� ���� �迭
	byte cBlock[BLOCK_SIZE][BLOCK_SIZE];
	//���� �����ӿ��� �˻� ������ ���� �迭
	byte pSerchRange[SERCH_RANGE + BLOCK_SIZE][SERCH_RANGE + BLOCK_SIZE];
	for (int i = 0; i < WIDTH; i += BLOCK_SIZE)
	{
		for (int j = 0; j < HEIGHT; j += BLOCK_SIZE)
		{
			int x, y;
			//���� �����
			MakeBlock(i, j, cBlock);
			//SerchRange�� �����;
			MakeSearchRange(i, j, pSerchRange);
			//���� SerchRange���� Ž���Ѵ�.			
			Serch(cBlock, pSerchRange, &x, &y);
			//ã�ƿ� ���� ��ġ�ϴ� ���� cBlock�� ���� rFreme�� �����Ѵ�.
			MakerBlock(now, i, j, pSerchRange, x, y, cBlock, true);
		}
	}
}

void MotionEstimationFalse(int now)
{
	//���� ������ ���� ���� �迭
	byte cBlock[BLOCK_SIZE][BLOCK_SIZE];
	//���� �����ӿ��� �˻� ������ ���� �迭
	byte pSerchRange[SERCH_RANGE + BLOCK_SIZE][SERCH_RANGE + BLOCK_SIZE];
	for (int i = BLOCK_SIZE; i < WIDTH - BLOCK_SIZE; i += BLOCK_SIZE)
	{
		for (int j = BLOCK_SIZE; j < HEIGHT - BLOCK_SIZE; j += BLOCK_SIZE)
		{
			int x, y;
			//���� �����
			MakeBlock(i, j, cBlock);
			//SerchRange�� �����;
			MakeSearchRange(i, j, pSerchRange);
			//���� SerchRange���� Ž���Ѵ�.			
			Serch(cBlock, pSerchRange, &x, &y);
			//ã�ƿ� ���� ��ġ�ϴ� ���� cBlock�� ���� rFreme�� �����Ѵ�.
			MakerBlock(now, i, j, pSerchRange, x, y, cBlock, false);
		}
	}
}

//���� �����ӿ��� ���� �����.
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

//���� �����ӿ��� �˻� ������ �����
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
	//serch ����� ���� �迭
	unsigned int temp[SERCH_RANGE][SERCH_RANGE];
	//SERCH_RANGE �˻�
	for (int i = 0; i < SERCH_RANGE; i++)
	{
		for (int j = 0; j < SERCH_RANGE; j++)
		{
			sum = 0;
			//�� ���������� BLOCK_SIZE ��ŭ �˻�
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
	//���� ���� ���� �ִ� ��ǥ�� ã�´�;
	FindMiniumPosition(temp, x, y);
}

//SERCH��� �迭���� ���� ���� ���� ��ġ�� �˾ƿ´�.
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
//ã�ƿ� ���� ��ġ�ϴ� ���� cBlock�� ���� rFreme�� �����Ѵ�.
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