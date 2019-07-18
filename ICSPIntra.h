#pragma once

void ICSPIntraMode(int nowFrame, int mode);
void verticalPrediction(int nowFrame);
void verticalPrediction_R(int nowFrame);
void horizentalPrediction(int nowFrame);
void horizentalPrediction_R(int nowFrame);
void meanPrediction(int nowFrame);
void meanPrediction_R(int nowFrame);