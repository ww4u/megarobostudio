// DllManager.cpp : 定义 DLL 应用程序的导出函数。

#include "kinematic.h"

#include "math.h"
#include <vector>

namespace ns_kinematic {

#include "param.h"

// 正解求末端位置
// angles--四个关节的当前角度
// res--正解的末端位置(X,Y,Z)
MEGA_EXPORT int GetEndPosition(double* angles,double* res)
{
	double s1, c1, s2, c2, s2a3, c2a3, s2a3a4, c2a3a4;      // 定义三角函数
	/*-------------- - 常用三角函数计算--------------*/
	s1 = sin(armLen0);     // sin(theta1)
	c1 = cos(armLen0);     // cos(theta1)
	s2 = sin(armLen1);     // sin(theta2)
	c2 = cos(armLen1);     // cos(theta2)
	s2a3 = sin(armLen1 + armLen2);      // sin(theta2 + theta3)
	c2a3 = cos(armLen1 + armLen2);      // cos(theta2 + theta3)
	s2a3a4 = sin(armLen1 + armLen2 + armLen3);      // sin(theta2 + theta3 + theta4)
	c2a3a4 = cos(armLen1 + armLen2 + armLen3);      // cos(theta2 + theta3 + theta4)

															/*-------------- - 位置正解求解--------------*/

	bool flag = CheckInputAngles(angles);		// 判断位置是否在限制范围内
	if (!flag) // 非法关节位置
		return 0;     // 退出程序
	double** endOut = new double*[4];
	for (int i = 0; i < 4; i++)
	{
		endOut[i] = new double[4];
	}
	endOut[0][0] = c1* c2a3a4;   // 计算位姿矩阵
	endOut[0][1] = -c1* s2a3a4;
	endOut[0][2] = s1;
	endOut[0][3] = armLen4 * s1 - armLen5 * c1 * s2a3a4 + c1*(armLen1 * c2 + armLen2 * c2a3 + armLen3 * c2a3a4);

	endOut[1][0] = s1* c2a3a4;
	endOut[1][1] = -s1* s2a3a4;
	endOut[1][2] = -c1;
	endOut[1][3] = -c1 * armLen4 - armLen5 * s1 * s2a3a4 + s1*(armLen1 * c2 + armLen2 * c2a3 + armLen3 * c2a3a4);

	endOut[2][0] = s2a3a4;
	endOut[2][1] = c2a3a4;
	endOut[2][3] = armLen5 * c2a3a4 + armLen0 + armLen1 * s2 + armLen2 * s2a3 + armLen3 * s2a3a4;

	endOut[3][3] = 1;
	res[0] = endOut[0][3];
	res[1] = endOut[1][3];
	res[2] = endOut[2][3];
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
// 逆解运算
// posLast -- 参考位置，用于确定最优路径
// posIn -- 用于逆解的末端位置(X,Y,Z)
// vIn--末端速度(VX,VY,VZ)
// tIn--时间节点
// 求解的点的个数
// 求解的返回值 : return trace count
MEGA_EXPORT int  GetArmPosition(double* posLast,
                                double* posIn, int skipP,
                                double* vIn, int skipV,
                                double* tIn, int skipT,
                                int len,
                                double* res )
{
	// 点个数
    resInfo.clear();

    double P[3];
    double V[3];

	for (int i = 0; i < len; i++)
	{
//		double* P = new double[3];
//		double* V = new double[3];

		for (int j = 0; j < 3; j++)
		{
//			P[j] = posIn[3*i+j];
//			V[j] = vIn[3 * i + j];

            P[j] = posIn[ skipP * i + j ];
            V[j] = vIn[ skipV * i + j ];
		}
		double* poseOut = NULL;
		for (int k = 0; k < 2; k++)
		{
			//末端位姿
			double* pose = CalPose(P, posLast, k);
			// 求解关节位置
			poseOut = CalPosition(pose, posLast);
			if (poseOut == NULL)
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
        ResInfo _info;
		if (poseOut != NULL)
		{
			double* allPos = new double[7];
			allPos[0] = poseOut[0];
			allPos[1] = poseOut[1];
			allPos[2] = poseOut[2];
			allPos[3] = poseOut[3];
			allPos[4] = P[0];
			allPos[5] = P[1];
			allPos[6] = P[2];
			double* res = CalV(allPos, V);
			// 求解关节速度
			double* vLast;
			double* vOut = CalVelocity(poseOut, res);
			double* newPose = GetContinueAngle(posLast, poseOut);			
            _info.P1 = newPose[0];
            _info.P2 = newPose[1];
            _info.P3 = newPose[2];
            _info.P4 = newPose[3];
            _info.V1 = vOut[0];
            _info.V2 = vOut[1];
            _info.V3 = vOut[2];
            _info.V4 = vOut[3];
            _info.T = tIn[ skipT*i ];
		}
		else
		{
			return -1;
		}
        resInfo.push_back(_info);
	}
    resInfo = AddZeroPoint(resInfo);
    int size = resInfo.size();
	for (int m = 0; m < size; m++)
	{
        res[m * 9] = resInfo[m].P1;
        res[m * 9 + 1] = resInfo[m].P2;
        res[m * 9 + 2] = resInfo[m].P3;
        res[m * 9 + 3] = resInfo[m].P4;
        res[m * 9 + 4] = resInfo[m].V1;
        res[m * 9 + 5] = resInfo[m].V2;
        res[m * 9 + 6] = resInfo[m].V3;
        res[m * 9 + 7] = resInfo[m].V4;
        res[m * 9 + 8] = resInfo[m].T;
	}

    return size;
}
double* CalPose(double* pIn, double* pLast,int index)
{
	// 末端相对水平的夹角
	double theta = pLast[1] + pLast[2] + pLast[3];
	double d = armLen4;
	double px = pIn[0];
	double py = pIn[1];
	double temp = sqrt(px * px + py * py);
	double det = -armLen5 * sin(theta) + armLen3 * cos(theta);
	double t1 = 0;
	//double theta = -Math.PI/2;
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
	// 末端姿态
	double* pose = new double[4*4];
	memset(pose, 0, 16* sizeof(double));
	// 计算关节1角度值
	//double t1 = atan2(pIn[1], pIn[0]);
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
	return pose;
}
double* CalPosition(double* posIn, double* posLast)
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
	double bx = posIn[3] - c0* armLen3 * posIn[9] - armLen4 * s0 + c0 * armLen5 * posIn[8];
	double by = posIn[7] - s0* armLen3 * posIn[9] + armLen4 * c0 + s0 * armLen5 * posIn[8];
	double bz = posIn[11] - armLen3 * posIn[8] - armLen5 * posIn[9];
	double A, B, C;
	if (abs(c0) > 0.1)
	{
		// 计算各关节位置
		A =pow(c0, 2)*(pow(bz - armLen0, 2) - pow(armLen1, 2) - pow(armLen2, 2)) +
			pow(bx, 2);
		B = 2 * pow(c0, 2)* armLen1 * armLen2;
	}
	else
	{
		A = pow(s0, 2)*(pow(bz - armLen0, 2) - pow(armLen1, 2) - pow(armLen2, 2)) +
			pow(by, 2);
		B = 2 * pow(s0, 2)* armLen1 * armLen2;
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
        if (posOrg[4] - posOrg[5] < error )
		{
			mark[1] = 0;
		}
	}
	// 无解
	else
	{
		return NULL;
	}
	for (int i = 0; i < 2; i++)
	{
		double c2 = cos(posOrg[4+i]);
		double s2 = sin(posOrg[4+i]);
		if (abs(c0) > 0.1)
		{
			// 计算各关节位置
			A = -bx*s2* armLen2 + (bz - armLen0)*c0*(armLen1 + c2* armLen2);
			B = (armLen1 + c2* armLen2)*bx + c0*s2* armLen2 * (bz - armLen0);
			C = c0*(pow(armLen1, 2) + 2 * c2* armLen1 * armLen2 + pow(armLen2, 2));
		}
		else
		{
			A = -by*s2* armLen2 + (bz - armLen0)*s0*(armLen1 + c2* armLen2);
			B = (armLen1+ c2* armLen2)*by + s0*s2* armLen2 * (bz - armLen0);
			C = s0*(pow(armLen1, 2) + 2 * c2* armLen1 * armLen2 + pow(armLen2, 2));
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
			return NULL;
		}
	}
	// 角度标准化
	CorrectAngles(posOrg, len);
	// 解的个数及索引位置
	int count = CheckPos(mark, posOrg, len);
	double* result = new double[4];
	switch (count)
	{
		// 无解
	case 0:
		result = new double[4];
		break;
	case 1:
		result = GetPosition(1, posOrg,len);
		break;
	case 2:
		result = GetPosition(2, posOrg,len);
		break;
	case 3:
		result = SelPos(posLast, posOrg,len);
		break;
	default:
		break;
	}
	return result;
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
double* GetPosition(int index, double* input,int len)
{
	double* res = new double[4];
	memset(res, 0, 4 * sizeof(double));
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
	return res;
}
double* SelPos(double* pLast, double* input, int len)
{
	double* temp = new double[2];	
	double* res = new double[4];
	memset(temp, 0, 2 * sizeof(double));
	memset(res, 0, 4 * sizeof(double));
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp[i] = temp[i] + pow(input[2*j+i] - pLast[j], 2);
		}
	}
	if (temp[0] > temp[1])
	{
		res = GetPosition(2, input,len);
	}
	else
	{
		res = GetPosition(1, input,len);
	}
	return res;
}
int CompareAngle(double data1, double date2)
{
	double delt = abs(data1 - date2);
	if (delt > PI)
	{
		delt = abs(delt - 2 * PI);
	}
	if (delt < 1e-3)
	{
		return 0;
	}
	return -1;
}
double * CalV(double* pos, double* v)
{
	// 末端速度
	double c1 = cos(pos[0]);
	double s1 = sin(pos[0]);
	double px = pos[4];
	double py = pos[5];
	double vx = v[0];
	double vy = v[1];
	double* poseV = new double[6];
	memset(poseV, 0, 6 * sizeof(double));
	// 保存末端速度
	poseV[0] = v[0];
	poseV[1] = v[1];
	poseV[2] = v[2];
	// 计算末端角速度
	poseV[5] = (c1 * vy - s1 * vx) / (c1 * px + s1 * py);
	return poseV;
}
double* CalVelocity(double* pos, double* v)
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
	double temp1 = -armLen3 * c1*s2a3a4 - armLen5 * c1*c2a3a4;
	J[0][0] = -armLen1 * s1 * c2 - armLen2 * s1 * c2a3 - s1 * armLen3 * c2a3a4 + armLen4 * c1 + armLen5 * s1*s2a3a4;
	J[0][1] = -armLen1 * c1 * s2 - armLen2 * c1 * s2a3 + temp1;
	J[0][2] = -armLen2 * c1 * s2a3 + temp1;
	J[0][3] = temp1;

	double temp2 = -armLen3 * s1 * s2a3a4 - armLen5 * s1 * c2a3a4;
	J[1][0] = armLen1 * c1 * c2 + armLen2 * c1 * c2a3 + c1 * armLen3 * c2a3a4 + armLen4 * s1 - armLen5 * c1*s2a3a4;
	J[1][1] = -armLen1 * s1 * s2 - armLen2 * s1 * s2a3 + temp2;
	J[1][2] = -armLen2 * s1*s2a3 + temp2;
	J[1][3] = temp2;

	double temp3 = armLen3 * c2a3a4 - armLen5 * s2a3a4;
	J[2][1] = armLen1 * c2 + armLen2* c2a3 + temp3;
	J[2][2] = armLen2 * c2a3 + temp3;
	J[2][3] = temp3;

	J[3][1] = s1;
	J[3][2] = s1;
	J[3][3] = s1;

	J[4][1] = -c1;
	J[4][2] = -c1;
	J[4][3] = -c1;

	J[5][0] = 1;
	double* vOut = new double[4];
	memset(vOut, 0, 4 * sizeof(double));
	vOut[0] = v[5];
	//奇偶规避
	double P = BizarreAvoid(s3);
	if (abs(c1) > 0.1)
	{
		double A = J[0][1] - J[0][3];
		double B = J[0][2] - J[0][3];
		double C = J[2][1] - J[2][3];
		double D = J[2][2] - J[2][3];

		double b1 = v[0] - (J[0][0] * vOut[0] + J[0][3] * v[4] / J[4][3]);
		double b2 = v[2] - v[4] * J[2][3] / J[4][3];

		P = P / (armLen1 * armLen2 * c1);
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

		P = P / (armLen1 * armLen2 * s1);
		vOut[1] = (D*b1 - B*b2)*P;
		vOut[2] = -(C*b1 - A*b2)*P;
		vOut[3] = v[3] / J[3][3] - vOut[1] - vOut[2];
	}
	return vOut;
}
double BizarreAvoid(double input)
{
	double temp;
	if (abs(input) < 0.1)
	{
		temp = 0.1 * (1 - abs(input) / 0.1);
	}
	else
	{
		temp = 0;
	}
	return input / (pow(input, 2) + pow(E, -abs(input)) * pow(temp, 2));
}
double* GetContinueAngle(double* last, double* pos)
{
	double* res = new double[4];
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
	return res;
}

std::vector<ResInfo> AddZeroPoint(std::vector<ResInfo> infoList)
{
    std::vector<ResInfo> resList;
	int count = infoList.size();
    double** tempP = new double*[count];Q_ASSERT( NULL != tempP );
    double** tempV = new double*[count];Q_ASSERT( NULL != tempV );
	for (int index = 0; index < count; index++)
	{
        tempP[index] = new double[4];Q_ASSERT( NULL != tempP[index] );
        tempV[index] = new double[4];Q_ASSERT( NULL != tempV[index] );
	}
	double* tempT = new double[count];
    Q_ASSERT( tempT != NULL );
	for (int i = 0; i < count; i++)
	{
		tempP[i][0] = infoList[i].P1;
		tempP[i][1] = infoList[i].P2;
		tempP[i][2] = infoList[i].P3;
		tempP[i][3] = infoList[i].P4;
		tempV[i][0] = infoList[i].V1;
		tempV[i][1] = infoList[i].V2;
		tempV[i][2] = infoList[i].V3;
		tempV[i][3] = infoList[i].V4;
		tempT[i] = infoList[i].T;
	}
	// 存在速度反向点的时间区间的个数
	int tCount = 0;
	// 一个时间区间内速度反向点的个数
	int tInnerCount = 0;
	// 保存一个区间的多个零点左区间下标
	int* selZCP = new int[4];
	// 保存一个区间的左边界至多个零点区间的比例
	double* selSca = new double[4];
	// 记录过零点区间左边界元素下标
	int* ZCP = new int[4 * count - 1];
	// 记录过零点所在区间比例
	double* sca = new double[4 * count - 1];
	for (int k = 0; k < count - 1; k++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (tempV[k][j] * tempV[k + 1][j] < 0)
			{
                if (abs(tempV[k][j]) < error )
				{
					tempV[k][j] = 0;
				}
                else if (abs(tempV[k + 1][j]) < error )
				{
					tempV[k + 1, j] = 0;
				}
				else
				{
					selZCP[tInnerCount] = k + 1;
					selSca[tInnerCount] = abs(tempV[k][ j] / (tempV[k + 1][ j] - tempV[k][j]));
					tCount++;
					tInnerCount++;
				}
			}
		}
		// 零点排序，保证时间为递增关系（sca与时间成正比关系）
		selSca = Rank(selSca,4);
		for (int m = 0; m < tInnerCount; m++)
		{
			int n = tCount - tInnerCount + m;
			ZCP[n] = selZCP[m];
			sca[n] = selSca[4 - tInnerCount + m];
		}
		tInnerCount = 0;
	}
	double** resP = new double*[count + tCount];
	double** resV = new double*[count + tCount];
	for (int p = 0; p < count + tCount; p++)
	{
		resP[p] = new double[4];
		resV[p] = new double[4];
	}
	double* resT = new double[count + tCount];
    Q_ASSERT( NULL != resT );
	// 零点处插值
	int index1 = 0, index2 = 0;
	for (int a = 0; a < 4; a++)
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
                        if (abs(resV[index1][a]) < error)
						{
							resV[index1, a] = 0;
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
	resList = DeleteRedundantPVT(resP, resV, resT,plen);
	return resList;
}
double* Rank(double* input,int len)
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
std::vector<ResInfo> DeleteRedundantPVT(double** inputP, double** inputV, double* inputT,int len)
{
    std::vector<ResInfo> lresInfo;
	double** tempP = new double*[len];
	double** tempV = new double*[len];
	double* tempT = new double[len];
	for (int i = 0; i < len; i++)
	{
		tempP[i] = new double[4];
		tempV[i] = new double[4];
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
            while ( fabs(inputT[index1 + index3] - inputT[index1] ) < error )
			{
				index3++;
			}
		}
		for (int i = 0; i < 4; i++)
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
		res.P3 = tempP[j][2];
		res.P4 = tempP[j][3];
		res.V1 = tempV[j][0];
		res.V2 = tempV[j][1];
		res.V3 = tempV[j][2];
		res.V4 =tempV[j][3];
		res.T = tempT[j];
        lresInfo.push_back(res);
	}
    return lresInfo;
}

}
