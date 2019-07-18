#include "pch.h"

#define BLOCK_8 8
void zig_zag_scanning(int nowFrameCount)
{
	int n, x, y;

	for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			n = 0;
			for (int idx = 0, d = 1; idx <= BLOCK_8 * BLOCK_8 - 1; idx++, d *= (-1))
			{
				x = splitX * 8;
				y = splitY * 8;
				if (idx <= BLOCK_8 - 1)
				{
					if (d == 1)
					{
						y += idx;
					}
					else
					{
						x += idx;
					}

				}
				else
				{
					if (d == 1)
					{
						x += idx - (BLOCK_8 - 1);
						y += BLOCK_8 - 1;
					}
					else
					{
						y += idx - (BLOCK_8 - 1);
						x += BLOCK_8 - 1;
					}

				}

				for (; x >= splitX * 8 && y >= splitY * 8 && x < (splitX + 1) * 8 && y < (splitY + 1) * 8; x = x + d, y = y - d)
				{
					tempFrame[y][x] = byProductFrame[y + n % BLOCK_8][x + n / BLOCK_8];
					n++;
				}
			}
		}
	}
}

void unzig_zag_scanning(int nowFrameCount)
{
	int n, x, y;
	for (int splitY = 0; splitY < HEIGHT / 8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / 8; splitX++) {
			n = 0;
			for (int idx = 0, d = 1; idx <= BLOCK_8 * BLOCK_8 - 1; idx++, d *= (-1))
			{
				x = splitX * 8;
				y = splitY * 8;
				if (idx <= BLOCK_8 - 1)
				{
					if (d == 1)
					{
						y += idx;						
					}
					else
					{
						x += idx;
					}
				}
				else
				{
					if (d == 1)
					{
						x += idx - (BLOCK_8 - 1);
						y += BLOCK_8 - 1;
					}
					else
					{
						y += idx - (BLOCK_8 - 1);
						x += BLOCK_8 - 1;
					}
				}

				for (; x >= splitX * 8 && y >= splitY * 8 && x < (splitX + 1) * 8 && y < (splitY + 1) * 8; x = x + d, y = y - d)
				{
					tempFrame[y + n % BLOCK_8][x + n / BLOCK_8]=byProductFrame[y][x];//byProductFrame[n%WIDTH][n/HEIGHT];
					n++;
				}
			}
		}
	}
	
}
