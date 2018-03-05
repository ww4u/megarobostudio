
#include <vector>
// ��������������
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
// ����PVT
void  CallRobot(int len,int* resLength);
// �켣�滮
void CalStepTraj();
// �������κ���������
void BuildCurve(double** tempPvt, double** tempParam);
// ���ɶ���ʽ��ϵ��
double** BulidStepFunction(double pStart, double tIn, double tStart, double tEnd, double vStart, double vEnd);
// ����ʽ��ֵ
double CallPolynomial(double* input, int inputLen, double t);
// ����ʽ��ֵ
double* CallPolynomial(double* input, int inputLen, double* t, int tLen);
// ɾ������ʽ��Ч��ϵ��(ȥ���ߴ���Ϊ0��Ԫ�أ�����ʣ�µ�����)
int ReviseConst(double* input, int inputLen, double* res);
// ɾ����ά����ָ���еĶ���ʽ��Ч��ϵ��(ȥ���ߴ���Ϊ0��Ԫ�أ�����ʣ�µ�����)
int ReviseConst(double* input, double* res, int index);
// ����ֱ���˶��滮
double** BuildLinear(double pStart, double pEnd, double tStart, double tEnd);
// ����pvt
void CalPvt(double** param, double** pvt, int index);
// ���PVT
void GetPvtArray(double* pConst, int plen, double* vConst, int vlen, double* pos, double* time, int index);
// ���仮��
double* DivRegion(double* pIn, int plen, double* tIn, int* len);
double SolveSingalSolution(double* p1, int inputLen, double* t1);
double SolveLinearFunction(double* pIn, double* tIn);
double SolveQuadraticFunction(double* pIn, double* tIn);
double ChechRegion(double* tIn, double res);
double* DisRegion(double p1, double p2, int* flg, int* len);
double* SolAmountSolution(double* pIn, int plen, double* xIn, double* yIn, int ylen);



