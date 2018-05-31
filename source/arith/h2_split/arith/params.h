//#pragma once
// 常量及变量定义
#define error  1e-4
#define PI 3.1415926
#define E 2.7182818
#define ERROR_OUT_OF_ANGLE_LIMIT -1
#define ERROR_OUT_OF_WORK_SPACE -2
#define SUCCESS 0
#define ERROR 1E-4
struct ArmLength
{
	double Ll;
    double Lr;
	double Lw;
	double Lh;
	double Lb;
	double Lp;
};
struct CorrectData
{
	double X;
	double Y;
	double deltAngleX;
	double deltAngleY;
};

double angleLim[4];
double deltP[2];
ArmLength armLen;
CorrectData correctData;
double workSpace[4];
// 求解
///*extern "C" __declspec(dllexport)*/ int SolveStructureKinematics(double* armLength, double*armLim, double* P0, double* posIn, double*vIn, double* tIn, int len, int mode, double* res);
int PositiveCalc(double* pIn, double* t, int length, double* result);
int OppositeCalc(double* pIn, double* v, double* t, int length, double* result);
void SetStructInfo(double* armLength, double* armLim);
int JudgeAngle(double* p, int len);
void AngleTrans(double *p, int len, int mode);
