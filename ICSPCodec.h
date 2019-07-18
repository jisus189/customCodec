#pragma once

#define WIDTH 8
#define HEIGHT 8
#define FRAME_MAX 90
#define BLOCK_8 8

//파라미터로 받을 것 frame수, 
const int		 BLOCK_SIZE = 16;
const int		 SERCH_UNIT = 7;
const int		 SERCH_RANGE = BLOCK_SIZE + SERCH_UNIT + SERCH_UNIT;
const int		 FRAME_SIZE = WIDTH * HEIGHT;
const int		 CB_SIZE = 21120;
const int		 CR_SIZE = 21120;
const int		 GARBAGE_AREA = CB_SIZE + CR_SIZE;

extern unsigned char **frame_no_loss_y;
extern unsigned char **frame_no_loss_u;
extern unsigned char **frame_no_loss_v;

extern byte pFrame[HEIGHT][WIDTH];
extern byte cFrame[HEIGHT][WIDTH];
extern byte rFrame[FRAME_MAX][HEIGHT][WIDTH];
extern byte DCTFrames[FRAME_MAX][HEIGHT][WIDTH];
extern byte calculatedFrames[FRAME_MAX][HEIGHT][WIDTH];
extern byte quantizationFrame[FRAME_MAX][HEIGHT][WIDTH];
extern byte IDCTFrames[FRAME_MAX][HEIGHT][WIDTH];
extern byte tempFrame[HEIGHT][WIDTH];
extern byte byProductFrame[HEIGHT][WIDTH];
extern FILE *infile;

using namespace std;

extern string entrophyString;

typedef enum {
	VERTICAL,
	HORIZENTAL,
	INTRA_MEAN
} IntraMode;


typedef enum {
	zeroCount,
	DC,
	PIXEL,
	MEDIAN,
	MEAN,
	LEFT,
	MEDIAN_R,
	MEAN_R,
	LEFT_R,
	DC_MEDIAN,
	PIXEL_MEDIAN,
	DC_MEAN,
	PIXEL_MEAN
} DPCMMode;

extern DPCMMode ICSPDpcm;
extern IntraMode ICSPIntra;

void getYUVFile(int inputFrames);
void saveByte(const char * filePath, int caseVal);
void saveYUV(const char * filePath, float caseUV = 1);
//unsigned char** getFrame(const char * filePath);