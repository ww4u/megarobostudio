// DllManager.cpp : 定义 DLL 应用程序的导出函数。
#include "T4params.h"
#include "AddPointParams.h"
#include "math.h"
#include <vector>

// 正解求末端位置
// armLength 机械臂的臂长
// angles--四个关节的当前角度
// res--正解的末端位置(X,Y,Z)
MEGA_EXTERN MEGA_EXPORT int GetEndPosition(double* armLength, double* deltAngles,double* angles,double* res)
{
	armLengthArray = armLength;
	bool flag = CheckInputAngles(angles);		// 判断位置是否在限制范围内
	double newAngles[4];
	CorrectAngles(deltAngles,angles, newAngles);
	if (!flag) // 非法关节位置
		return 0;     // 退出程序
	double s1, c1, s2, c2, s2a3, c2a3, s2a3a4, c2a3a4;      // 定义三角函数
	/*-------------- - 常用三角函数计算--------------*/
	s1 = sin(newAngles[0]);     // sin(theta1)
	c1 = cos(newAngles[0]);     // cos(theta1)
	s2 = sin(newAngles[1]);     // sin(theta2)
	c2 = cos(newAngles[1]);     // cos(theta2)
	s2a3 = sin(newAngles[1] + newAngles[2]);      // sin(theta2 + theta3)
	c2a3 = cos(newAngles[1] + newAngles[2]);      // cos(theta2 + theta3)
	s2a3a4 = sin(newAngles[1] + newAngles[2] + newAngles[3]);      // sin(theta2 + theta3 + theta4)
	c2a3a4 = cos(newAngles[1] + newAngles[2] + newAngles[3]);      // cos(theta2 + theta3 + theta4)
	/*-------------- - 位置正解求解--------------*/
	double** endOut = new double*[4];
	for (int i = 0; i < 4; i++)
	{
		endOut[i] = new double[4];
	}
	endOut[0][0] = c1* c2a3a4;   // 计算位姿矩阵
	endOut[0][1] = -c1* s2a3a4;
	endOut[0][2] = s1;
	endOut[0][3] = armLengthArray[4] * s1 - armLengthArray[5] * c1 * s2a3a4 + c1*(armLengthArray[1] * c2 + armLengthArray[2] * c2a3 + armLengthArray[3] * c2a3a4);

	endOut[1][0] = s1* c2a3a4;
	endOut[1][1] = -s1* s2a3a4;
	endOut[1][2] = -c1;
	endOut[1][3] = -c1 * armLengthArray[4] - armLengthArray[5] * s1 * s2a3a4 + s1*(armLengthArray[1] * c2 + armLengthArray[2] * c2a3 + armLengthArray[3] * c2a3a4);

	endOut[2][0] = s2a3a4;
	endOut[2][1] = c2a3a4;
	endOut[2][3] = armLengthArray[5] * c2a3a4 + armLengthArray[0] + armLengthArray[1] * s2 + armLengthArray[2] * s2a3 + armLengthArray[3] * s2a3a4;

	endOut[3][3] = 1;
	res[0] = endOut[0][3];
	res[1] = endOut[1][3];
	res[2] = endOut[2][3];
	delete[]endOut;
	return 1;
}
bool CheckInputAngles(double* input)
{
	for(int i =0;i<4;i++)
	{
		if (input[i] > PI  || input[i] < -PI)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}
void CorrectAngles(double* delt,double* input,double* res)
{
	memset(res, 0, 4 * sizeof(double));
	for (int i = 0; i < 4; i++)
	{
		res[i] = (input[i] - delt[i])*PI/180;
	}
}

MEGA_EXTERN MEGA_EXPORT  int  GetPvtInfo(double* res)
{	
	int size = resInfo.size();
	for (int m = 0; m < size; m++)
	{
		res[m * 9] = resInfo[m].P1 * 180 / PI + deltAnglesArray[0];
		res[m * 9 + 1] = resInfo[m].P2 * 180 / PI + deltAnglesArray[1];
		res[m * 9 + 2] = resInfo[m].P3 * 180 / PI + deltAnglesArray[2];
		res[m * 9 + 3] = resInfo[m].P4 * 180 / PI + deltAnglesArray[3];
		res[m * 9 + 4] = resInfo[m].V1 * 180 / PI;
		res[m * 9 + 5] = resInfo[m].V2 * 180 / PI;
		res[m * 9 + 6] = resInfo[m].V3 * 180 / PI;
		res[m * 9 + 7] = resInfo[m].V4 * 180 / PI;
		res[m * 9 + 8] = resInfo[m].T;
	}
	return 0;
}
// 逆解运算--计算结束PVT的条数
// posLast -- 参考位置，用于确定最优路径,输入为4个关节的角度
// posIn -- 用于逆解的末端位置(X,Y,Z)
// vIn--末端速度(VX,VY,VZ)
// tIn--时间节点
// 求解的点的个数
// 求解的返回值
MEGA_EXTERN MEGA_EXPORT int  GetPvtLen(double* armLength, double* deltAngles, double* posLast,
                                       double* posIn, int skipP,
                                       double* vIn, int skipV,
                                       double* tIn, int skipT,
                                       int len,
                                       int* resLen )
{
	armLengthArray = armLength;
	deltAnglesArray = deltAngles;
	// 转换构型
	for (int index1 = 0; index1 < 4; index1++)
	{
		posLast[index1] = (posLast[index1] - deltAngles[index1])*PI / 180;
	}
	// 点个数
	resInfo.clear();
	for (int i = 0; i < len; i++)
	{
		double P[3];
		double V[3];
		memset(P, 0, 3 * sizeof(double));
		memset(V, 0, 3 * sizeof(double));
		for (int j = 0; j < 3; j++)
		{
                        P[j] = posIn[skipP*i+j];
                        V[j] = vIn[skipV * i + j];
		}
		double poseOut[4];
		memset(poseOut, 0, 4 * sizeof(double));
		for (int k = 0; k < 2; k++)
		{
			//末端位姿
			double pose[16];
			memset(pose, 0, 16 * sizeof(double));
			CalPose(P, posLast, k, pose);
			// 求解关节位置			
			int resFlg = CalPosition(pose, posLast, poseOut);
			if (resFlg == -1)
			{
				return -1;
			}
			double p1 = posLast[1] + posLast[2] + posLast[3];
			double p2 = poseOut[1] + poseOut[2] + poseOut[3];
			int res = CompareAngle(p1, p2);
			if (res == 0)
			{
				break;
			}
		}
		ResInfo info;
		if (poseOut != NULL)
		{
			double allPos[7];
			allPos[0] = poseOut[0];
			allPos[1] = poseOut[1];
			allPos[2] = poseOut[2];
			allPos[3] = poseOut[3];
			allPos[4] = P[0];
			allPos[5] = P[1];
			allPos[6] = P[2];
			double poseV[6];
			memset(poseV, 0, 6 * sizeof(double));
			CalV(allPos, V,poseV);
			// 求解关节速度
			double vOut[4];
			memset(vOut, 0, 4 * sizeof(double));
			CalVelocity(poseOut, poseV, vOut);
			double newPose[4];
			memset(newPose, 0, 4 * sizeof(double));
			GetContinueAngle(posLast, poseOut,newPose);			
			info.P1 = newPose[0];
			info.P2 = newPose[1];
			info.P3 = newPose[2];
			info.P4 = newPose[3];
			info.V1 = vOut[0];
			info.V2 = vOut[1];
			info.V3 = vOut[2];
			info.V4 = vOut[3];
                        info.T = tIn[i*skipT];
		}
		else
		{
			return -1;
		}
		resInfo.push_back(info);			
	}
	resInfo = AddZeroPoint(resInfo);
	int size = resInfo.size();
	resLen[0] = size;	
	return 0;
}
void CalPose(double* pIn, double* pLast,int index,double* pose)
{
	// 末端相对水平的夹角
	double theta = pLast[1] + pLast[2] + pLast[3];
	double d = armLengthArray[4];
	double px = pIn[0];
	double py = pIn[1];
	double temp = sqrt(px * px + py * py);
	double det = -armLengthArray[5] * sin(theta) + armLengthArray[3] * cos(theta);
	double t1 = 0;
	if (index == 0)
	{
		if (d != 0)
		{
			if (det >= 0)
				t1 = atan2(py, px) - asin(-d / temp);
			else
				t1 = atan2(-py, -px) - asin(d / temp);
		}
		else
		{
			if (det >= 0)
				t1 = atan2(py, px);
			else
				t1 = atan2(-py, -px);
		}
	}
	else
	{
		if (d != 0)
		{
			if (det > 0)
				t1 = atan2(-py, -px) - asin(d / temp);
			else
				t1 = atan2(py, px) - asin(-d / temp);
		}
		else
		{
			if (det > 0)
				t1 = atan2(-py, -px);
			else
				t1 = atan2(py, px);
		}
		
	}
	pose[0] =cos(t1) * cos(theta);
	pose[1] = -cos(t1) * sin(theta);
	pose[2] = sin(t1);
	pose[3] = pIn[0];

	pose[4] = sin(t1)* cos(theta);
	pose[5] = -sin(t1) * sin(theta);
	pose[6] = -cos(t1);
	pose[7] = pIn[1];

	pose[8] =sin(theta);
	pose[9] = cos(theta);
	pose[11] = pIn[2];

	pose[15] = 1;
}
int CalPosition(double* posIn, double* posLast,double* result)
{
	// 初始化位姿矩阵
	int len = 4*2;
	double* posOrg = new double[len];
	memset(posOrg, 0, len * sizeof(double));
	// 初始化标志
	int* mark = new int[2];
	// double[] posOut = new double[2];
	// 关节1角度
	posOrg[0] = atan2(posIn[2], -posIn[6]);
	posOrg[1] = posOrg[0];
	double c0 = cos(posOrg[0]);
	double s0 = sin(posOrg[0]);
	double bx = posIn[3] - c0* armLengthArray[3] * posIn[9] - armLengthArray[4] * s0 + c0 * armLengthArray[5] * posIn[8];
	double by = posIn[7] - s0* armLengthArray[3] * posIn[9] + armLengthArray[4] * c0 + s0 * armLengthArray[5] * posIn[8];
	double bz = posIn[11] - armLengthArray[3] * posIn[8] - armLengthArray[5] * posIn[9];
	double A, B, C;
	if (fabs(c0) > 0.1)
	{
		// 计算各关节位置
		A =pow(c0, 2)*(pow(bz - armLengthArray[0], 2) - pow(armLengthArray[1], 2) - pow(armLengthArray[2], 2)) +
			pow(bx, 2);
		B = 2 * pow(c0, 2)* armLengthArray[1] * armLengthArray[2];
	}
	else
	{
		A = pow(s0, 2)*(pow(bz - armLengthArray[0], 2) - pow(armLengthArray[1], 2) - pow(armLengthArray[2], 2)) +
			pow(by, 2);
		B = 2 * pow(s0, 2)* armLengthArray[1] * armLengthArray[2];
	}
	double delt = A / B;
	bool flg = CheckError(delt);
	// 有解
	if (flg)
	{
		posOrg[4] = acos(delt);
		posOrg[5] = -acos(delt);
		mark[0] = 1;
		mark[1] = 1;
		// 唯一解
		if (posOrg[4] - posOrg[5] < error)
		{
			mark[1] = 0;
		}
	}
	// 无解
	else
	{
		return -1;
	}
	for (int i = 0; i < 2; i++)
	{
		double c2 = cos(posOrg[4+i]);
		double s2 = sin(posOrg[4+i]);
		if (fabs(c0) > 0.1)
		{
			// 计算各关节位置
			A = -bx*s2* armLengthArray[2] + (bz - armLengthArray[0])*c0*(armLengthArray[1] + c2* armLengthArray[2]);
			B = (armLengthArray[1] + c2* armLengthArray[2])*bx + c0*s2* armLengthArray[2] * (bz - armLengthArray[0]);
			C = c0*(pow(armLengthArray[1], 2) + 2 * c2* armLengthArray[1] * armLengthArray[2] + pow(armLengthArray[2], 2));
		}
		else
		{
			A = -by*s2* armLengthArray[2] + (bz - armLengthArray[0])*s0*(armLengthArray[1] + c2* armLengthArray[2]);
			B = (armLengthArray[1]+ c2* armLengthArray[2])*by + s0*s2* armLengthArray[2] * (bz - armLengthArray[0]);
			C = s0*(pow(armLengthArray[1], 2) + 2 * c2* armLengthArray[1] * armLengthArray[2] + pow(armLengthArray[2], 2));
		}
		double s1 = A / C;
		double c1 = B / C;
		bool flg1 = CheckError(s1);
		bool flg2 = CheckError(c1);
		// 有解
		if (flg1 && flg2)
		{
			posOrg[2+i] = atan2(s1, c1);
			posOrg[6+i] = atan2(posIn[8], posIn[9]) - posOrg[1*2+ i] - posOrg[2*2 + i];
		}
		else
		{
			mark[i] = 0;
			return -1;
		}
	}
	// 角度标准化
	CorrectAngles(posOrg, len);
	// 解的个数及索引位置
	int count = CheckPos(mark, posOrg, len);
	switch (count)
	{
		// 无解
	case 0:
		break;
	case 1:
		GetPosition(1, posOrg,len,result);
		break;
	case 2:
		GetPosition(2, posOrg,len, result);
		break;
	case 3:
		SelPos(posLast, posOrg,len, result);
		break;
	default:
		break;
	}
	delete[] posOrg;
	return 0;
}
bool CheckError(double data)
{
	// 超出范围
	if (data < -1 - error || data>1 + error)
	{
		return false;
	}
	else
	{
		return true;
	}
}
void CorrectAngles(double* input,int len)
{
	for (int i = 0; i < len; i++)
	{
		if (input[i] > PI)
		{
			input[i] = input[i] - 2 * PI;
		}
		if (input[i] < -PI)
		{
			input[i] = input[i] + 2 * PI;
		}
	}
}
int CheckPos(int* flg, double* input, int len)
{
	// 存在有效解
	if (flg[0] == 1 || flg[1] == 1)
	{
		// 存在两个有效解
		if (flg[0] == 1 && flg[1] == 1)
		{
			for (int i = 0; i <len; i++)
			{
				if (input[i] > PI || input[i] < -PI)
				{
					return 0;
				}
			}
			return 3;
		}
		else if (flg[0] == 1)
		{
			for (int j = 0; j < 4; j++)
			{
				if (input[j] > PI || input[j] < -PI)
				{
					return 0;
				}
			}
			return 1;
		}
		else
		{
			for (int j = 4; j < len; j++)
			{
				if (input[j] > PI || input[j] < -PI)
				{
					return 0;
				}
			}
			return 2;
		}
	}
	return 0;
}
void GetPosition(int index, double* input,int len,double* res)
{
	if (index == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			res[i] = input[2*i];
		}
	}
	if (index == 2)
	{
		for (int i = 0; i < 4; i++)
		{
			res[i] = input[2*i + 1];
		}
	}
}
void SelPos(double* pLast, double* input, int len,double* res)
{
	double temp[2];	
	memset(&temp, 0, 2 * sizeof(double));
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp[i] = temp[i] + pow(input[2*j+i] - pLast[j], 2);
		}
	}
	if (temp[0] > temp[1])
	{
		GetPosition(2, input,len,res);
	}
	else
	{
		GetPosition(1, input,len,res);
	}
}
int CompareAngle(double data1, double date2)
{
	double delt = fabs(data1 - date2);
	if (delt > PI)
	{
		delt = fabs(delt - 2 * PI);
	}
	if (delt < 1e-3)
	{
		return 0;
	}
	return -1;
}
void CalV(double* pos, double* v,double* poseV)
{
	// 末端速度
	double c1 = cos(pos[0]);
	double s1 = sin(pos[0]);
	double px = pos[4];
	double py = pos[5];
	double vx = v[0];
	double vy = v[1];
	// 保存末端速度
	poseV[0] = v[0];
	poseV[1] = v[1];
	poseV[2] = v[2];
	// 计算末端角速度
	poseV[5] = (c1 * vy - s1 * vx) / (c1 * px + s1 * py);
}
void CalVelocity(double* pos, double* v,double* vOut)
{
	double s1 = sin(pos[0]);
	double c1 = cos(pos[0]);
	double s2 = sin(pos[1]);
	double c2 = cos(pos[1]);
	double s3 = sin(pos[2]);
	double s2a3 = sin(pos[1] + pos[2]);
	double c2a3 = cos(pos[1] + pos[2]);
	double s2a3a4 = sin(pos[1] + pos[2] + pos[3]);
	double c2a3a4 = cos(pos[1] + pos[2] + pos[3]);
	// ---------------速度雅克比矩阵计算-------------- -
	double** J = new double*[6];
	for (int i = 0; i < 6; i++)
	{
		J[i] = new double[4];			
	}
	double temp1 = -armLengthArray[3] * c1*s2a3a4 - armLengthArray[5] * c1*c2a3a4;
	J[0][0] = -armLengthArray[1] * s1 * c2 - armLengthArray[2] * s1 * c2a3 - s1 * armLengthArray[3] * c2a3a4 + armLengthArray[4] * c1 + armLengthArray[5] * s1*s2a3a4;
	J[0][1] = -armLengthArray[1] * c1 * s2 - armLengthArray[2] * c1 * s2a3 + temp1;
	J[0][2] = -armLengthArray[2] * c1 * s2a3 + temp1;
	J[0][3] = temp1;

	double temp2 = -armLengthArray[3] * s1 * s2a3a4 - armLengthArray[5] * s1 * c2a3a4;
	J[1][0] = armLengthArray[1] * c1 * c2 + armLengthArray[2] * c1 * c2a3 + c1 * armLengthArray[3] * c2a3a4 + armLengthArray[4] * s1 - armLengthArray[5] * c1*s2a3a4;
	J[1][1] = -armLengthArray[1] * s1 * s2 - armLengthArray[2] * s1 * s2a3 + temp2;
	J[1][2] = -armLengthArray[2] * s1*s2a3 + temp2;
	J[1][3] = temp2;

	double temp3 = armLengthArray[3] * c2a3a4 - armLengthArray[5] * s2a3a4;
	J[2][1] = armLengthArray[1] * c2 + armLengthArray[2]* c2a3 + temp3;
	J[2][2] = armLengthArray[2] * c2a3 + temp3;
	J[2][3] = temp3;

	J[3][1] = s1;
	J[3][2] = s1;
	J[3][3] = s1;

	J[4][1] = -c1;
	J[4][2] = -c1;
	J[4][3] = -c1;

	J[5][0] = 1;
	vOut[0] = v[5];
	//奇偶规避
	double P = BizarreAvoid(s3);
	if (fabs(c1) > 0.1)
	{
		double A = J[0][1] - J[0][3];
		double B = J[0][2] - J[0][3];
		double C = J[2][1] - J[2][3];
		double D = J[2][2] - J[2][3];

		double b1 = v[0] - (J[0][0] * vOut[0] + J[0][3] * v[4] / J[4][3]);
		double b2 = v[2] - v[4] * J[2][3] / J[4][3];

		P = P / (armLengthArray[1] * armLengthArray[2] * c1);
		vOut[1] = (D*b1 - B*b2)*P;
		vOut[2] = -(C*b1 - A*b2)*P;
		vOut[3] = v[4] / J[4][3] - vOut[1] - vOut[2];
	}
	else
	{
		double A = J[1][1] - J[1][3];
		double B = J[1][2] - J[1][3];
		double C = J[2][1] - J[2][3];
		double D = J[2][2] - J[2][3];

		double b1 = v[1] - (J[1][0] * vOut[0] + J[1][3] * v[3] / J[3][3]);
		double b2 = v[2] - v[4] * J[2][3] / J[3][3];

		P = P / (armLengthArray[1] * armLengthArray[2] * s1);
		vOut[1] = (D*b1 - B*b2)*P;
		vOut[2] = -(C*b1 - A*b2)*P;
		vOut[3] = v[3] / J[3][3] - vOut[1] - vOut[2];
	}
	delete[] J;
}
double BizarreAvoid(double input)
{
	double temp;
	if (fabs(input) < 0.1)
	{
		temp = 0.1 * (1 - fabs(input) / 0.1);
	}
	else
	{
		temp = 0;
	}
	return input / (pow(input, 2) + pow(E, -fabs(input)) * pow(temp, 2));
}
void GetContinueAngle(double* last, double* pos,double* res)
{
	for (int i = 0; i < 4; i++)
	{
		double temp1 = last[i] - floor(last[i] / (2 * PI)) * 2 * PI;
		double temp2 = pos[i] - floor(pos[i] / (2 * PI)) * 2 * PI;
		double delt = temp2 - temp1;
		if (delt > PI)
		{
			delt -= 2 * PI;
		}
		if (delt < -PI)
		{
			delt += 2 * PI;
		}
		res[i] = last[i] + delt;
	}
}

std::vector<ResInfo> AddZeroPoint(std::vector<ResInfo> infoList)
{
	std::vector<ResInfo> resList;
	ResInfo res;
	int count = infoList.size();
	double* pIn = (double*)malloc(count * 4 * sizeof(double));
	double* vIn = (double*)malloc(count * 4 * sizeof(double));
	double* tIn = (double*)malloc(count* sizeof(double));
	int arrayCount = 0;
	for (int i = 0; i < count-1; i++)
	{
		if (infoList[i].V1*infoList[i + 1].V1 < 0)
		{
			arrayCount++;
		}
		if (infoList[i].V2*infoList[i + 1].V2<0)
		{
			arrayCount++;
		}
		if (infoList[i].V3*infoList[i + 1].V3 < 0)
		{
			arrayCount++;
		}
		if (infoList[i].V4*infoList[i + 1].V4 < 0)
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
		pIn[4 * j] = infoList[j].P1;
		pIn[4 * j+1] = infoList[j].P2;
		pIn[4 * j+2] = infoList[j].P3;
		pIn[4 * j+3] = infoList[j].P4;
		vIn[4 * j] = infoList[j].V1;
		vIn[4 * j + 1] = infoList[j].V2;
		vIn[4 * j + 2] = infoList[j].V3;
		vIn[4 * j + 3] = infoList[j].V4;
		tIn[j] = infoList[j].T;
	}
	double* pOut = (double*)malloc(4 * (count + arrayCount) * sizeof(double));
	double* vOut = (double*)malloc(4 * (count + arrayCount) * sizeof(double));
	double* tOut = (double*)malloc((count + arrayCount) * sizeof(double));
	int resLen[1];
	GetNewPvt(pIn, vIn, tIn, count, 0, pOut, vOut, tOut, resLen);
	for (int j = 0; j < resLen[0]; j++)
	{
		res.P1 = pOut[4 * j];
		res.P2 = pOut[4 * j + 1];
		res.P3 = pOut[4 * j + 2];
		res.P4 = pOut[4 * j + 3];
		res.V1 = vOut[4 * j];
		res.V2 = vOut[4 * j + 1];
		res.V3 = vOut[4 * j + 2];
		res.V4 = vOut[4 * j + 3];
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

