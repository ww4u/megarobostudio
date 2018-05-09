#include "params.h"
#include "math.h"
#include "string.h"
/// 求解函数的导出函数
/// 输入参数：1：armLength -- 各轴长度， 2：armLim 两关节角度范围限制，3：P0--末端初始位置，用于筛选解
/// 4:posIn -- 逆解为输入的末端点，正解为关节角度， 5：vin -- 正解为0 逆解为末端速度，6:len -- 求解点的个数，7：mode --求解模式 0为正解 ，1为逆解
/// 输出参数：res-- 求解的返回值
/*extern "C" __declspec(dllexport) */int SolveStructureKinematics(double* armLength, double*armLim, double* P0, double* posIn, double*vIn,double* tIn, int len, int mode,double* res)
{
	// 正解角度转换
	if (mode == 0)
	{
		for (int i = 0; i < len; i++)
		{
			posIn[2 * i] = posIn[2 * i] * PI / 180;
			posIn[2 * i + 1] = posIn[2 * i + 1] * PI / 180;
		}
	}	
	SetStructInfo(armLength, armLim, P0);	
	// 进行正逆解求解
	if (mode == 0)
	{
		// 正解
		return PositiveCalc(posIn,tIn,len,res);		
	}
	else
	{
		//逆解
		return OppositeCalc(posIn, vIn,tIn, len, res);
	}
}
int PositiveCalc(double* pIn,double* t,int length,double* result)
{
	memset(result, 0, 3 * length * sizeof(double));
	//int res = JudgeAngle(pIn, length);
	//// 角度超出区间范围
	//if (res == -1)
	//{
	//	return -1;
	//}
	// 角度转换
	AngleTrans(pIn, length,0);
	/* 判断末端执行器是否在工作区间内 */
	for (int i = 0; i < length; i++)
	{
		double tmp1 = armLen.Ll * pIn[2*i];
		double tmp2 = armLen.Lr * pIn[2 * i +1];
		// 计算x
		result[2*i] = 0.25 * (tmp1 - tmp2);			
		// 工作区间判断												
		if (fabs(result[2 * i] - workSpace[0])<ERROR
			|| fabs(result[2 * i] - workSpace[1])<ERROR)
		{
			return ERROR_OUT_OF_WORK_SPACE;
		}
		// 计算y
		result[2 * i + 1] = 0.25 * (tmp1 + tmp2);
		// T
		result[2 * i + 2] = t[i];
		if (fabs(result[2 * i + 1] - workSpace[2])<ERROR || fabs(result[2 * i + 1] - workSpace[3])<ERROR)
		{
			return ERROR_OUT_OF_WORK_SPACE;
		}
	}

        return 0;
}
int OppositeCalc(double* pIn,double* v,double* t, int length, double* result)
{
	double temp[2];
	memset(result, 0, 5 * length * sizeof(double));
	// 判断末端执行器是否在工作区间内
	for (int i = 0; i < length; i++)
	{
		
		/* x超出工作区间 */
		/*if (abs(pIn[2 * i] - workSpace[0])<ERROR
			|| abs(pIn[2 * i] - workSpace[1])<ERROR)
		{
			return ERROR_OUT_OF_WORK_SPACE;
		}*/			
		/* y超出工作区间 */
		/*if (abs(pIn[2 * i + 1] - workSpace[2])<ERROR || abs(pIn[2 * i + 1] - workSpace[3])<ERROR)
		{
			return ERROR_OUT_OF_WORK_SPACE;
		}*/
		// 逆解求解
		temp[0] = 2.0 * (pIn[2 * i] + pIn[2 * i + 1]) / armLen.Ll;
		temp[1] = 2.0 * (-pIn[2 * i] + pIn[2 * i + 1]) / armLen.Lr;
		// 角度转换
		AngleTrans(temp, length,1);
		//int res = JudgeAngle(pIn, length);
		//// 角度超出区间范围
		//if (res == ERROR_OUT_OF_ANGLE_LIMIT)
		//{
		//	return ERROR_OUT_OF_ANGLE_LIMIT;
		//}
		result[5 * i] = temp[0] *  180 / PI;
		result[5 * i + 1] = temp[1] * 180 / PI;
		// 速度逆解
		result[5 * i +2] = 2.0 * (v[2*i] + v[2 * i+1]) / armLen.Ll * 180 / PI;
		result[5 * i +3] = 2.0 * (-v[2 * i] + v[2 * i + 1]) / armLen.Lr * 180 / PI;
		result[5 * i + 4] = t[i];
	}
	return SUCCESS;
}
void SetStructInfo(double* armLength,double* armLim,double*P0)
{
	// 机构参数设置
	armLen.Lr = armLength[0];
	armLen.Ll = armLength[1];
	armLen.Lw = armLength[2];
	armLen.Lh = armLength[3];
	armLen.Lb = armLength[4];
	armLen.Lp = armLength[5];
	angleLim[0] = armLim[0];
	angleLim[1] = armLim[1];
	angleLim[2] = armLim[2];
	angleLim[3] = armLim[3];
	correctData.X = P0[0];
	correctData.Y = P0[1];
	correctData.deltAngleX = -2 * (P0[0] + P0[1]) / armLen.Ll;
	correctData.deltAngleY = -2 * (-P0[0] + P0[1]) / armLen.Lr;
	// 工作区间
	double tmp1 = 0;
	double tmp2 = 0;
	if (armLen.Ll >= armLen.Lr)
		tmp1 = (armLen.Ll + armLen.Lb) / 2.0;
	else
		tmp1 = (armLen.Lr + armLen.Lb) / 2.0;
	// Xmin
	workSpace[0] = tmp1;
	// Xmax
	workSpace[1] = tmp1 + armLen.Lh - armLen.Lb;		

	tmp2 = (armLen.Lw - armLen.Lp) / 2.0;
	// Ymin
	workSpace[2] = -tmp2;
	// Ymax
	workSpace[3] = tmp2;			
}
int JudgeAngle(double* p,int len)
{
	double temp[2];
	for (int i = 0; i < len; i++)
	{
		temp[0] = p[2 * i];
		temp[1] = p[2 * i +1];
		if (temp[0] >= angleLim[0] && temp[0] < angleLim[1]
			&& temp[1] >= angleLim[2] && temp[1] < angleLim[3])
		{
			continue;
		}
		else
		{
			return ERROR_OUT_OF_ANGLE_LIMIT;
		}
	}
	return SUCCESS;
}

void AngleTrans(double *p,int len, int mode)
{
	int i;
	double sign;

	if (mode == 0)		// 正解
	{
		sign = -1.0;
		for (i = 0; i < len; i++)		// 输入/出角度转换
		{
			p[2 * i] += sign * correctData.deltAngleX;		// 输入/出角度1转换
			p[2 * i + 1] += sign * correctData.deltAngleY;		// 输入/出角度2转换
		}
	}
	else		//逆解
	{
		sign = 1.0;
		p[0] += sign * correctData.deltAngleX;		// 输入/出角度1转换
		p[1] += sign * correctData.deltAngleY;		// 输入/出角度2转换
	}

	
}
