#include "params.h"
#include "math.h"
#include <vector>
// 常量及变量定义
#define error  5e-4
#define PI 3.1415926
#define E 2.7182818
double* armLengthArray;
double vMax;
double sca;
double thetaS;
double deltArm[2];
double* posLim;
struct ResInfo
{
	double P1;
	double P2;
	double P3;
	double V1;
	double V2;
	double V3;
	double T;
};
Igus igusParam;
std::vector<ResInfo> resInfo;
int TransMotion(double* pIn, double* vIn, int len, int index, double* pout, double* vout);
int GetPvtData(double* pIn, double* vIn, double* tIn, int len, int index);
int CalPosition(double* pIn, double* pOut, int index);
int CalV(double* p1, double* p2, double* vIn, double* vOut, int index);
extern "C" __declspec(dllexport) int  GetIgusPvtInfo(double* resOut)
{
	for (int i = 0; i < resInfo.size(); i++)
	{
		resOut[7 * i] = resInfo[i].P1;
		resOut[7 * i + 1] = resInfo[i].P2;
		resOut[7 * i + 2] = resInfo[i].P3;
		resOut[7 * i + 3] = resInfo[i].V1;
		resOut[7 * i + 4] = resInfo[i].V2;
		resOut[7 * i + 5] = resInfo[i].V3;
		resOut[7 * i + 6] = resInfo[i].T;
	}
	return 0;
}
extern "C" __declspec(dllexport) int  GetIgusPvtLen(Igus *systemParam, double* posIn, double* vIn, double* tIn, int len, int* resLen)
{
	igusParam = *systemParam;
	armLengthArray = igusParam.armLength;
	if (igusParam.scal < error)
	{
		return -1;
	}
	sca = igusParam.scal;
	vMax = igusParam.vM;
	posLim = igusParam.posLim;
	for (int i = 0; i < 2; i++)
	{
		deltArm[i] = igusParam.offset[i];
	}
	//坐标系转换
	for (int i = 0; i < len; i++)
	{
		posIn[3 * i] = posIn[3 * i] - igusParam.P0[0];
		posIn[3 * i + 1] = posIn[3 * i+1] - igusParam.P0[1];
		posIn[3 * i + 2] = posIn[3 * i+2] - igusParam.P0[2];
	}
	// 正解
	if (igusParam.mode == 0)
	{
		double* pIn = (double*)malloc(len * 3 * sizeof(double));
		double* v = (double*)malloc(len * 3 * sizeof(double));
		TransMotion(posIn, vIn, len, igusParam.mode, pIn, v);
		int res = GetPvtData(pIn, v, tIn, len, igusParam.mode);
		if (res != 0)
		{
			return -1;
		}
	}
	else
	{
		int res = GetPvtData(posIn, vIn, tIn, len, igusParam.mode);
		if (res != 0)
		{
                        return res;
		}
	}
	// 添加
	resLen[0] = resInfo.size() * 7;
	return 0;
}
int TransMotion(double* pIn, double* vIn, int len, int index, double* pout, double* vout)
{
	if (index == 0)
	{
		for (int i = 0; i < len; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				pout[3 * i + j] = (pIn[3 * i + j] - deltArm[0])*sca / 360 + deltArm[1];
				vout[3 * i + j] = vIn[3 * i + j] * sca/360;
			}
		}
	}
	else
	{
		for (int i = 0; i < len; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				pout[3 * i + j] = (pIn[3 * i + j] - deltArm[1]) / sca *360 + deltArm[0];
				vout[3 * i + j] = vIn[3 * i + j] / sca*360;
			}
		}
	}
	return 0;
}
int GetPvtData(double* pIn, double* vIn, double* tIn, int len, int index)
{
	resInfo.clear();
	for (int i = 0; i < len; i++)
	{
		double tempP[3];
		double tempV[3];
		for (int j = 0; j < 3; j++)
		{
			tempP[j] = pIn[3 * i + j];
			tempV[j] = vIn[3 * i + j];
		}
		double pOut[3];
		memset(&pOut, 0, 3 * sizeof(double));
		int res = CalPosition(tempP, pOut, index);
		if (res < 0)
		{
                        return -10;
		}
		double vOut[3];
		memset(&vOut, 0, 3 * sizeof(double));
		res = CalV(pOut, tempP, tempV, vOut, index);
		if (res < 0)
		{
                        return -11;
		}
		if (index == 1)
		{
			TransMotion(pOut, vOut, 1, index, pOut, vOut);
			for (int k = 0; k < 3; k++)
			{
				if (fabs(vOut[k]) > vMax)
				{
                                        return -12;
				}
			}
		}
		ResInfo tempRes;
		tempRes.P1 = -pOut[0];
		tempRes.P2 = -pOut[1];
		tempRes.P3 = -pOut[2];
		tempRes.V1 = -vOut[0];
		tempRes.V2 = -vOut[1];
		tempRes.V3 = -vOut[2];
		tempRes.T = tIn[i];
		resInfo.push_back(tempRes);
	}
	return 0;
}
int CalPosition(double* pIn, double* pOut, int index)
{
	double d1 = armLengthArray[0] - armLengthArray[2];
	double d2 = armLengthArray[1];
	double theta = armLengthArray[3];
	double sqr = sqrt(3);
	double c = cos(theta);
	double s = sin(theta);
	// 正解
	if (index == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			// 超出工作区间判断
			if (pIn[i]<posLim[0] || pIn[i]>posLim[1])
			{
				return -1;
			}
		}
		double xa = -sqr / 3 * d1 + pIn[0] * c;
		double ya = 0;
		double za = -pIn[0] * s;

		double xb = sqr / 6 * d1 - 0.5 * pIn[1] * c;
		double yb = 0.5*(-d1 + sqr*pIn[1] * c);
		double zb = -pIn[1] * s;

		double xc = sqr / 6 * d1 - 0.5*pIn[2] * c;
		double yc = 0.5*(d1 - sqr*pIn[2] * c);
		double zc = -pIn[2] * s;

		double A1 = 2 * (xb - xa);
		double A2 = 2 * (xc - xb);

		double B1 = 2 * (yb - ya);
		double B2 = 2 * (yc - yb);

		double C1 = 2 * (zb - za);
		double C2 = 2 * (zc - zb);

		double D1 = pow(xb, 2) - pow(xa, 2) + pow(yb, 2) - pow(ya, 2) + pow(zb, 2) - pow(za, 2);
		double D2 = pow(xc, 2) - pow(xb, 2) + pow(yc, 2) - pow(yb, 2) + pow(zc, 2) - pow(zb, 2);

		double K = A1 * B2 - A2 * B1;
		if (fabs(K) < error)
		{
			return -1;
		}
		double E1 = (B1 * C2 - B2 * C1) / K;
		double F1 = (B2 * D1 - B1 * D2) / K;

		double E2 = (A2 * C1 - A1 * C2) / K;
		double F2 = (A1 * D2 - A2 * D1) / K;

		double a = pow(E2, 2) + pow(E1, 2) + 1;
		double b = 2 * (E1 * (F1 - xa) + E2 * (F2 - ya) - za);
		double c = pow(xa, 2) + pow(ya, 2) + pow(za, 2) + pow(F1, 2) + pow(F2, 2) - 2 * (xa * F1 + ya * F1) - pow(d2, 2);

		double delt = pow(b, 2) - 4 * a * c;
		if (delt < 0)
		{
			return -1;
		}
		pOut[2] = (-b - sqrt(delt)) / (2 * a);
		pOut[0] = E1 * pOut[2] + F1;
		pOut[1] = E2 * pOut[2] + F2;
	}
	// 逆解
	else
	{
		double temp = pow(pIn[2], 2) - pow(d2, 2);
		double b = 2 * (-(sqr / 3 * d1 + pIn[0]) * c + pIn[2] * s);
		double cc = pow((sqr / 3 * d1 + pIn[0]), 2) + pow(pIn[1], 2) + temp;

		double delt = b*b - 4 * cc;
		if (delt < 0)
		{
			return -1;
		}
		pOut[0] = (-b - sqrt(delt)) / 2;
		// 判断范围
		if ((pOut[0] < posLim[0] && fabs(pOut[0] - posLim[0]) < error))
		{
			pOut[0] = posLim[0];
		}
		if ((pOut[0] > posLim[1] && fabs(pOut[0] - posLim[1]) < error))
		{
			pOut[0] = posLim[1];
		}
		if (pOut[0]<posLim[0] || pOut[0]>posLim[1])
		{
			return -1;
		}
		b = (-sqr / 6 * d1 + pIn[0] - sqr * (0.5 * d1 + pIn[1])) * c + 2 * pIn[2] * s;
		cc = pow((sqr / 6 * d1 - pIn[0]), 2) + pow((0.5 * d1 + pIn[1]), 2) + temp;
		delt = b*b - 4 * cc;
		if (delt < 0)
		{
			return -1;
		}
		pOut[1] = (-b - sqrt(delt)) / 2;
		// 判断范围
		if ((pOut[1] < posLim[0] && fabs(pOut[1] - posLim[0]) < error))
		{
			pOut[1] = posLim[0];
		}
		if ((pOut[1] > posLim[1] && fabs(pOut[1] - posLim[1]) < error))
		{
			pOut[1] = posLim[1];
		}
		if (pOut[1]<posLim[0] || pOut[1]>posLim[1])
		{
			return -1;
		}
		b = (-sqr / 6 * d1 + pIn[0] - sqr * (0.5 * d1 - pIn[1])) * c + 2 * pIn[2] * s;
		cc = pow((sqr / 6 * d1 - pIn[0]), 2) + pow((0.5 * d1 - pIn[1]), 2) + temp;
		delt = b*b - 4 * cc;
		if (delt < 0)
		{
			return -1;
		}
		pOut[2] = (-b - sqrt(delt)) / 2;
		// 判断范围
		if ((pOut[2] < posLim[0] && fabs(pOut[2] - posLim[0]) < error))
		{
			pOut[2] = posLim[0];
		}
		if ((pOut[2] > posLim[1] && fabs(pOut[2] - posLim[1]) < error))
		{
			pOut[2] = posLim[1];
		}
		if (pOut[2]<posLim[0] || pOut[2]>posLim[1])
		{
			return -1;
		}
	}
	return 0;
}
int CalV(double* p1, double* p2, double* vIn, double* vOut, int index)
{
	double d1 = armLengthArray[0] - armLengthArray[2];
	double d2 = armLengthArray[1];
	double theta = armLengthArray[3];
	double sqr = sqrt(3);
	double c = cos(theta);
	double s = sin(theta);
	double A1 = 2 * (p1[0] * c - sqr / 3 * d1 - p2[0]);
	double B1 = -2 * p2[1];
	double C1 = -2 * (p1[0] * s + p2[2]);
	double D1 = 2 * (p1[0] - (sqr / 3 * d1 + p2[0]) * c + p2[2] * s);

	double A2 = -p1[1] * c + sqr / 3 * d1 - 2 * p2[0];
	double B2 = sqr *p1[1] * c - d1 - 2 * p2[1];
	double C2 = -2 * (p1[1] * s + p2[2]);
	double D2 = 2 * p1[1] + (-sqr / 6 * d1 + p2[0] - sqr*(0.5*d1 + p2[1]))*c + 2 * p2[2] * s;

	double A3 = -p1[2] * c + sqr / 3 * d1 - 2 * p2[0];
        double B3 = -(sqr*p1[2] * c - d1 + 2 * p2[1]);
	double C3 = -2 * (p1[2] * s + p2[2]);
	double D3 = 2 * p1[2] + (-sqr / 6 * d1 + p2[0] - sqr*(0.5*d1 - p2[1]))*c + 2 * p2[2] * s;

	if (index == 0)
	{
		D1 = D1 * vIn[0];
		D2 = D2 * vIn[1];
		D3 = D3 * vIn[2];

		double K = A1 * B2 - A2 * B1;
		if (fabs(K) < error)
		{
			return -1;
		}

		double E1 = (B1 * C2 - B2 * C1) / K;
		double F1 = (B2 * D1 - B1 * D2) / K;

		double E2 = (A2 * C1 - A1 * C2) / K;
		double F2 = (A1 * D2 - A2 * D1) / K;

		double K1 = D3 - A3 * F1 - B3 * F2;
		double K2 = C3 + A3 * E1 + B3 * E2;

		if (fabs(K2) < error)
		{
			return -1;
		}
		vOut[2] = K1 / K2;
		vOut[0] = E1 * vOut[2] + F1;
		vOut[1] = E2 * vOut[2] + F2;
	}
	else
	{
		if (fabs(D1) < error || fabs(D2) < error || fabs(D3) < error)
		{
			return -1;
		}

		vOut[0] = (A1 * vIn[0] + B1 * vIn[1] + C1 * vIn[2]) / D1;
		vOut[1] = (A2 * vIn[0] + B2 * vIn[1] + C2 * vIn[2]) / D2;
		vOut[2] = (A3 * vIn[0] + B3 * vIn[1] + C3 * vIn[2]) / D3;
	}
	return 0;
}
