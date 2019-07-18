#include "pch.h"
#include "ICSPEntropyCoding.h"
#include <vector>
#include <bitset>

using namespace std;

string entropyString="";
string entropyWords[13] = {
	"00","010","011","100","101" ,
	"110" ,"1110" ,"11110" ,"111110" ,"1111110" ,
	"11111110","111111110","1111111110" };
//우선 byProductFrame
void ICSPEntropyEncode(int nowFrame) {
	int powTwo,category, absVal;
	string signBit; //maxblock?
	for (int splitY = 0; splitY < HEIGHT / BLOCK_8; splitY++) {
		for (int splitX = 0; splitX < WIDTH / BLOCK_8; splitX++) {
			for (int y = splitY; y < splitY + BLOCK_8; y++)
			{
				for (int x = splitX + 1; x < splitY + BLOCK_8; x++)
				{
					if (byProductFrame[y][x] == 0) { 
						entropyString += entropyWords[0];
					} //00
					else if (byProductFrame[y][x] == 1) {
						entropyString += entropyWords[1]+"1";
					}
					else if (byProductFrame[y][x] == -1) {
						entropyString += entropyWords[1] + "0";
					}
					else {
						if (byProductFrame[y][x] > 0) {
							signBit = "1";
							for (powTwo = 2, category = 1; powTwo < byProductFrame[y][x]; powTwo *= 2) category++;
							//category + 부호bit + 2진수 변환으로 range 계산
					
							entropyString += entropyWords[category] + signBit + ten2Two(byProductFrame[y][x]);
						}
						else {
							signBit = "0";
							absVal=abs(byProductFrame[y][x]);
							for (powTwo = 2, category = 1; powTwo < absVal; powTwo *= 2) category++;
							entropyString += entropyWords[category] + signBit + ten2Two(byProductFrame[y][x]);
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