#pragma once
typedef struct
{
	// 杆长
	double armLength[4];
	// 关节初始角度，关节初始位移
	double offset[2];
	// 关节角度为0对应的空间点
	double P0[3];
	// 关节角度限制
	double posLim[2];
	// 导程
	double scal;
	// 关节最大速度
	double vM;
	// 0:正解 1：逆解
	int mode;
}Igus;

/*extern "C" __declspec(dllexport) */int  GetIgusPvtLen(Igus *systemParam,double* posIn, double* vIn, double* tIn,int len,int* resLen);
/*extern "C" __declspec(dllexport) */int  GetIgusPvtInfo(double* resOut);
