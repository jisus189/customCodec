//  ISPCCodec.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>

unsigned char **frame_no_loss_yuv;
unsigned char **frame_no_loss_y;
unsigned char **frame_no_loss_u;
unsigned char **frame_no_loss_v;

const char * inputYUVPath = "C:/Users/jisus/Downloads/신입생코덱/CIF(352x288)/football_cif(352X288)_90f.yuv";
const char * YUVPath = "C:/Users/jisus/Downloads/신입생코덱/CIF(352x288)/saveYUV.yuv";
const char * YPath = "C:/Users/jisus/Downloads/신입생코덱/CIF(352x288)/saveY.yuv";
const char * UPath = "C:/Users/jisus/Downloads/신입생코덱/CIF(352x288)/saveU.yuv";
const char * VPath = "C:/Users/jisus/Downloads/신입생코덱/CIF(352x288)/saveV.yuv";
const char * MVPath = "C:/Users/jisus/Downloads/신입생코덱/CIF(352x288)/saveMV.yuv";
const char * FDCTPath = "C:/Users/jisus/Downloads/신입생코덱/CIF(352x288)/saveDCT.yuv";
const char * IDCTPath = "C:/Users/jisus/Downloads/신입생코덱/CIF(352x288)/saveIDCT.yuv";
const char * YFDCTPath = "C:/Users/jisus/Downloads/신입생코덱/CIF(352x288)/saveYDCT.yuv";
const char * QuanPath = "C:/Users/jisus/Downloads/신입생코덱/CIF(352x288)/saveQuan.yuv";

byte pFrame[HEIGHT][WIDTH];
byte cFrame[HEIGHT][WIDTH];
byte rFrame[FRAME_MAX][HEIGHT][WIDTH];
byte DCTFrames[FRAME_MAX][HEIGHT][WIDTH];
byte quantizationFrame[FRAME_MAX][HEIGHT][WIDTH];
byte calculatedFrames[FRAME_MAX][HEIGHT][WIDTH];
byte IDCTFrames[FRAME_MAX][HEIGHT][WIDTH];
byte tempFrame[HEIGHT][WIDTH];
byte byProductFrame[HEIGHT][WIDTH];

FILE *infile;

DPCMMode ICSPDpcm;
IntraMode ICSPIntra;

void getYUVFile(int inputFrames) {
	frame_no_loss_y = new unsigned char *[inputFrames];
	frame_no_loss_u = new unsigned char *[inputFrames];
	frame_no_loss_v = new unsigned char *[inputFrames];

	for (int i = 0; i < inputFrames; i++) {
		frame_no_loss_y[i] = new unsigned char[WIDTH * HEIGHT];
		frame_no_loss_u[i] = new unsigned char[WIDTH * HEIGHT * 1 / 4];
		frame_no_loss_v[i] = new unsigned char[WIDTH * HEIGHT * 1 / 4];
	}

	FILE *infile = fopen(inputYUVPath, "rb");

	if (!infile) {
		printf("NO FILE");
		return;
	}
	for (int i = 0; i < inputFrames; i++) {
		fread(frame_no_loss_y[i], 1, WIDTH * HEIGHT, infile);
		fread(frame_no_loss_u[i], 1, WIDTH * HEIGHT * 1 / 4, infile);
		fread(frame_no_loss_v[i], 1, WIDTH * HEIGHT * 1 / 4, infile);
	}

	fclose(infile);
	printf("read SUCCESS\n");

	FILE *outfile = fopen(YPath, "wb");
    for (int i = 0; i < FRAME_MAX; i++) {
		fwrite(frame_no_loss_y[i], 1, WIDTH * HEIGHT, outfile);
	}
	fclose(outfile);
	printf("saveY\n");

	outfile = fopen(UPath, "wb");
	for (int i = 0; i < FRAME_MAX; i++) {
		fwrite(frame_no_loss_u[i], 1, WIDTH * HEIGHT * 1 / 4, outfile);
	}
	fclose(outfile);
	printf("saveU\n");

	outfile = fopen(VPath, "wb");
	outfile = fopen(VPath, "wb");
	for (int i = 0; i < FRAME_MAX; i++) {
		fwrite(frame_no_loss_v[i], 1, WIDTH * HEIGHT * 1 / 4, outfile);
	}
	fclose(outfile);
	printf("saveV\n");
}

void getFrame(int frameCount) {
	if (!infile) {
		printf("NO FILE");
		return;
	}
	for (int i = 0; i < HEIGHT; i++) {
		fread(tempFrame[i], 1, WIDTH, infile);
	}

	printf("SUCCESS\n");

}

void saveByte(const char * filePath, int caseVal) {
	HANDLE hFile;
	DWORD dwRead;

	hFile = CreateFile(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (caseVal == 1) {
		for (int i = 0; i < FRAME_MAX; i++)
		{
			WriteFile(hFile, rFrame[i], FRAME_SIZE, &dwRead, NULL);
		}
	}
	else if (caseVal == 2) {
		for (int i = 0; i < FRAME_MAX; i++)
		{
			WriteFile(hFile, DCTFrames[i], FRAME_SIZE, &dwRead, NULL);
		}
	}
	else if (caseVal == 3) {
		for (int i = 0; i < FRAME_MAX; i++)
		{
			WriteFile(hFile, IDCTFrames[i], FRAME_SIZE, &dwRead, NULL);
		}
	}

	CloseHandle(hFile);
	printf("saved saveMV!\n");
}

void saveYUV(const char * filePath, float caseUV) {
	FILE *outfile = fopen(filePath, "wb");
	float savingSize = WIDTH * HEIGHT * caseUV;

	for (int i = 0; i < FRAME_MAX; i++) {
		fwrite(DCTFrames[i], 1, savingSize, outfile);
	}

	fclose(outfile);
}

void testDCT() {
	infile = fopen(YPath, "rb");
	makeFirstRFrame();

	for (int i_frame = 0; i_frame < FRAME_MAX; i_frame++) {
		if (1) { // inter -
			MotionEstimationExc(i_frame);
		}
		else { // intra

		}

		//getFrame(i_frame, YPath);
		//ICSPFowardDct(i_frame);
		//quantization
		//
	}
	saveByte(MVPath, 1);
	//saveYUV(MVPath);

}
void testMV() {
	infile = fopen(YPath, "rb");
	for (int i_frame = 0; i_frame < FRAME_MAX; i_frame++) {
		getFrame(i_frame);
		ICSPFowardDct(i_frame);
	}
	saveYUV(FDCTPath);
	fclose(infile);

	infile = fopen(YFDCTPath, "rb");
	for (int i_frame = 0; i_frame < FRAME_MAX; i_frame++) {
		getFrame(i_frame);
		ICSPInverseDct(i_frame);
	}
	saveYUV(IDCTPath);
	fclose(infile);
}

void testDPCM() {
	int n = 0;
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)//아래
		{
			tempFrame[i][j] = n++;
			printf("%d	", tempFrame[i][j]);
		}
		printf("\n");
	}

	printf("\n");
	ICSPDpcmSelector(0, LEFT, PIXEL);

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)//아래
		{
			printf("%d	", calculatedFrames[0][i][j]);
		}
		printf("\n");
	}
}
void startIntraPrediction(int nowFrame, int mode) {
	ICSPIntraMode(nowFrame, mode);
}

void startInterPrediction(int nowFrame) {
	MotionEstimationExc(nowFrame);
}
/*
	입력값 : 원본 영상, 영상크기(w&h 고정), QP_DC, QP_AC, intra Period, enable
	flow   : intra period에 따라 처음 부터
			 1.inter -
			 2.intra -
*/

int main()
{
	//"C:/Users/jisus/Downloads/신입생코덱/CIF(352x288)/
	int intraPeriod=30;
	//파일명, 크기, QP_DC, QP_AC, intra Period, enable 입력 (MFC)

	//YUV 분할 저장
	getYUVFile(FRAME_MAX);

	infile = fopen(YPath,"rb");
	for (int i_frame = 0; i_frame < FRAME_MAX; i_frame++) {
		getFrame(i_frame);
		//input : tempFrame / output : rFrame[i]
		if (FRAME_MAX % intraPeriod == 0) {
			startIntraPrediction(i_frame,HORIZENTAL);
		}
		else {
			startInterPrediction(i_frame);
		}

		//input : rFrame / output : DCTFrame
		ICSPFowardDct(i_frame);
		//input : DCTFrame / output :quantizationFrame
		ICSPQuantize(i_frame);

		zig_zag_scanning(i_frame);
		ICSPDpcmSelector(i_frame,MEAN,DC);
		//entrophy
		//stream header
	}
	saveYUV(FDCTPath);
	fclose(infile);


	/*

	saveYUV(UPath, frame_no_loss_u, 0.25);
	saveYUV(VPath, frame_no_loss_v, 0.25);
	ICSPFowardDct(DCTPath, frame_no_loss_yuv);
	InitFrames();
	MotionEstimationExc(YUVPath, FRAME_MAX);
	saveME(MVPath);
	MotionEstimationExc(YPath,FRAME_MAX);
	saveME(MVPath);
	MotionEstimationExc(UPath, FRAME_MAX);
	saveME(MVPath);
	MotionEstimationExc(VPath, FRAME_MAX);
	saveME(MVPath);
	*/
}
