#include "pch.h"
#include "ICSPEntropyCoding.h"
#include <vector>
#include <bitset>

using namespace std;

string entropyString[FRAME_MAX];

string entropyWords[13] = {
	"00","010","011","100","101" ,
	"110" ,"1110" ,"11110" ,"111110" ,"1111110" ,
	"11111110","111111110","1111111110" };
//우선 byProductFrame
void ICSPEntropyEncode(int nowFrame) {
	int powTwo,category, absVal;
	string signBit; //maxblock?
	entropyString[nowFrame] = "";
	for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			for (int y = splitY; y < splitY + BLOCK_8; y++)
			{
				for (int x = splitX; x < splitY + BLOCK_8; x++)
				{
					if (byProductFrame[y][x] == 0) { 
						entropyString[nowFrame] += entropyWords[0];
					} //00
					else if (byProductFrame[y][x] == 1) {
						entropyString[nowFrame] += entropyWords[1]+"1";
					}
					else if (byProductFrame[y][x] == -1) {
						entropyString[nowFrame] += entropyWords[1] + "0";
					}
					else {
						if (byProductFrame[y][x] > 0) {
							signBit = "1";
							for (powTwo = 2, category = 1; powTwo < byProductFrame[y][x]; powTwo *= 2) category++;
							//category + 부호bit + 2진수 변환으로 range 계산
					
							entropyString[nowFrame] += entropyWords[category] + signBit + ten2Two(byProductFrame[y][x]);
						}
						else {
							signBit = "0";
							absVal=abs(byProductFrame[y][x]);
							for (powTwo = 2, category = 1; powTwo < absVal; powTwo *= 2) category++;
							entropyString[nowFrame] += entropyWords[category] + signBit + ten2Two(byProductFrame[y][x]);
						}
					}
				}
			}
		}
	}
}

void ICSPEntropyDecode(int nowFrame) {

	int sp, category;

	for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			sp = 0;
			for (int y = splitY; y < splitY + BLOCK_8; y++)
			{
				for (int x = splitX; x < splitY + BLOCK_8; x++)
				{
					
					if (entropyString[nowFrame][sp]	== (char)"0"){
					if (entropyString[nowFrame].substr(sp, 2) == "00") {
						tempFrame[y][x] = 0;
						sp += 2;
					}
					else if (entropyString[nowFrame].substr(sp, 3) == "010") {
						if (entropyString[nowFrame][sp + 3] == 1) tempFrame[y][x] = 1;
						else tempFrame[y][x] = -1;
						sp += 4;
					}
					else if (entropyString[nowFrame].substr(sp, 3) == "011") {
						if (entropyString[nowFrame][sp + 3] == 1) tempFrame[y][x] = 1;
						else tempFrame[y][x] = -1;
						if (entropyString[nowFrame][sp + 4] == 1) tempFrame[y][x] *= 3;
						else tempFrame[y][x] *= 2 ;
						sp += 5;
					}
					}
					else {
						if (entropyString[nowFrame].substr(sp, 3) == "100") {
							if (entropyString[nowFrame][sp + 3] == 1) tempFrame[y][x] = 1;
							else tempFrame[y][x] = -1;
							tempFrame[y][x] *= two2Ten(entropyString[nowFrame].substr(sp+4,2))+4;
							sp += 6;
						}
						else if (entropyString[nowFrame].substr(sp, 3) == "101") {
							if (entropyString[nowFrame][sp + 3] == 1) tempFrame[y][x] = 1;
							else tempFrame[y][x] = -1;
							tempFrame[y][x] *= two2Ten(entropyString[nowFrame].substr(sp + 4, 3))+8;
							sp += 7;	
						}
						//110
						for (int fp = sp+2,category = 5; fp < sp +10; fp++) {
							if (entropyString[nowFrame][fp] == 1) category++;
							else {
								if (entropyString[nowFrame][sp + category-1] == 1) tempFrame[y][x] = 1;
								else tempFrame[y][x] = -1;
								tempFrame[y][x] *= two2Ten(entropyString[nowFrame].substr(sp + category, category-1)) + (int)pow(2,category-1);
								sp += (category-1)*2;
								break;
							}
						}
					}
				}
			}
		}
	}
}

string ten2Two(int num)
{
	string result = "";
	while( num > 0)
	{
		int binary = num % 2;
		if (binary==1) result += "1";
		else if (binary == 0) result += "0";
		num /= 2;
	}
	reverse(result.begin(), result.end());
	return result;
}

int two2Ten(string num)
{
	int sum=0;
	for (int i=1; i<= num.length();i++){
		sum += (num[num.length() - i] - '0')*(int)pow(2, i - 1);
	}
	return sum;
}