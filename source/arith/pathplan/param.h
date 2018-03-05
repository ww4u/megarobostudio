
#include <vector>
// 常量及变量定义
#define sca  0.3
#define error  1e-4

double step, deltDis, deltTime;
double** P, *P1, *P2, *P3, **V, *V1, *V2, *V3, *T, *T1, *T2, *T3, *tempP, *tempV, *tempT;
int p1Len, p2Len, p3Len, tempLen, n2, n3, mode;
double* posIn;
double* resOut;
struct PosInfo
{
	double x;
	double y;
	double z;
	double t;
	double m;
};

struct ResInfo
{
	double Px;
	double Py;
	double Pz;
	double Vx;
	double Vy;
	double Vz;
	double End;
	double T;
};
PosInfo pos[2];
static std::vector<ResInfo> resInfo;
// 生成PVT
void  CallRobot(int len,int* resLength);
// 轨迹规划
void CalStepTraj();
// 生成三段函数的曲线
void BuildCurve(double** tempPvt, double** tempParam);
// 生成多项式的系数
double** BulidStepFunction(double pStart, double tIn, double tStart, double tEnd, double vStart, double vEnd);
// 多项式差值
double CallPolynomial(double* input, int inputLen, double t);
// 多项式差值
double* CallPolynomial(double* input, int inputLen, double* t, int tLen);
// 删除多项式无效的系数(去掉高次项为0的元素，返回剩下的数组)
int ReviseConst(double* input, int inputLen, double* res);
// 删除二维数组指定行的多项式无效的系数(去掉高次项为0的元素，返回剩下的数组)
int ReviseConst(double* input, double* res, int index);
// 匀速直线运动规划
double** BuildLinear(double pStart, double pEnd, double tStart, double tEnd);
// 计算pvt
void CalPvt(double** param, double** pvt, int index);
// 获得PVT
void GetPvtArray(double* pConst, int plen, double* vConst, int vlen, double* pos, double* time, int index);
// 区间划分
double* DivRegion(double* pIn, int plen, double* tIn, int* len);
double SolveSingalSolution(double* p1, int inputLen, double* t1);
double SolveLinearFunction(double* pIn, double* tIn);
double SolveQuadraticFunction(double* pIn, double* tIn);
double ChechRegion(double* tIn, double res);
double* DisRegion(double p1, double p2, int* flg, int* len);
double* SolAmountSolution(double* pIn, int plen, double* xIn, double* yIn, int ylen);



