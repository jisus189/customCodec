//  ISPCCodec.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"

unsigned char **frame_no_loss_yuv;
unsigned char **frame_no_loss_y;
unsigned char **frame_no_loss_u;
unsigned char **frame_no_loss_v;

const char * initYUV2 = "C:/Users/jisus/Downloads/신입생코덱/CIF(352x288)/football_cif(352X288)_90f.yuv";
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
byte IDCTFrames[FRAME_MAX][HEIGHT][WIDTH];
byte tempFrame[HEIGHT][WIDTH];

FILE *infile;

void initYUV() {
	for (int i = 0; i < FRAME_MAX; i++) {
		frame_no_loss_y = new unsigned char *[HEIGHT];
		frame_no_loss_u = new unsigned char *[HEIGHT * 1 / 2];
		frame_no_loss_v = new unsigned char *[HEIGHT * 1 / 2];
	}
	for (int i = 0; i < HEIGHT; i++) {
		frame_no_loss_y[i] = new unsigned char[WIDTH];
	}
	for (int i = 0; i < HEIGHT * 1 / 2; i++) {
		frame_no_loss_u = new unsigned char *[WIDTH * 1 / 2];
		frame_no_loss_v = new unsigned char *[WIDTH * 1 / 2];
	}



	//fclose(infile);
	/*
	FILE *outfile = fopen("C:/Users/jisus/Downloads/신입생코덱/CIF(352x288)/saveYUV.yuv", "wb");
	for (int i = 0; i < FRAME_MAX; i++) {
		fwrite(frame_no_loss_y[i], 1, WIDTH * HEIGHT, outfile);
		fwrite(frame_no_loss_u[i], 1, WIDTH * HEIGHT * 1 / 4, outfile);
		fwrite(frame_no_loss_v[i], 1, WIDTH * HEIGHT * 1 / 4, outfile);
	}
	fclose(outfile);
	*/
}

void getFrame(int frameCount, const char * filePath) {
	/*
	HANDLE hFile;
	DWORD dwRead;

	//얻고자 하는 프레임의 시작 부분의 위치 계산
	frameCount = frameCount * FRAME_SIZE;
	hFile = CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(hFile, frameCount, 0, FILE_BEGIN);
	ReadFile(hFile, tempFrame, FRAME_SIZE, &dwRead, NULL);
	CloseHandle(hFile);
	*/


	if (!infile) {
		printf("NO FILE");
		return;
	}
	for (int i = 0; i < HEIGHT; i++) {
		fread(tempFrame[i], 1, WIDTH, infile);
	}



	printf("SUCCESS\n");
	/*
	for (int i = 0; i < HEIGHT * 1 / 2; i++) {
		fread(frame_no_loss_u[i], 1, WIDTH * 1 / 2, infile);
	}

	for (int i = 0; i < HEIGHT * 1 / 2; i++) {
		fread(frame_no_loss_v[i], 1, WIDTH * 1 / 2, infile);
	}
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)//아래
		{
			printf("%d : %d + %d = %d \n",frameCount,i,j,tempFrame[i][j]);
		}
	}
	*/

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
/*
	입력값 : 원본 영상, 영상크기(w&h 고정), QP_DC, QP_AC, intra Period, enable
	flow   : intra period에 따라 처음 부터
			 1.inter -
			 2.intra -
*/
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
		getFrame(i_frame, YPath);
		ICSPFowardDct(i_frame);
	}
	saveYUV(FDCTPath);
	fclose(infile);

	infile = fopen(YFDCTPath, "rb");
	for (int i_frame = 0; i_frame < FRAME_MAX; i_frame++) {
		getFrame(i_frame, YFDCTPath);
		ICSPInverseDct(i_frame);
	}
	saveYUV(IDCTPath);
	fclose(infile);
}
int main()
{

	//getYUVFile(FRAME_MAX);

	//initYUV();
	infile = fopen(YPath, "rb");
	for (int i_frame = 0; i_frame < FRAME_MAX; i_frame++) {
		getFrame(i_frame, YPath);
		ICSPFowardDct(i_frame);
	}
	saveYUV(FDCTPath);
	fclose(infile);

	infile = fopen(FDCTPath, "rb");
	for (int i_frame = 0; i_frame < FRAME_MAX; i_frame++) {
		getFrame(i_frame, FDCTPath);
		ICSPQuantize(i_frame);
	}
	saveYUV(QuanPath);
	fclose(infile);


	return 0;
	infile = fopen(YFDCTPath, "rb");
	for (int i_frame = 0; i_frame < FRAME_MAX; i_frame++) {
		getFrame(i_frame, YFDCTPath);
		ICSPInverseDct(i_frame);

	}
	saveYUV(IDCTPath);
	fclose(infile);
	//for (int i_frame = 59; i_frame < FRAME_MAX; i_frame++) {
	//	for (int i = 0; i < HEIGHT; i++) {
	//		for (int j = 0; j < WIDTH; j++)//아래
	//		{
	//			printf("%d %d %d =%d\n",i_frame,i,j,DCTFrames[i_frame][i][j]);
	//		}
	//	}
	//}
	//saveByte(, 2);

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
