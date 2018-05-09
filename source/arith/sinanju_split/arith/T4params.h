#ifndef T4PARAMS_H
#define T4PARAMS_H
#include <vector>
// 常量及变量定义
#define error  1e-4
#define PI 3.1415926
#define E 2.7182818
double* armLengthArray;
double* deltAnglesArray;
struct ResInfo
{
	double P1;
	double P2;
	double P3;
	double P4;
	double V1;
	double V2;
	double V3;
	double V4;
	double T;
};
std::vector<ResInfo> resInfo;
// 导出函数
// 正解求末端位置
MEGA_EXTERN MEGA_EXPORT int GetEndPosition(double* armLength, double* deltAngles, double* angles, double* res);
// 逆解求关节PVT
MEGA_EXTERN MEGA_EXPORT int  GetPvtInfo(double* res);
MEGA_EXTERN MEGA_EXPORT int  GetPvtLen(double* armLength, double* deltAngles, double* posLast, double* posIn, double* vIn, double* tIn, int len, int* resLen);
bool CheckInputAngles(double* input);
void CalPose(double* pIn, double* pLast, int index, double* pose);
int CalPosition(double* posIn, double* posLast, double* result);
bool CheckError(double data);
void CorrectAngles(double* input, int len);
int CheckPos(int* flg, double* input, int len);
void GetPosition(int index, double* input, int len, double* res);
void SelPos(double* pLast, double* input, int len, double* res);
int CompareAngle(double data1, double date2);
void CalV(double* pos, double* v, double* poseV);
void CalVelocity(double* pos, double* v, double* vOut);
double BizarreAvoid(double input);
void GetContinueAngle(double* last, double* pos, double* res);
void CorrectAngles(double* delt, double* input, double* res);
std::vector<ResInfo> AddZeroPoint(std::vector<ResInfo> infoList);
#endif