//#pragma once
#include <vector>
#define PI 3.1415926
#define ERROR 1E-10

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
/*extern "C" __declspec(dllexport)*/ int GetDeltEndPosition(double* armLength, double* deltAngles, double* p, double* angles, int len, double* result);
/*extern "C" __declspec(dllexport)*/ int  GetDeltArmPosition(double* armLength, double* deltPosition, double*deltAngles, double* posLast, double* pIn, double* vIn, double* tIn, int len, double* res);
int CalPosition(double*armLength, int plen, double* res);
int CalVelocity(double*armLength, double* pIn, double* vIn, int len, double* res);
double* MatrixMult(double* input1, double* input2);
void TansAngles(double* input, double* delt, int len, int mode);
int GetAnswer(double* input1, double* input2, double * outPut);
double CheckInputAngle(double input);
double* SelPos(double* input, double* posLast,double * res);
std::vector<ResInfo> AddZeroPoint(std::vector<ResInfo> infoList);
double* Rank(double* input, int len);
std::vector<ResInfo> DeleteRedundantPVT(double** inputP, double** inputV, double* inputT, int len);


