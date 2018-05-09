#include "AddPointParams.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
/* 给速度反向的两行PVT中间插值速度为0的一行*/
/* 输入：pIn,vIn,tIn
length: 点的个数
mode:构型参数（0:T4,1:DT)
输出：pOut,vOut,tOut
pLen:插值完的长度
*/
void GetNewPvt(double* pIn, double*vIn, double*tIn, int length, int mode, double* pOut, double* vOut, double*tOut, int* pLen)
{
	int joinNum = 0;
	if (mode == 0)
	{
		// T4
		joinNum = 4;
	}
	else if (mode == 1)
	{
		// DT
		joinNum = 2;
	}
	// 存在速度反向点的时间区间的个数
	int tCount = 0;
	// 记录过零点区间左边界元素下标
	int* ZCP = (int*)malloc((length - 1) * sizeof(int));
	memset(ZCP, 0, (length - 1) * sizeof(int));
	double* scaZCP = (double*)malloc(joinNum *(length - 1) * sizeof(double));
	memset(scaZCP, 0, joinNum * (length - 1) * sizeof(double));
	for (int i = 0; i < length - 1; i++)
	{
		for (int j = 0; j < joinNum; j++)
		{
			if (vIn[joinNum*i + j] * vIn[joinNum*(i + 1) + j] < 0)
			{
				double sca = fabs(vIn[joinNum*i + j] / (vIn[joinNum*(i + 1) + j] - vIn[joinNum*i + j]));
				if (fabs(vIn[joinNum*i + j]) < error)
				{
					vIn[joinNum*i + j] = 0;
				}
				else if (fabs(vIn[joinNum*(i + 1) + j]) < error)
				{
					vIn[joinNum*(i + 1) + j] = 0;
				}
				else
				{
					tCount++;
					scaZCP[joinNum* i +j] = sca;
					if (ZCP[i] < 1)
					{
						ZCP[i] = 1;
					}
				}
			}
		}
	}
	// 不存在速度反向的点
	if (tCount == 0)
	{
                memcpy(pOut, pIn, joinNum*length * sizeof(double));
                memcpy(vOut, vIn, joinNum*length * sizeof(double));
                memcpy(tOut, tIn, length * sizeof(double));

                *pLen = length;

                free( ZCP );
                free( scaZCP );
                return;
	}
	double* resP = (double*)malloc((length + tCount)*joinNum  * sizeof(double));
	double* resV = (double*)malloc((length + tCount)*joinNum * sizeof(double));
	double* resT = (double*)malloc((length + tCount) * sizeof(double));
	// 零点处插值
	int  index2 = 0;
	for (int i = 0; i < length; i++)
	{
		if (i < length - 1)
		{
			int pvtNum = 0;
			for (int j = 0; j < joinNum; j++)
			{
				if (vIn[joinNum*i + j] * vIn[joinNum*(i + 1) + j] < 0)
				{
					pvtNum++;
				}
			}
			if (1 == ZCP[i])
			{
				pvtNum = pvtNum + 2;
				double* pZcp = (double*)malloc(joinNum*pvtNum * sizeof(double));
				double* vZcp = (double*)malloc(joinNum*pvtNum * sizeof(double));
				double* tZcp = (double*)malloc(pvtNum * sizeof(double));
				memset(pZcp, 0, joinNum*pvtNum * sizeof(double));
				memset(vZcp, 0, joinNum*pvtNum * sizeof(double));
				memset(tZcp, 0, pvtNum * sizeof(double));
				CalZcp(pIn, vIn, tIn, scaZCP, i, joinNum, pvtNum, pZcp, vZcp, tZcp);
				int  index1 = index2 + pvtNum - 1;
				for (int j = index2; j < index1; j++)
				{
					for (int k = 0; k < joinNum; k++)
					{
						resP[joinNum*j+k] = pZcp[pvtNum*k +j - index2];
						resV[joinNum*j + k] = vZcp[pvtNum*k + j - index2];
						resT[j] = tZcp[j - index2];
					}
				}
				index2 = index1;
				free(pZcp);
				free(vZcp);
				free(tZcp);
			}
			else
			{
				for (int j = 0; j < joinNum; j++)
				{
					resP[joinNum*index2+j] = pIn[joinNum*i+j];
					resV[joinNum*index2 + j] = vIn[joinNum*i + j];
					resT[index2] = tIn[i];
				}
				index2++;
			}
		}
		else
		{
			for (int j = 0; j < joinNum; j++)
			{
				resP[joinNum*index2 + j] = pIn[joinNum*i + j];
				resV[joinNum*index2 + j] = vIn[joinNum*i + j];
				resT[index2] = tIn[i];
			}
		}
	}
	DeleteRedundantPvt(resP, resV, resT,length+tCount, joinNum,pOut,vOut,tOut, pLen);
	free(ZCP);
	free(scaZCP);
	free(resP);
	free(resV);
	free(resT);
}
void CalZcp(double* p, double* v, double* t, double* scaIn, int index, int jointCount,int pvtCount, double* pOut, double* vOut, double* tOut)
{
	double* pZcp = (double*)malloc(jointCount * sizeof(double));
	double* sZcp = (double*)malloc(2*jointCount * sizeof(double));
	double* flgZcp = (double*)malloc(jointCount * sizeof(double));
	memset(sZcp, 0, 2 * jointCount * sizeof(double));
	memset(flgZcp, 0, jointCount * sizeof(double));
	int zcpCount = 0;
	double sca = 0;
	for (int i = 0; i < jointCount; i++)
	{
		if (scaIn[jointCount*index +i] > 0)
		{
			sZcp[zcpCount] = i + 1;
			sZcp[jointCount +zcpCount] = scaIn[jointCount*index + i];
			flgZcp[i] = 1;
			double deltT = t[index + 1] - t[index];
			double deltV = v[(index + 1)*jointCount+ i] - v[index*jointCount+ i];
			sca = scaIn[jointCount*index + i];;
			double deltP1 = 0.5*v[index*jointCount + i] * deltT* sca + p[index*jointCount + i];
			double deltP2 = -0.5 * v[(index + 1)*jointCount + i] * deltT*(1 - sca) + p[(index + 1)*jointCount + i];

			if (deltV < 0)
			{
				if (deltP1 > deltP2)
				{
					pZcp[zcpCount] = deltP1;
				}
				else
				{
					pZcp[zcpCount] = deltP2;
				}
			}
			else
			{
				if (deltP1 > deltP2)
				{
					pZcp[zcpCount] = deltP2;
				}
				else
				{
					pZcp[zcpCount] = deltP1;
				}
			}
			zcpCount++;
		}
	}
	int tempIndex = 0;
	int pvtNum = zcpCount + 2;
	Rank(sZcp,jointCount);
	for (int i = 0; i < jointCount; i++)
	{
		pOut[pvtCount*i] = p[jointCount*index+i];
		pOut[pvtCount*i + pvtNum - 1] = p[jointCount*(index + 1)+ i];
		vOut[pvtCount*i] = v[jointCount*index + i];
		vOut[pvtCount*i + pvtNum - 1] = v[jointCount*(index + 1) + i];
		if (i < 1)
		{
			tOut[0] = t[index];
			tOut[pvtNum - 1] = t[index + 1];
		}
		int flg = 0;
		int temp;
		for (int j = 0; j < zcpCount; j++)
		{
			temp = jointCount - zcpCount + j;
			if (fabs(sZcp[temp] - i - 1)<error)
			{
				pOut[pvtCount*i+j + 1] = pZcp[tempIndex];
				vOut[pvtCount*i + j + 1] = 0;
				sca = sZcp[jointCount+ temp];
				flg = temp;
				tempIndex++;
			}
		}
		for (int k = 0; k < zcpCount; k++)
		{
			temp = jointCount - zcpCount + k;
			double sca1;
			if (flgZcp[i] > 0)
			{
				if (temp < flg)
				{
					sca1 = sZcp[jointCount+  temp] / sca;
					pOut[pvtCount*i+ k + 1] = p[jointCount *index + i] * (1 - sca1) + pZcp[tempIndex - 1] * sca1;
				}
				else if (temp > flg)
				{
					sca1 = (sZcp[jointCount+ temp] - sca) / (1 - sca);
					pOut[pvtCount*i + k + 1] = p[jointCount *(index + 1)+ i] * sca1 + pZcp[tempIndex - 1] * (1 - sca1);
				}
			}
			else
			{
				pOut[pvtCount*i + k + 1] = p[jointCount *(index + 1) + i] * sZcp[jointCount+ temp] + p[jointCount *index + i] * (1 - sZcp[jointCount + temp]);
			}
			if (temp != flg)
			{
				vOut[pvtCount*i + k + 1] = v[jointCount *(index + 1) + i] * sZcp[jointCount + temp] + v[jointCount *index + i] * (1 - sZcp[jointCount + temp]);
			}
			if (i < 1)
			{
				tOut[k + 1] = t[index + 1] * sZcp[jointCount + temp] + t[index] * (1 - sZcp[jointCount + temp]);
			}
		}
	}
	free(pZcp);
	free(sZcp);
	free(flgZcp);
}
void Rank(double* input,int len)
{
	for (int i = 0; i < len-1; i++)
	{
		for (int j = i + 1; j < len; j++)
		{
			if (input[4+i] > input[4+j])
			{
				double temp1 = input[4 + i];
				input[4 + i] = input[4 + j];
				input[4 + j] = temp1;
				double temp2 = input[i];
				input[i] = input[j];
				input[j] = temp2;
			}
		}
	}
}
void DeleteRedundantPvt(double* inputP, double* inputV, double* inputT,int len,int joinCount,double* pOut, double* vOut, double* tOut, int* outLen)
{
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
			while (fabs(inputT[index1 + index3] - inputT[index1]) < error)
			{
				index3++;
			}
		}
		for (int i = 0; i < joinCount; i++)
		{
			pOut[joinCount * index2+ i] = inputP[joinCount*index1+ i];
			vOut[joinCount *index2 +i] = inputV[joinCount *index1 + i];
		}
		tOut[index2] = inputT[index1];
		index1 = index1 + index3;
		index3 = 1;
		index2++;
	};
	outLen[0] = index2;
}
