#include "params.h"
#include "math.h"
#include "string.h"
/*正解求末端位置*/
/*入参：armLength 各轴长度(主动杆，从动杆，轮间距，平台间距)，deltAngles 外部关节角度转机构0角度的差值
	p:机构0位对应的末端点位置，angles：输入的关节角度,len:输入需要求解点的个数
*/
/*输出参数：res：正解求得的末端位置*/
/*extern "C" __declspec(dllexport)*/ int GetDeltEndPosition(double* armLength, double* deltAngles,double* p, double* angles,int len, double* result)
{
	posIn = (double*)malloc(len*2*sizeof(double));
	double posOut[8];
	double A[8];
	double B[2];	
	memset(&posOut, 0, 8 * sizeof(double));
	// 输入的关节角度转换为模型的角度
	TansAngles(angles, deltAngles, len,0);
	double s0 = sin(posIn[0]);
	double c0 = cos(posIn[0]);
	double s1 = sin(posIn[1]);
	double c1 = cos(posIn[1]);

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
	double* outPut = (double*)malloc(len * 4 * sizeof(double));
	int temp = GetAnswer(A, B, outPut);
	if (temp == 0)
	{
		return -1;
	}
	for (int j = 0; j< 2; j++)
	{
		for (int i = 0; i < 2; i++)
		{
			double flg = pow(-1, i);
			double c2i = (outPut[i] - flg * armLength[2] - armLength[0] * cos(angles[i])) / armLength[1];
			double s2i = (outPut[i+2] - armLength[2] * sin(angles[i])) / armLength[1];
			posOut[4 * j + 2+ i] = CheckInputAngle(atan2(s2i, c2i));
		}
		posOut[4 * j] = outPut[j];
		posOut[4 * j+1] = outPut[j+2];
	}
	// 筛选最优解
	SelPos(posOut,p,result);
	free(posIn);
	free(outPut);
	return 0;
}
/*空间坐标逆解为关节角度*/
/*入参：armLength 各轴长度(主动杆，从动杆，轮间距，平台间距)，deltPosition,外部点转机构空间点的差值，deltAngles 外部关节角度转机构0角度的差值
posLast:机构0位对应的关节角度，posIn：输入的末端点位置,vIn：输入的末端点速度，tIn：时间，len:输入需要求解点的个数
*/
/*输出参数：res：逆解求得的关节角度集合*/
/*extern "C" __declspec(dllexport)*/ int  GetDeltArmPosition(double* armLength, double* deltPosition,double*deltAngles, double* posLast, double* pIn, double* vIn, double* tIn, int len, double* res)
{
	posIn = (double*)malloc(len * 3 * sizeof(double));
	// 输入的空间坐标点转换为机构的坐标点
	TansAngles(pIn, deltPosition, len,1);
	double* pTemp = (double*)malloc(len * 4 * sizeof(double));
	int flg = CalPosition(armLength, len,pTemp);
	if (flg != 0)
	{
		return -1;
	}
	double* vTemp = (double*)malloc(len * 4 * sizeof(double));;
	flg = CalVelocity(armLength,pTemp,vIn, len, vTemp);
	resInfo.clear();	
	for (int i = 0; i < len; i++)
	{
		ResInfo info;
		info.P1 = pTemp[4 * i];
		info.P2 = pTemp[4 * i+1];
		info.V1 = vTemp[4 * i];
		info.V2 = vTemp[4 * i + 1];
		info.T = tIn[i];
		resInfo.push_back(info);
	}
	resInfo = AddZeroPoint(resInfo);
	int size = resInfo.size();
	for (int m = 0; m < size; m++)
	{
		res[m * 5] = resInfo[m].P2 * 180 / PI + deltAngles[0];
		res[m * 5 + 1] = resInfo[m].P1 * 180 / PI + deltAngles[1];
		res[m * 5 + 2] = resInfo[m].V1 * 180 / PI;
		res[m * 5 + 3] = resInfo[m].V2 * 180 / PI;
		res[m * 5+ 4] = resInfo[m].T;
	}
	free(posIn);
	free(pTemp);
	free(vTemp);
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
			res[4*i+j] = -2 * atan((-temp1 + flg * delt) / (temp2 - temp3));
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
		double* temp = MatrixMult(J, vIn);
		res[4 * i] = temp[0];
		res[4 * i + 1] = temp[1];
		res[4 * i + 2] = temp[2];
		res[4 * i + 3] = temp[3];
	}
	return 0;
}
double* MatrixMult(double* input1, double* input2)
{
	double temp[4];
    memset(temp, 0, 4 * sizeof(double));
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
            temp[i] += input1[4*i+j] * input2[j % 2];
		}
	}
	return temp;
}
void TansAngles(double* input, double* delt, int len,int mode)
{
	//正解
	if (mode == 0)
	{
		for (int i = 0; i < len; i++)
		{
			posIn[2 * i] = (delt[1] - input[2 * i + 1])*PI/180;
			posIn[2 * i + 1] = (delt[0] - input[2 * i]) * PI / 180;
		}
	}
	// 逆解
	else
	{
		for (int i = 0; i < len; i++)
		{
			posIn[3 * i] = input[3 * i] - delt[0];
			posIn[3 * i +1] = input[3 * i+1];
			posIn[3 * i + 2] = input[3 * i + 2] - delt[1];;
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
			double E = input1[3];
			double F = -input2[0] + pow(outPut[0], 2) + input1[2] * outPut[0];
			delt = pow(E, 2) - 4 * D*F;
			if (delt < 0)
			{
				return 0;
			}
			outPut[2] = (-E + pow(delt, 0.5)) / (2 * D);
			outPut[3] = (-E - pow(delt, 0.5)) / (2 * D);
		}
	}
	else
	{
		double B = (input2[0] - input2[1]) / delt;
		double C = (input1[6] - input1[2]) / delt;

		double D = pow(C, 2) + 1;
		double E = 2 * B * C + input1[2] + input1[3] * C;
		double F = pow(B, 2) + input1[3] * B - input2[0];

		double det = pow(E, 2) - 4 * D * F;
		if (det < 0)
		{
			return 0;
		}

		outPut[0] = (-E + pow(det, 0.5)) / (2 * D);
		outPut[1] = B + C * outPut[0];

		outPut[2] = (-E - pow(det, 0.5)) / (2 * D);
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
double* SelPos(double* input,double* posLast,double* res)
{
	double delt[2];
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			delt[i] = pow(input[4 * i+j] - posLast[j], 2);
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
	return res;
}
std::vector<ResInfo> AddZeroPoint(std::vector<ResInfo> infoList)
{
	std::vector<ResInfo> resList;
	int count = infoList.size();
	double** tempP = new double*[count];
	double** tempV = new double*[count];
	for (int index = 0; index < count; index++)
	{
		tempP[index] = new double[2];
		tempV[index] = new double[2];
	}
	double* tempT = new double[count];
	for (int i = 0; i < count; i++)
	{
		tempP[i][0] = infoList[i].P1;
		tempP[i][1] = infoList[i].P2;
		tempV[i][0] = infoList[i].V1;
		tempV[i][1] = infoList[i].V2;
		tempT[i] = infoList[i].T;
	}
	// 存在速度反向点的时间区间的个数
	int tCount = 0;
	// 一个时间区间内速度反向点的个数
	int tInnerCount = 0;
	// 保存一个区间的多个零点左区间下标
	int* selZCP = new int[2];
	// 保存一个区间的左边界至多个零点区间的比例
	double* selSca = new double[2];
	// 记录过零点区间左边界元素下标
	int* ZCP = new int[2 * count - 1];
	// 记录过零点所在区间比例
	double* sca = new double[2 * count - 1];
	for (int k = 0; k < count - 1; k++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (tempV[k][j] * tempV[k + 1][j] < 0)
			{
				if (fabs(tempV[k][j]) < ERROR)
				{
					tempV[k][j] = 0;
				}
				else if (fabs(tempV[k + 1][j]) < ERROR)
				{
                    tempV[k + 1][j] = 0;
				}
				else
				{
					selZCP[tInnerCount] = k + 1;
					selSca[tInnerCount] = fabs(tempV[k][j] / (tempV[k + 1][j] - tempV[k][j]));
					tCount++;
					tInnerCount++;
				}
			}
		}
		// 零点排序，保证时间为递增关系（sca与时间成正比关系）
		selSca = Rank(selSca, 2);
		for (int m = 0; m < tInnerCount; m++)
		{
			int n = tCount - tInnerCount + m;
			ZCP[n] = selZCP[m];
			sca[n] = selSca[2 - tInnerCount + m];
		}
		tInnerCount = 0;
	}
	double** resP = new double*[count + tCount];
	double** resV = new double*[count + tCount];
	for (int p = 0; p < count + tCount; p++)
	{
		resP[p] = new double[2];
		resV[p] = new double[2];
	}
	double* resT = new double[count + tCount];
	// 零点处插值
	int index1 = 0, index2 = 0;
	for (int a = 0; a < 2; a++)
	{
		for (int b = 0; b < count; b++)
		{
			// 保留原本的元素
			resP[index1][a] = tempP[b][a];
			resV[index1][a] = tempV[b][a];
			if (a == 0)
			{
				resT[index1] = tempT[b];
			}
			index1++;
			if (b < count - 1)
			{
				for (int c = 0; c < 4; c++)
				{
					if (b == ZCP[index2] - 1 && index2 <= count)
					{
						resP[index1][a] = tempP[b][a] * (1 - sca[index2]) + tempP[b + 1][a] * sca[index2];
						resV[index1][a] = tempV[b][a] * (1 - sca[index2]) + tempV[b + 1][a] * sca[index2];
						if (a == 1)
						{
							resT[index1] = tempT[b] * (1 - sca[index2]) + tempT[b + 1] * sca[index2];
						}
						if (fabs(resV[index1][a]) < ERROR)
						{
                            resV[index1][ a] = 0;
						}
						index2++;
						index1++;
					}
				}
			}
		}
		index1 = 0;
		index2 = 0;
	}
	int plen = count + tCount;
	resList = DeleteRedundantPVT(resP, resV, resT, plen);
	delete[]tempP;
	delete[]tempV;
	delete[]tempT;
	delete[]resP;
	delete[]resV;
	delete[]resT;
	return resList;
}
double* Rank(double* input, int len)
{
	for (int i = 0; i < len; i++)
	{
		for (int j = i + 1; j < len; j++)
		{
			if (input[i] > input[j])
			{
				double temp = input[i];
				input[i] = input[j];
				input[j] = temp;
			}
		}
	}
	return input;
}
std::vector<ResInfo> DeleteRedundantPVT(double** inputP, double** inputV, double* inputT, int len)
{
	std::vector<ResInfo> resInfo;
	double** tempP = new double*[len];
	double** tempV = new double*[len];
	double* tempT = new double[len];
	for (int i = 0; i < len; i++)
	{
		tempP[i] = new double[2];
		tempV[i] = new double[2];
	}
	// 记录输入矩阵不重复元素的下标
	int index1 = 0;
	// 记录输出矩阵的元素个数
	int index2 = 0;
	// 记录相邻重复元素的个数
	int index3 = 1;
	while (index1 <= len - 1)
	{
		if (index1 < len - 1)
		{
			while (fabs(inputT[index1 + index3] - inputT[index1]) < ERROR)
			{
				index3++;
			}
		}
		for (int i = 0; i < 2; i++)
		{
			tempP[index2][i] = inputP[index1][i];
			tempV[index2][i] = inputV[index1][i];
		}
		tempT[index2] = inputT[index1];
		index1 = index1 + index3;
		index3 = 1;
		index2++;
	};
	for (int j = 0; j < index2; j++)
	{
		ResInfo res;
		res.P1 = tempP[j][0];
		res.P2 = tempP[j][1];
		res.V1 = tempV[j][0];
		res.V2 = tempV[j][1];
		res.T = tempT[j];
		resInfo.push_back(res);
	}
	delete[] tempP;
	delete[] tempV;
	delete[] tempT;
	return resInfo;
}
