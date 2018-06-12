#include "params.h"
#include "math.h"
#include "string.h"
#include "AddPointParams.h"
/*正解求末端位置*/
/*入参：armLength 各轴长度(主动杆，从动杆，轮间距，平台间距)，deltAngles 外部关节角度转机构0角度的差值
	p:机构0位对应的末端点位置，angles：输入的关节角度,len:输入需要求解点的个数
*/
/*输出参数：res：正解求得的末端位置*/
extern "C" __declspec(dllexport) int GetDeltEndPosition(double* armLength, double* deltAngles,double* p, double* angles,int len, double* result)
{
	posIn = (double*)malloc(len*2*sizeof(double));	
	memset(posIn, 0, len * 2 * sizeof(double));
	double posOut[8];
	double A[8];
	double B[2];	
	// 输入的关节角度转换为模型的角度
	TansAngles(angles, deltAngles, len,0);
	for (int i = 0; i < len; i++)
	{
		memset(&A, 0, 8 * sizeof(double));
		memset(&B, 0, 2 * sizeof(double));
		memset(&posOut, 0, 8 * sizeof(double));
		double s0 = sin(posIn[2*i]);
		double c0 = cos(posIn[2*i]);
		double s1 = sin(posIn[2 * i+1]);
		double c1 = cos(posIn[2 * i+1]);

		A[0] = 1;
		A[1] = 1;
		A[2] = -2 * (armLength[0] * c0 + armLength[2]);
		A[3] = -2 * armLength[0] * s0;

		A[4] = 1;
		A[5] = 1;
		A[6] = -2 * (armLength[0] * c1 - armLength[2]);
		A[7] = -2 * armLength[0] * s1;

		B[0] = -armLength[2] * (2 * armLength[0] * c0 + armLength[2]) + pow(armLength[1], 2) - pow(armLength[0], 2);
		B[1] = -armLength[2] * (-2 * armLength[0] * c1 + armLength[2]) + pow(armLength[1], 2) - pow(armLength[0], 2);

		// 求解
		double* outPut = (double*)malloc(4 * sizeof(double));
		memset(outPut, 0, 4 * sizeof(double));
		int temp = GetAnswer(A, B, outPut);
		for (int j = 0; j< 2; j++)
		{
			for (int k = 0; k< 2; k++)
			{
				double flg = pow(-1, k);
				double c2i = (outPut[k] - flg * armLength[2] - armLength[0] * cos(posIn[2*i+k])) / armLength[1];
				double s2i = (outPut[k + 2] - armLength[2] * sin(posIn[2 * i + k])) / armLength[1];
				posOut[4 * j + 2 + k] = CheckInputAngle(atan2(s2i, c2i));
			}
			posOut[4 * j] = outPut[j];
			posOut[4 * j + 1] = outPut[j + 2];
		}
		double tempRes[2];
		memset(&tempRes, 0, 2 * sizeof(double));
		// 筛选最优解
		SelPos(posOut, p, tempRes);
		if (fabs(tempRes[0]) < error)
		{
			tempRes[0] = 0;
		}
		if (fabs(tempRes[1]) < error)
		{
			tempRes[1] = 0;
		}
		result[2 * i] = tempRes[0]-p[0];
		result[2 * i+1] = tempRes[1]-p[1];
		free(outPut);
	}	
	free(posIn);
	return 0;
}
/*空间坐标逆解为关节角度*/
/*入参：armLength 各轴长度(主动杆，从动杆，轮间距，平台间距)，deltPosition,外部点转机构空间点的差值，deltAngles 外部关节角度转机构0角度的差值
posLast:机构0位对应的关节角度，posIn：输入的末端点位置,vIn：输入的末端点速度，tIn：时间，len:输入需要求解点的个数
*/
/*输出参数：res：逆解求得的关节角度集合*/
extern "C" __declspec(dllexport) int  GetDeltArmLen(double* armLength, double* deltPosition, double*deltAngles, double* posLast, double* pIn, double* vIn, double* tIn, int len, int* resCount)
{
	posIn = (double*)malloc(len * 3 * sizeof(double));
	deltAngleArray = deltAngles;
	// 输入的空间坐标点转换为机构的坐标点
	TansAngles(pIn, deltPosition, len, 1);
	double* pTemp = (double*)malloc(len * 4 * sizeof(double));
	memset(pTemp, 0, len * 4 * sizeof(double));
	int flg = CalPosition(armLength, len, pTemp);
	if (flg != 0)
	{
		return -1;
	}
	double* vTemp = (double*)malloc(len * 4 * sizeof(double));;
	flg = CalVelocity(armLength, pTemp, vIn, len, vTemp);
	resInfo.clear();
	for (int i = 0; i < len; i++)
	{
		ResInfo info;
		info.P1 = pTemp[4 * i];
		info.P2 = pTemp[4 * i + 1];
		info.V1 = vTemp[4 * i];
		info.V2 = vTemp[4 * i + 1];
		info.T = tIn[i];
		resInfo.push_back(info);
	}
	resInfo = AddZeroPoint(resInfo);
	int size = resInfo.size();	
	resCount[0] = size;
	free(posIn);
	free(pTemp);
	free(vTemp);
	return 0;
}
extern "C" __declspec(dllexport) int  GetDeltArmPosition(double* res)
{	
	int count = resInfo.size();
	for (int m = 0; m < count; m++)
	{	
		if (fabs(resInfo[m].P2) < ERROR)
		{
			resInfo[m].P2 = 0;
		}
		if (resInfo[m].P2 < 0)
		{
			resInfo[m].P2 = deltAngleArray[0] * PI / 180 - (PI + resInfo[m].P2);
		}
		else
		{
			resInfo[m].P2 = deltAngleArray[0] * PI / 180 + (PI - resInfo[m].P2);
		}
		res[m * 5] =resInfo[m].P2 * 180 / PI;
		if (fabs(resInfo[m].P1) < ERROR)
		{
			resInfo[m].P1 = 0;
		}
		res[m * 5 + 1] = deltAngleArray[1] - resInfo[m].P1 * 180 / PI;

		if (fabs(resInfo[m].V2) < ERROR)
		{
			resInfo[m].V2 = 0;
		}
                res[m * 5 + 2] = -resInfo[m].V2 * 180 / PI;     //! \note invert by coord
		if (fabs(resInfo[m].V1) < ERROR)
		{
			resInfo[m].V1 = 0;
		}
                res[m * 5 + 3] = -resInfo[m].V1 * 180 / PI;
		res[m * 5+ 4] = resInfo[m].T;
	}
	return 0;
}
int CalPosition(double*armLength, int plen, double* res)
{
	for (int i = 0; i < plen; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			int flg = pow(-1, j);
			double temp1 = -2 * armLength[0] * posIn[3 * i + 2];
			double temp2 = -2 * armLength[0] * (posIn[3 * i] - flg * armLength[2]);
			double temp3 = pow(posIn[3 * i], 2) + pow(posIn[3 * i + 2], 2) + pow(armLength[0], 2) -
				pow(armLength[1], 2) + pow(armLength[2], 2) - 2 * flg * armLength[2] * posIn[3 * i];
			double delt = pow(temp1, 2) + pow(temp2, 2) - pow(temp3, 2);
			// 判断是否有解
			if (delt < 0)
			{
				return -1;
			}
			delt = sqrt(delt);
			res[4*i+j] = 2 * atan((-temp1 + flg * delt) / (temp3 - temp2));
			if (fabs(res[4 * i + j]) < ERROR)
			{
				res[4 * i + j] = 0;
			}
			double s1 = sin(res[4 * i + j]);
			double c1 = cos(res[4 * i + j]);
			double c2 = (posIn[3 * i] - flg * armLength[2] - armLength[0] * c1) / armLength[1];
			double s2 = (posIn[3 * i +2] - armLength[0] * s1) / armLength[1];
			res[4 * i + 2 + j] = CheckInputAngle(atan2(s2, c2));
		}
	}
	return 0;
}
int CalVelocity(double*armLength,double* pIn,double* vIn, int len, double* res)
{
	for (int i = 0; i < len; i++)
	{
		// 速度雅克比矩阵
		double J[16];
		memset(&J, 0, 16 * sizeof(double));
		double s11 = sin(pIn[4 * i]);
		double c11 = cos(pIn[4 * i]);
		double s12 = sin(pIn[4 * i + 1]);
		double c12 = cos(pIn[4 * i + 1]);
		double s21 = sin(pIn[4 * i + 2]);
		double c21 = cos(pIn[4 * i + 2]);
		double s22 = sin(pIn[4 * i + 3]);
		double c22 = cos(pIn[4 * i + 3]);
		double s11c21 = sin(pIn[4 * i] - pIn[4 * i + 2]);
		double s12c22 = sin(pIn[4 * i + 1] - pIn[4 * i + 3]);

		//速度雅克比矩阵
		J[0] = -c21 / (armLength[0] * s11c21);
		J[1] = -s21 / (armLength[0] * s11c21);
		J[4] = -c22 / (armLength[0] * s12c22);
		J[5] = -s22 / (armLength[0] * s12c22);

		J[10] = c11 / (armLength[1] * s11c21);
		J[11] = s11 / (armLength[1] * s11c21);
		J[14] = c12 / (armLength[1] * s12c22);
		J[15] = s12 / (armLength[1] * s12c22);
		double* temp = (double*)malloc(4 * sizeof(double));
		memset(temp, 0, 4 * sizeof(double));
		double vtemp[2];
		vtemp[0] = vIn[3 * i];
		vtemp[1] = vIn[3 * i+2];
		MatrixMult(J, vtemp, temp);
		res[4 * i] = temp[0];
		res[4 * i + 1] = temp[1];
		res[4 * i + 2] = temp[2];
		res[4 * i + 3] = temp[3];
		free(temp);
	}
	return 0;
}
void MatrixMult(double* input1, double* input2,double* res)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res[i] += input1[4*i+j] * input2[j % 2];
		}
	}
}
void TansAngles(double* input, double* delt, int len,int mode)
{
	//正解
	if (mode == 0)
	{
		for (int i = 0; i < len; i++)
		{

			posIn[2 * i] = (delt[1] - input[2 * i + 1])*PI / 180;
			double temp = delt[0] - input[2 * i];
			if (temp > 180)
			{
				posIn[2 * i + 1] = (temp -360) * PI / 180;
			}
			else
			{
				posIn[2 * i + 1] = temp * PI / 180;
			}			
		}
	}
	// 逆解
	else
	{
		for (int i = 0; i < len; i++)
		{
			posIn[3 * i] = input[3 * i] - delt[0];
			posIn[3 * i +1] = input[3 * i+1];
			posIn[3 * i + 2] = input[3 * i + 2] - delt[1];
		}
	}
	
}
int GetAnswer(double* input1, double* input2,double * outPut)
{
	int len = 0;
	// 无解
	if (fabs(input1[0] - input1[1]) > ERROR || fabs(input1[4] - input1[5]) > ERROR)
	{
		return len;
	}
	double delt = input1[3] - input1[7];
	if (fabs(delt) < ERROR)
	{
		delt = input1[2] - input1[6];
		if (fabs(delt) < ERROR)
		{
			return len;
		}
		else
		{
			outPut[0] = (input2[0] - input2[1]) / delt;
			outPut[1] = (input2[0] - input2[1]) / delt;
			double D = input1[1];
			double EE = input1[3];
			double F = -input2[0] + pow(outPut[0], 2) + input1[2] * outPut[0];
			delt = pow(EE, 2) - 4 * D*F;
			if (delt < 0)
			{
				return -1;
			}
			outPut[2] = (-EE + pow(delt, 0.5)) / (2 * D);
			outPut[3] = (-EE - pow(delt, 0.5)) / (2 * D);
		}
	}
	else
	{
		double B = (input2[0] - input2[1]) / delt;
		double C = (input1[6] - input1[2]) / delt;

		double D = pow(C, 2) + 1;
		double EE = 2 * B * C + input1[2] + input1[3] * C;
		double F = pow(B, 2) + input1[3] * B - input2[0];

		double det = pow(EE, 2) - 4 * D * F;
		if (det < 0)
		{
			return -1;
		}

		outPut[0] = (-EE + pow(det, 0.5)) / (2 * D);
		outPut[2] = B + C * outPut[0];

		outPut[1] = (-EE - pow(det, 0.5)) / (2 * D);
		outPut[3] = B + C * outPut[1];
		return 4;
	}
	return 0;
}
double CheckInputAngle(double input)
{
	if (input >= PI)
	{
		return input - 2 * PI;
	}
	if (input < -PI)
	{
		return 2 * PI + input;
	}
	return input;
}
void SelPos(double* input,double* posLast,double* res)
{
	double delt[2];
	memset(delt, 0, 2 * sizeof(double));
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			delt[i] += pow(input[4 * i+j] - posLast[j], 2);
		}
	}
	// 选择距离最小的解为最优解
	if (delt[0] > delt[1])
	{
		res[0] = input[4];
		res[1] = input[5];
	}
	else
	{
		res[0] = input[0];
		res[1] = input[1];
	}
}
std::vector<ResInfo> AddZeroPoint(std::vector<ResInfo> infoList)
{
	std::vector<ResInfo> resList;
	ResInfo res;
	int count = infoList.size();
	double* pIn = (double*)malloc(count * 2 * sizeof(double));
	double* vIn = (double*)malloc(count * 2 * sizeof(double));
	double* tIn = (double*)malloc(count * sizeof(double));
	int arrayCount = 0;
	for (int i = 0; i < count - 1; i++)
	{
		if (infoList[i].V1*infoList[i + 1].V1 < 0)
		{
			arrayCount++;
		}
		if (infoList[i].V2*infoList[i + 1].V2<0)
		{
			arrayCount++;
		}
	}
	if (arrayCount == 0)
	{
		return infoList;
	}
	for (int j = 0; j < count; j++)
	{
		pIn[2 * j] = infoList[j].P1;
		pIn[2 * j + 1] = infoList[j].P2;
		vIn[2 * j] = infoList[j].V1;
		vIn[2 * j + 1] = infoList[j].V2;
		tIn[j] = infoList[j].T;
	}
	double* pOut = (double*)malloc(2 * (count + arrayCount) * sizeof(double));
	double* vOut = (double*)malloc(2 * (count + arrayCount) * sizeof(double));
	double* tOut = (double*)malloc((count + arrayCount) * sizeof(double));
	int resLen[1];
	GetNewPvt(pIn, vIn, tIn, count, 1, pOut, vOut, tOut, resLen);
	for (int j = 0; j < resLen[0]; j++)
	{
		res.P1 = pOut[2 * j];
		res.P2 = pOut[2 * j + 1];
		res.V1 = vOut[2 * j];
		res.V2 = vOut[2 * j + 1];
		res.T = tOut[j];
		resList.push_back(res);
	}
	free(pIn);
	free(vIn);
	free(tIn);
	free(pOut);
	free(vOut);
	free(tOut);
	return resList;
}
