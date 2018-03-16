// DllManager.cpp : 定义 DLL 应用程序的导出函数。


#include "math.h"
#include <vector>

//! only once
#include "param.h"

// 获取轨迹规划的结果的长度
// 输入: 
// 1:pIn 输入的轨迹点集
// 2:length 点的个数
// 3:pStep 插值步长，单位mm
// 4:pMode 轨迹规划方式， 0:线性插值，1:3次多项式差值，2:5次多项式差值,目前只用pMode =0的模式进行轨迹规划
// 输出：
// resLength 
// 1:轨迹规划的结果的长度
MEGA_EXPORT int  GetPvtLen(double* pIn, int length, double pStep, int pMode, int* resLength)
{
	posIn = pIn;
	step = pStep;
	mode = pMode;

    //! trim the dot
    trimPoints( pIn, length );

	CallRobot(length, resLength);
    return 0;
}
// 获取轨迹规划的结果
MEGA_EXPORT int  GetPvtInfo(double* outPut,int len)
{	
    Q_ASSERT( NULL != outPut );
    Q_ASSERT( NULL != resOut );

    memcpy(outPut, resOut, len * sizeof(double) );

	if (NULL != resOut)
    { delete[]resOut; }

    return 0;
}

//! x,y,z,end,t,*,*
void CallRobot(int len, int* resLength)
{
	// 计算点的个数
	int pcount = 0;
	std::vector<ResInfo> resInfo;
	ResInfo res;
	for (int i = 0; i < len-1; i++)
	{
		if (posIn[i * 6 + 5] == 0)
		{
			res.Px = posIn[i * 6];
			res.Py = posIn[i * 6+1];
			res.Pz = posIn[i * 6+2];
			res.Vx = 0;
			res.Vy = 0;
			res.Vz = 0;
			res.End = posIn[i * 6+3];
			res.T = posIn[i * 6+4];
			resInfo.push_back(res);
			continue;
		}
		for (int j = 0; j < 2; j++)
		{
			pos[j].x = posIn[6 * (i+j)];
			pos[j].y = posIn[6 * (i + j) + 1];
			pos[j].z = posIn[6 * (i + j) + 2];
			pos[j].t = posIn[6 * (i + j) + 4];
			pos[j].m = posIn[6 * (i + j) + 5];
		}
		// 计算两点间的空间距离
		deltDis = sqrt(pow(pos[1].x - pos[0].x, 2) + pow(pos[1].y - pos[0].y, 2) + pow(pos[1].z - pos[0].z, 2));
		// 计算空间距离在各轴上的分量
		double delt[3] = { (pos[1].x - pos[0].x) / deltDis ,(pos[1].y - pos[0].y) / deltDis ,(pos[1].z - pos[0].z) / deltDis };
		// 生成轨迹
		CalStepTraj();
        int index = 0;
        if(i >0)
        {
            index =1;
        }
        for (int k = index; k < tempLen; k++)
		{
            res.Px = tempP[k] * delt[0] + posIn[ i * 6 + 0 ];
            res.Py = tempP[k] * delt[1] + posIn[ i * 6 + 1 ];
            res.Pz = tempP[k] * delt[2] + posIn[ i * 6 + 2 ];
			res.Vx = tempV[k] * delt[0];
			res.Vy = tempV[k] * delt[1];
			res.Vz = tempV[k] * delt[2];
			if (k < tempLen - 1)
			{
				res.End = posIn[6*i + 3];
			}
			else
			{
				res.End = posIn[6 * (i+1) + 3];
			}
			res.T = tempT[k];
			resInfo.push_back(res);
		}
	}
	int size = resInfo.size();
	resOut = new double[size * 7];
	memset(resOut, 0, size * 7 * sizeof(double));
	for (int m = 0; m < size; m++)
	{
		resOut[m * 7] = resInfo[m].Px;
		resOut[m * 7 + 1] = resInfo[m].Py;
		resOut[m * 7 + 2] = resInfo[m].Pz;
		resOut[m * 7 + 3] = resInfo[m].Vx;
        resOut[m * 7 + 4] = resInfo[m].Vy;
        resOut[m * 7 + 5] = resInfo[m].Vz;
		resOut[m * 7 + 6] = resInfo[m].T;
	}
	resLength[0] = size * 7;
}

void CalStepTraj()
{
	// 生成3段曲线函数
	int col = 2 * mode + 3;
	double** constParam = new double*[6];
	for (int c = 0; c < 6; c++)
	{
		constParam[c] = new double[col];
	}
	double** tempPvt = new double*[3];
	for (int cc = 0; cc < 3; cc++)
	{
		tempPvt[cc] = new double[4];
	}
	BuildCurve(tempPvt, constParam);
	// 计算第一段的pvt
	CalPvt(constParam, tempPvt, 1);
	// 计算第二段的pvt
	CalPvt(constParam, tempPvt, 2);
	// 计算第三段的pvt
	CalPvt(constParam, tempPvt, 3);
	if (n2 > 2)
	{
		tempLen = p1Len + p2Len + p3Len - 2;
		tempP = new double[tempLen];
		tempV = new double[tempLen];
		tempT = new double[tempLen];
		for (int i = 0; i < tempLen; i++)
		{
			if (i < p1Len)
			{
				tempP[i] = P1[i];
				tempV[i] = V1[i];
				tempT[i] = T1[i];
			}
			else if (i >= p1Len &&i< p1Len + p2Len - 1)
			{
				tempP[i] = P2[i - p1Len + 1];
				tempV[i] = V2[i - p1Len + 1];
				tempT[i] = T2[i - p1Len + 1];
			}
			else
			{
				tempP[i] = P3[i - p1Len - p2Len + 2];
				tempV[i] = V3[i - p1Len - p2Len + 2];
				tempT[i] = T3[i - p1Len - p2Len + 2];
			}
		}
	}
	else
	{
		tempLen = p1Len + p3Len - 1;
		tempP = new double[tempLen];
		tempV = new double[tempLen];
		tempT = new double[tempLen];
		for (int i = 0; i < tempLen; i++)
		{
			if (i < p1Len)
			{
				tempP[i] = P1[i];
				tempV[i] = V1[i];
				tempT[i] = T1[i];
			}
			else
			{
				tempP[i] = P3[i - p1Len + 1];
				tempV[i] = V3[i - p1Len + 1];
				tempT[i] = T3[i - p1Len + 1];
			}
		}
	}
}

void BuildCurve(double** tempPvt, double** tempParam)
{
	int col = 2 * mode + 3;
	// 初始时间
	double t0 = pos[0].t;
	// 截止时间
	double t1 = pos[1].t;
	// 加减速的时间
	double deltT = sca * (t1 - t0);
	deltTime = t1 - t0;
	// 速度最大值
	double vMax = deltDis / (deltTime - deltT);
	double P1 = 0;
	double P4 = deltDis;
	tempPvt[0][0] = P1;
	tempPvt[1][0] = 0;
	tempPvt[2][0] = t0;
	double pt = 0;
	pt = t0;
	// 生成第一段曲线系数
	double** const1 = BulidStepFunction(0, pt, t0, t0 + deltT, 0, vMax);
	double* input1 = new double[col];
	double* input2 = new double[col];
	for (int i = 0; i < col; i++)
	{
		input1[i] = const1[0][i];
		input2[i] = const1[1][i];
	}
	double pTime = t0 + deltT;
	tempPvt[0][1] = CallPolynomial(input2, col, pTime);
	tempPvt[1][1] = CallPolynomial(input1, col, pTime);
	tempPvt[2][1] = pTime;
	// 生成第三段减速运动
	pt = t1;
	double** const3 = BulidStepFunction(P4, pt, t1 - deltT, t1, vMax, 0);
	input1 = new double[col];
	input2 = new double[col];
	for (int j = 0; j < col; j++)
	{
		input1[j] = const3[0][j];
		input2[j] = const3[1][j];
	}
	pTime = t1 - deltT;
	tempPvt[0][2] = CallPolynomial(input2, col, pTime);
	tempPvt[1][2] = CallPolynomial(input1, col, pTime);
	tempPvt[2][2] = pTime;
	// 第二段匀速直线运动
	double** temp = BuildLinear(tempPvt[0][1], tempPvt[0][2], t0 + deltT, t1 - deltT);
	double** const2 = new double*[2];
	for (int k = 0; k < 2; k++)
	{
		const2[k] = new double[col];
	}
	for (int k = 0; k < 2; k++)
	{
		const2[k][0] = 0;
		const2[0][col - k - 1] = temp[0][3 - k - 2];
		const2[1][col - k - 1] = temp[1][3 - k - 2];
	}
	tempPvt[0][3] = P1 + deltDis;
	tempPvt[1][3] = 0;
	tempPvt[2][3] = t1;
	for (int m = 0; m < col; m++)
	{
		tempParam[0][m] = const1[0][m];
		tempParam[1][m] = const1[1][m];
		tempParam[2][m] = const2[0][m];
		tempParam[3][m] = const2[1][m];
		tempParam[4][m] = const3[0][m];
		tempParam[5][m] = const3[1][m];
	}
}
double** BulidStepFunction(double pStart, double tIn, double tStart, double tEnd, double vStart, double vEnd)
{
	int col = 2 * mode + 3;
	double** res = new double*[2];
	for (int i = 0; i < 2; i++)
	{
		res[i] = new double[col];
	}
	double* temp;
	double p0 = 0;
	double k = 0;
	double a = 0;
	double b = 0;
	double c = 0;
	double d = 0;
	double e = 0;
	double f = 0;
	switch (mode)
	{
	case 0:
		k = 1 / (tEnd - tStart);
		a = k*(vEnd - vStart);
		b = k*(vStart*tEnd - vEnd*tStart);
		res[0][0] = 0;
		res[0][1] = a;
		res[0][2] = b;
		res[1][0] = a / 2;
		res[1][1] = b;
		temp = new double[3]{ a / 2, b, 0 };
		p0 = pStart - CallPolynomial(temp, 3, tIn);
		res[1][2] = p0;
		break;
	case 1:
		k = (vEnd - vStart) / pow(tEnd - tStart, 3);
		a = -2 * k;
		b = 3 * k*(tStart + tEnd);
		c = -6 * k*tStart*tEnd;
		d = k*pow(tStart, 2)*(3 * tEnd - tStart) + vStart;
		res[0][0] = 0;
		res[0][1] = a;
		res[0][2] = b;
		res[0][3] = c;
		res[0][4] = d;
		res[1][0] = a / 4;
		res[1][1] = b / 3;
		res[1][2] = c / 2;
		res[1][3] = d;
		temp = new double[5]{ a / 4, b / 3, c / 2, d, 0 };
		p0 = pStart - CallPolynomial(temp, 5, tStart);
		res[1][4] = p0;
		break;
	case 2:
		k = (vEnd - vStart) / pow(tEnd - tStart, 5);
		a = 6 * k;
		b = -15 * k * (tStart + tEnd);
		c = 10 * k * (pow(tStart, 2) + 4 * tStart * tEnd + pow(tEnd, 2));
		d = -30 * k*tStart*tEnd*(tStart + tEnd);
		e = 30 * k*pow(tStart + tEnd, 2);
		f = -k * pow(tStart, 3) * (pow(tStart, 2) - 5 * tStart*tEnd + 10 * pow(tEnd, 2)) + vStart;
		res[0][0] = 0;
		res[0][1] = a;
		res[0][2] = b;
		res[0][3] = c;
		res[0][4] = d;
		res[0][5] = e;
		res[0][6] = f;
		res[1][0] = a / 6;
		res[1][1] = b / 5;
		res[1][2] = c / 4;
		res[1][3] = d / 3;
		res[1][4] = e / 2;
		res[1][5] = f;
		temp = new double[7]{ a / 6, b / 5, c / 4, d / 3, e / 2, f, 0 };
		p0 = pStart - CallPolynomial(temp, 7, tStart);
		res[1][6] = p0;
		break;
	}
	return res;
}

double CallPolynomial(double* input, int inputLen, double t)
{
	double* res = new double[2 * mode + 3];
	int len = ReviseConst(input, inputLen, res);
	double* b = new double[len];
	b[0] = res[0];
	for (int j = 1; j < len; j++)
	{
		b[j] = res[j] + t*b[j - 1];
	}
	double fx = b[len - 1];
	return fx;
}

double* CallPolynomial(double* input, int inputLen, double* t, int tLen)
{
	double* res = new double[2 * mode + 3];
	int len = ReviseConst(input, inputLen, res);
	double* b = new double[len];
	double* fx = new double[tLen];
	b[0] = res[0];
	for (int i = 0; i < tLen; i++)
	{
		for (int j = 1; j < len; j++)
		{
			b[j] = res[j] + t[i] * b[j - 1];
		}
		fx[i] = b[len - 1];
        if ( local_ABS(fx[i]) < error)
		{
			fx[i] = 0;
		}
	}
	return fx;
}

int ReviseConst(double* input, int inputLen, double* res)
{
	int flg = 0;
	for (int i = 0; i < inputLen; i++)
	{
        if ( local_ABS(input[i]) > error )
		{
			flg = i;
			break;
		}
	}
	for (int j = 0; j < inputLen - flg; j++)
	{
		res[j] = input[j + flg];
	}
	return inputLen - flg;
}

int ReviseConst(double** input, double* res, int index)
{
	int len = 0;
	int count = 2 * mode + 3;
    if (local_ABS(input[index - 1][0]) < error)
	{
		len = count - 1;
		for (int i = 0; i < count - 1; i++)
		{
			res[i] = input[index - 1][i + 1];
		}
	}
	else
	{
		len = count;
		for (int i = 0; i < count; i++)
		{
			res[i] = input[index - 1][i];
		}
	}
	return len;
}

double** BuildLinear(double pStart, double pEnd, double tStart, double tEnd)
{
	double** res = new double*[2];
	for (int i = 0; i < 2; i++)
	{
		res[i] = new double[2];
	}
	res[0][0] = 0;
	res[0][1] = (pEnd - pStart) / (tEnd - tStart);
	res[1][0] = res[0][1];
	res[1][1] = pStart - res[0][1] * tStart;
	return res;
}
void CalPvt(double** param, double** pvt, int index)
{
	double* pConst1 = new double[3]{ 0,0,0 };
	int pConst1Len = 0;
	double* vConst1 = new double[3]{ 0,0,0 };
	int vConst1Len = 0;
	double* time1;
	double* pos1;
	double* pConst2 = new double[3]{ 0,0,0 };
	int pConst2Len = 0;
	double* vConst2 = new double[3]{ 0,0,0 };
	int vConst2Len = 0;
	double* time2;
	double* pos2;
	double* pConst3 = new double[3]{ 0,0,0 };
	int pConst3Len = 0;
	double* vConst3 = new double[3]{ 0,0,0 };
	int vConst3Len = 0;
	double* time3;
	double* pos3;
	switch (index)
	{
	case 1:
		pConst1Len = ReviseConst(param, pConst1, 2);
		vConst1Len = ReviseConst(param, vConst1, 1);
		pos1 = new double[2]{ pvt[0][0], pvt[0][1] };
		time1 = new double[2]{ pvt[2][0], pvt[2][1] };
		GetPvtArray(pConst1, pConst1Len, vConst1, vConst1Len, pos1, time1, 1);
		break;
	case 2:
		pConst2Len = ReviseConst(param, pConst2, 4);
		vConst2Len = ReviseConst(param, vConst2, 3);
		pos2 = new double[2]{ pvt[0][1], pvt[0][2] };
		time2 = new double[2]{ pvt[2][1], pvt[2][2] };
        if (local_ABS(pos2[1] - pos2[0]) > step / 2)
		{
			GetPvtArray(pConst2, pConst2Len, vConst2, vConst2Len, pos2, time2, 2);
			n2 = p2Len;
		}
		break;
	case 3:
		pConst3Len = ReviseConst(param, pConst3, 6);
		vConst3Len = ReviseConst(param, vConst3, 5);
		pos3 = new double[2]{ pvt[0][2], pvt[0][3] };
		time3 = new double[2]{ pvt[2][2], pvt[2][3] };
		GetPvtArray(pConst3, pConst3Len, vConst3, vConst3Len, pos3, time3, 3);
		break;
	}
}

void GetPvtArray(double* pConst, int plen, double* vConst, int vlen, double* pos, double* time, int index)
{
	int len = 0;
	double* res = DivRegion(pConst, plen, time, &len);
	double* x = NULL;
	int xlen;
	double* y = NULL;
	double* x1;
	int x1len = 0;
	double* y1;
	double* x2;
	int x2len = 0;
	double* y2;
	double p1;
	double p2;
	double p3;
	int flg = 0;
	if (len == 4)
	{
		p1 = pos[0];
		p2 = CallPolynomial(pConst, plen, res[1]);
		p3 = pos[1];
		double* xlim = new double[2]{ res[0], res[1] };
		int ylen = 0;
		double* yTemp = DisRegion(p1, p2, &flg, &ylen);
		if (flg == 1)
		{
			x1 = xlim;
			y1 = new double[2]{ p1, p2 };
		}
		else
		{
			if (flg == 2)
			{
				int xlen;
				double* xtemp = SolAmountSolution(pConst, plen, xlim, yTemp, ylen);
				x1 = new double[ylen + 2];
				x1len = ylen + 2;
				x1[0] = xlim[0];
				x1[ylen - 1] = xlim[1];
				for (int i = 0; i < ylen; i++)
				{
					x1[i + 1] = xtemp[i];
				}
				y1 = new double[ylen + 2];
				y1[0] = p1;
				y1[ylen + 1] = p2;
				for (int j = 0; j < ylen; j++)
				{
					y1[j + 1] = yTemp[j];
				}
			}
			else
			{
				x1 = NULL;
				x1len = 0;
				flg = 1;
			}

		}
		if (flg > 0)
		{
			xlim = new double[2]{ res[2], res[3] };
			yTemp = DisRegion(p1, p2, &flg, &ylen);
			if (flg == 1)
			{
				x2 = new double[1]{ xlim[1] };
				y2 = new double[1]{ p3 };
			}
			else
			{
				if (flg == 2)
				{
					double* xtemp = SolAmountSolution(pConst, plen, xlim, yTemp, ylen);
					x2 = new double[ylen + 1];
					x2len = ylen + 1;
					x2[ylen - 1] = xlim[1];
					for (int i = 0; i < ylen; i++)
					{
						x2[i] = xtemp[i];
					}
					y2 = new double[ylen + 1];
					y2[ylen] = p3;
					for (int j = 0; j < ylen; j++)
					{
						y2[j] = yTemp[j];
					}
				}
				else
				{
					x2 = NULL;
					x2len = 0;
					flg = 1;
				}
			}
			xlen = x1len + x2len;
			x = new double[xlen];
			for (int m = 0; m < xlen; m++)
			{
				if (m < x1len)
				{
					x[m] = x1[m];
				}
				else
				{
					x[m] = x2[m - x2len];
				}
			}
		}
	}
	else
	{
		p1 = pos[0];
		p2 = pos[1];
		int ylen = 0;
		double* yTemp = DisRegion(p1, p2, &flg, &ylen);
		if (flg == 1)
		{
			x = time;
			y = pos;
		}
		else
		{
			if (flg == 2)
			{
				double* xtemp = SolAmountSolution(pConst, plen, time, yTemp, ylen);
				x = new double[ylen + 2];
				xlen = ylen + 2;
				x[0] = time[0];
				x[ylen + 1] = time[1];
				for (int i = 0; i < ylen; i++)
				{
					x[i + 1] = xtemp[i];
				}

				y = new double[ylen + 2];
				y[0] = p1;
				y[ylen + 1] = p2;
				for (int j = 1; j < ylen + 1; j++)
				{
					y[j] = yTemp[j - 1];
				}
			}
			else
			{
				x = NULL;
				flg = 1;
			}
		}
	}
	if (index == 1)
	{
		if (flg > 0)
		{
			if (x == NULL)
			{
				P1 = pos;
				T1 = time;
				V1 = CallPolynomial(vConst, vlen, time, 2);
				p1Len = 1;
			}
			else
			{
				P1 = y;
				V1 = CallPolynomial(vConst, vlen, x, xlen);
				T1 = x;
				p1Len = xlen;
			}
		}
		else
		{
			P1 = 0;
			V1 = 0;
			T1 = 0;
			p1Len = 1;
		}
	}
	else if (index == 2)
	{
		if (flg > 0)
		{
			P2 = y;
			V2 = CallPolynomial(vConst, vlen, x, xlen);
			T2 = x;
			p2Len = xlen;
		}
		else
		{
			P2 = new double[1];
			V2 = new double[1];
			T2 = new double[1];
			p2Len = 1;
		}
	}
	else if (index == 3)
	{
		if (flg > 0)
		{
			if (x == NULL)
			{
				P3 = new double[2]{ pos[0], pos[1] };
				T3 = new double[2]{ time[0], time[1] };
				V3 = CallPolynomial(vConst, vlen, T3, 2);
				p3Len = 2;
			}
			else
			{
				P3 = y;
				V3 = CallPolynomial(vConst, vlen, x, xlen);
				T3 = x;
				p3Len = xlen;
			}
		}
		else
		{
			P3 = new double[1];
			V3 = new double[1];
			T3 = new double[1];
			p3Len = 1;
		}
	}
}

double* DivRegion(double* pIn, int plen, double* tIn, int* len)
{
	double* dConst = new double[plen - 1];
	double* res;
	int dConstlen = plen - 1;
	for (int i = 0; i < plen - 1; i++)
	{
		dConst[i] = (plen - 1 - i)*pIn[i];
	}
    double delt1 = local_ABS(CallPolynomial(dConst, plen - 1, tIn[0]));
    double delt2 = local_ABS(CallPolynomial(dConst, plen - 1, tIn[1]));
	// 边界导数为0，不需要划分区间
	if (delt1 < error || delt2 < error)
	{
		len[0] = 2;
		res = tIn;
	}
	double sol = SolveSingalSolution(dConst, plen - 1, tIn);
    if (local_ABS(sol - tIn[0])<error)
	{
		len[0] = 2;
		res = tIn;
	}
	else
	{
        delt1 = local_ABS(tIn[0] - sol);
        delt2 = local_ABS(tIn[1] - sol);
		if (delt1 < error || delt2 < error)
		{
			len[0] = 2;
			res = tIn;
		}
		else
		{
			len[0] = 4;
			res = new double[4]{ tIn[0], sol, sol, tIn[1] };
		}
	}
	return res;
}

double SolveSingalSolution(double* p1, int inputLen, double* t1)
{
	double* p = new double[2 * mode + 3];
	int plen = ReviseConst(p1, inputLen, p);
	double* f = CallPolynomial(p, plen, t1, 2);
	double sol = 0;
    if (local_ABS(f[0]) < error)
	{
		sol = t1[0];
	}
	else
	{
        if (local_ABS(f[1]) < error)
		{
			sol = t1[1];
		}
		else
		{
			switch (plen)
			{
			case 1:
				sol = t1[0];
				break;
			case 2:
				sol = SolveLinearFunction(p1, t1);
				break;
			case 3:
				sol = SolveQuadraticFunction(p1, t1);
				break;
			}
		}
	}
	return sol;
}

double SolveLinearFunction(double* pIn, double* tIn)
{
	double res;
    if (local_ABS(pIn[0]) < error)
	{
		res = 0;
	}
	else
	{
		res = -pIn[1] / pIn[0];
	}
	return ChechRegion(tIn, res);
}

double SolveQuadraticFunction(double* pIn, double* tIn)
{
	// 二次项系数
	double a = pIn[0];
	// 一次项系数
	double b = pIn[1];
	// 常数项系数
	double c = pIn[2];
	double res = 0;
    if (local_ABS(a) < error || local_ABS(a / b) < error)
	{
		res = SolveLinearFunction(pIn, tIn);
	}
	else
	{
		double delt = pow(b, 2) - 4 * a*c;
		if (delt < 0)
		{
			res = tIn[0];
		}
		if (sqrt(delt) < error)
		{
			res = -b / (2 * a);
		}
		else
		{
			double* x = new double[2]{ (-b + sqrt(delt)) / (2 * a), (-b - sqrt(delt)) / (2 * a) };
            if (local_ABS(pIn[1] - pIn[0]) < error)
			{
				for (int i = 0; i < 2; i++)
				{
                    if (local_ABS(tIn[0] - x[i]) < error)
					{
						res = tIn[0];
					}
				}
			}
			else
			{
				int count = 0;
				int tick = 0;
				for (int i = 0; i < 2; i++)
				{
					if (x[i] >= tIn[0] && x[i] <= tIn[1])
					{
						count++;
						tick = i;
					}
				}
				if (count == 0 || count == 2)
				{
					res = tIn[0];
				}
				if (count == 1)
				{
					res = x[tick];
				}
			}

		}
	}
	return res;
}
double ChechRegion(double* tIn, double res)
{
	//区间为一点
    if (local_ABS(tIn[1] - tIn[0]) < error)
	{
        if (local_ABS(tIn[0] - res) < error)
		{
			return res;
		}
	}
	else
	{
		if (res >= tIn[0] - error && res <= tIn[1] + error)
		{
			return res;
		}
		else
		{
			return tIn[0];
		}
	}
	return res;
}

double* DisRegion(double p1, double p2, int* flg, int* len)
{
	double delt = p2 - p1;
	double* res;
    if (local_ABS(delt) < step / 2)
	{
		flg[0] = 0;
		len[0] = 2;
		return new double[2]{ p1, p2 };
	}
	else
	{
        if (local_ABS(delt) < 3 * step / 2)
		{
			flg[0] = 1;
			len[0] = 2;
			return new double[2]{ p1, p2 };
		}
		else
		{
			flg[0] = 2;
			int n = (int)(delt / step);
            double err = local_ABS(delt - n*step);
			if (n > 1)
			{
				res = new double[n - 1];
				for (int i = 1; i < n - 1; i++)
				{
					res[i] = p1 + step*(i + 1);
				}
				if (err >= step / 2)
				{
					res[n - 1 - 1] = p2 - step;
				}
				res[0] = p1 + step;
				len[0] = n - 1;
			}
			else
			{
				res = new double[n];
				res[0] = p1 + step;
				len[0] = n;
			}

			return res;
		}
	}
}

double* SolAmountSolution(double* pIn, int plen, double* xIn, double* yIn, int ylen)
{
	double* temp = new double[plen];
	for (int j = 0; j < plen; j++)
	{
		temp[j] = pIn[j];
	}
	double* tempT = new double[2];
	for (int m = 0; m < 2; m++)
	{
		tempT[m] = xIn[m];
	}
	double* res = new double[ylen];
	for (int i = 0; i < ylen; i++)
	{
		temp[plen - 1] = pIn[plen - 1] - yIn[i];
		res[i] = SolveSingalSolution(temp, plen, tempT);
		tempT[0] = res[i];
	}
	return res;
}
