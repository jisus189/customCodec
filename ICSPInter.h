#pragma once

void ReadFrames(int pIndex, int cIndex);
void makeFirstRFrame();
void MotionEstimationExc(int nowFrameCount);
void CopyMargin(int now);
void InitFrames();
void MotionEstimation(int now);
void MotionEstimationFalse(int now);
void MakeBlock(int wid, int hei, byte(*cBlock)[BLOCK_SIZE]);
void MakeSearchRange(int wid, int hei, byte(*pSerchRange)[SERCH_RANGE + BLOCK_SIZE]);
void Serch(byte(*cBlock)[BLOCK_SIZE], byte(*pSerchRange)[SERCH_RANGE + BLOCK_SIZE], int *x, int* y);
void FindMiniumPosition(unsigned int(*temp)[SERCH_RANGE], int* x, int* y);
void MakerBlock(int now, int wid, int hei, byte(*pSerchRange)[SERCH_RANGE + BLOCK_SIZE], int x, int y, byte(*cBlock)[BLOCK_SIZE], bool caseVal);

