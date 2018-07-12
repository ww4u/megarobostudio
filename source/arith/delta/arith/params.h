#pragma once
#include <vector>
#define PI 3.1415926
#define ERROR 1E-4
double* deltAngleArray;
double* posIn;
struct ResInfo
{
	double P1;
	double P2;
	double V1;
	double V2;
	double T;
};
std::vector<ResInfo> resInfo;
int GetDeltEndPosition(double* armLength, double* deltAngles, double* p, double* angles, int len, double* result);
int GetDeltArmLen(double* armLength, double* deltPosition, double*deltAngles, double* posLast, double* pIn, double* vIn, double* tIn, int len, int* resCount);
int GetDeltArmPosition(double* res);
int CalPosition(double*armLength, int plen, double* res);
int CalVelocity(double*armLength, double* pIn, double* vIn, int len, double* res);
void MatrixMult(double* input1, double* input2,double* res);
void TansAngles(double* input, double* delt, int len, int mode);
int GetAnswer(double* input1, double* input2, double * outPut);
double CheckInputAngle(double input);
void SelPos(double* input, double* posLast,double * res);
std::vector<ResInfo> AddZeroPoint(std::vector<ResInfo> infoList);


