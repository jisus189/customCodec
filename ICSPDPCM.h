#pragma once


void ICSPDpcmSelector(int nowFrameCount, int mode, int target);
void makeDCBlock();
double calculatedValue(int posY, int posX, int maxX, int maxY, int mode, int target);
void medianPrediction(int nowFrameCount, int startY, int startX, int stage);
void medianPrediction_R(int nowFrameCount, int startY, int startX, int stage);
void meanPrediction(int nowFrameCount, int startY, int startX, int stage);
void meanPrediction_R(int nowFrameCount, int startY, int startX, int stage);
void leftPrediction(int nowFrameCount, int startY, int startX, int stage);
void leftPrediction_R(int nowFrameCount, int startY, int startX, int target);