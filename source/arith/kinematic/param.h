

#include <vector>
// 常量及变量定义
#define error  1e-4
#define armLen0  247.75
#define armLen1  255
#define armLen2  250
#define armLen3 0
#define armLen4 0
#define armLen5 0
#define PI 3.1415926
#define E 2.7182818
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
static std::vector<ResInfo> resInfo;
// 导出函数

bool CheckInputAngles(double* input);
double* CalPose(double* pIn, double* pLast, int index);
double* CalPosition(double* posIn, double* posLast);
bool CheckError(double data);
void CorrectAngles(double* input, int len);
int CheckPos(int* flg, double* input, int len);
double* GetPosition(int index, double* input, int len);
double* SelPos(double* pLast, double* input, int len);
int CompareAngle(double data1, double date2);
double * CalV(double* pos, double* v);
double* CalVelocity(double* pos, double* v);
double BizarreAvoid(double input);
double* GetContinueAngle(double* last, double* pos);
std::vector<ResInfo> AddZeroPoint(std::vector<ResInfo> infoList);
double* Rank(double* input, int len);
std::vector<ResInfo> DeleteRedundantPVT(double** inputP, double** inputV, double* inputT, int len);

