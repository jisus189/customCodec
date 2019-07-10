#pragma once

#define WIDTH 352
#define HEIGHT 288
#define FRAME_MAX 90

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
extern byte rFrame[FRAME_MAX - 1][HEIGHT][WIDTH];
extern byte DCTFrames[FRAME_MAX][HEIGHT][WIDTH];
extern byte IDCTFrames[FRAME_MAX][HEIGHT][WIDTH];
extern byte tempFrame[HEIGHT][WIDTH];


//void getYUVFile(int inputFrames);
void initYUV();
void saveByte(const char * filePath, int caseVal);
void saveYUV(const char * filePath, unsigned char ** fileVal, float caseUV = 1);
//unsigned char** getFrame(const char * filePath);