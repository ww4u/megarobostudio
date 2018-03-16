/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvtScurveSolution.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.11.23;
历史版本:  无;
*********************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "arm_math.h"
#include "pvtScurveSolution.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/
#define    MAXDEGREE        3
#define    VELCMAXDEGREE    MAXDEGREE + 1
#define    POSTMAXDEGREE    MAXDEGREE + 2

#define    SOLVING      0
#define    BUILDERROR   1



/*************************************局部常量和类型定义**************************************/
typedef enum
{
    CURVE_ACC = 0,
    CURVE_UNF,
    CURVE_DEC
    
}CurvetYPEEnum;

typedef struct
{
    f32  postCoeff[POSTMAXDEGREE];    // 位置多项式系数
	f32  velcCoeff[VELCMAXDEGREE];    // 速度多项式系数
	
	int type;
    
}PolynomialCoeffStruct;

typedef struct
{
    PvtPointStruct accStartPoint;    //加速段起始位置  
    PvtPointStruct accEndPoint;      //加速段结束位置
  
    u8  accScale;        //加速段占比，不超过50%
    
    PvtPointStruct decStartPoint;    //减速段起始位置
    PvtPointStruct decEndPoint;      //减速段结束位置
    
    u8  decScale;        //减速段占比，不超过50%

    PolynomialCoeffStruct polyCoeff[3];    //各段曲线的多项式系数
      
}ScurveInfoStruct;

typedef struct
{
    f32 velocity[2];
    f32 time[2];
    f32 refPoint[2];

    f32 position[2];    //匀速段使用
    
}PolynomialParaStruct;

typedef struct
{
    f32 solution[MAXDEGREE + 1];

    u8  solutionNum;
    
}SolutionInfoStruct;



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvtPolynomialCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 计算多项式及其导数值;
*********************************************************************************************/
int reviseCoefficient(f32 coeff[], int arrayLength)
{
	f32 err = 1e-20;		// 定义误差
	int i;

	if (arrayLength == 0)		// 数组长度为零，直接返回
		return 0;

	for (i=0;i<arrayLength;i++)
		if (fabs(coeff[i])> 0)		// 记录无效高次项系数
			break;

	if (fabs(coeff[i]) < err)		// 系数无效，返回0
		return 0;
	else
		return arrayLength - i;		// 输出有效项数
}


/*********************************************************************************************
函 数 名: pvtPolynomialCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 计算多项式及其导数值;
*********************************************************************************************/
f32 pvtPolynomialCalc(f32 coeff[], f32 x, int arrayLength, int mode)
{
	f32 b[6], c;
	int i;
	int length, index;

	length = reviseCoefficient(coeff, arrayLength);		// 获取有效项数
	index = arrayLength - length;		// 有效系数起始下标

	if (length == 0)		// 无有效系数
		return 0;

	b[index] = coeff[index];
	for (i = index+1;i < arrayLength;i++)
		b[i] = coeff[i] + x*b[i-1];
	if (mode == 0)		
		return b[i-1];		// 返回多项式的值
	else
	{
		if (length > 1)
		{
			c = b[index];
			for (i = index+1;i < arrayLength - 1;i++)
				c = b[i] + x*c;
		}
		else
		{
			c = 0;
		}
		return c;		// 返回导数值
	}
}


/*********************************************************************************************
函 数 名: pvtLinearPolynomialBuild;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 依据始末点位置构建线性函数;
*********************************************************************************************/
u8 pvtLinearPolynomialBuild(PolynomialCoeffStruct *coefficient, PolynomialParaStruct polyPara)
{
	f32 slope;


	slope = (polyPara.position[1] - polyPara.position[0]) / 
	        (polyPara.time[1]     - polyPara.time[0]);

	coefficient->velcCoeff[3] = slope;		// 计算速度多项式参数

	// 计算位置多项式参数
	coefficient->postCoeff[3] = coefficient->velcCoeff[3];
	coefficient->postCoeff[4] = polyPara.position[0] - slope * polyPara.time[0];

	coefficient->type = 0;		// 设置标志位

	return SOLVING;		// 正常
}


/*********************************************************************************************
函 数 名: pvtStepPolynomialBuild;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 依据始末点速度构建STEP多项式;
*********************************************************************************************/
u8 pvtStepPolynomialBuild(PolynomialCoeffStruct *coefficient, PolynomialParaStruct polyPara)
{
    f32 slope;


    switch(coefficient->type)		// 检测所构造的多项式次数是否超出定义范围
    {
        case 0:			// 零次多项式，即常数
            if (MAXDEGREE < 0)		// 定义的多项式次数为负，退出
            {
                return BUILDERROR;
            }
          break;
          
        case 1:			// 一次多项式
            if (MAXDEGREE < 1)		// 构建的多项式次数超出范围，退出
            {
                return BUILDERROR;
            }
          break;
        
        case 2:			// 三次多项式
            if (MAXDEGREE < 3)		// 构建的多项式次数超出范围，退出
            {
                return BUILDERROR;
            }
          break;
        
        case 3:			// 五次多项式
            if (MAXDEGREE < 5)		// 构建的多项式次数超出范围，退出
            {
                return BUILDERROR;
            }
          break;
        
        default :				// 构建的多项式次数不合理，退出
            return BUILDERROR;
          break;
    }

    switch (coefficient->type)
    {
        case 1:			// STEP1多项式求解
            slope = 1.0f / (polyPara.time[1] - polyPara.time[0]);
            coefficient->velcCoeff[MAXDEGREE - 1] = slope * (polyPara.velocity[1] - polyPara.velocity[0]);		// 计算速度多项式参数
            coefficient->velcCoeff[MAXDEGREE]     = slope * (polyPara.velocity[0] * polyPara.time[1] - 
                                                             polyPara.velocity[1] * polyPara.time[0]);

            coefficient->postCoeff[MAXDEGREE - 1] = coefficient->velcCoeff[2] * 0.5f;		// 计算位置多项式参数
            coefficient->postCoeff[MAXDEGREE]     = coefficient->velcCoeff[3];
            coefficient->postCoeff[MAXDEGREE + 1] = polyPara.refPoint[1] - 
                                                    pvtPolynomialCalc(coefficient->postCoeff, 
                                                                      polyPara.refPoint[0], 
                                                                      POSTMAXDEGREE, 
                                                                      0);		// 求解积分常数项
          break;

        case 2:			// STEP3多项式求解
            slope = (polyPara.velocity[1] - polyPara.velocity[0]) / pow((polyPara.time[1] - polyPara.time[0]), 3.0);
            coefficient->velcCoeff[MAXDEGREE - 3] = -2.0f * slope;		// 计算速度多项式参数
            coefficient->velcCoeff[MAXDEGREE - 2] =  3.0f * slope * (polyPara.time[0] + polyPara.time[1]);
            coefficient->velcCoeff[MAXDEGREE - 1] = -6.0f * slope * polyPara.time[0] * polyPara.time[1];
            coefficient->velcCoeff[MAXDEGREE]     = slope * polyPara.time[0] * polyPara.time[0] * 
                                                    (3.0f * polyPara.time[1] - polyPara.time[0]) + polyPara.velocity[1];

            coefficient->postCoeff[MAXDEGREE - 3] = coefficient->velcCoeff[0] * 0.25f;		// 计算位置多项式参数
            coefficient->postCoeff[MAXDEGREE - 2] = coefficient->velcCoeff[1] / 3.0;
            coefficient->postCoeff[MAXDEGREE - 1] = coefficient->velcCoeff[2] * 0.5f;
            coefficient->postCoeff[MAXDEGREE]     = coefficient->velcCoeff[3];
            coefficient->postCoeff[MAXDEGREE + 1] = polyPara.refPoint[1] - 
                                                    pvtPolynomialCalc(coefficient->postCoeff, 
                                                                      polyPara.refPoint[0], 
                                                                      POSTMAXDEGREE, 
                                                                      0);		// 求解积分常数项
          break;
    }

    return SOLVING;		// 正常
}


/*********************************************************************************************
函 数 名: pvtScurveBuild;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 构建S曲线: 求解轨迹多项式系数及节点参数;
*********************************************************************************************/
u8 pvtScurveBuild(ScurveInfoStruct *pScurveInfo)
{
    u8  buildResult = SOLVING;
    f32 motionTime;
    f32 accTime;
    f32 decTime;
    f32 postOffset;
    f32 maxSpeed; 
    PolynomialParaStruct polyPara;
    

    //运动时间
    motionTime = pScurveInfo->decEndPoint.time - pScurveInfo->accStartPoint.time;
    accTime = motionTime * pScurveInfo->accScale / 100.0f;
    decTime = motionTime * pScurveInfo->decScale / 100.0f;

    //运动距离
    postOffset = pScurveInfo->decEndPoint.position - pScurveInfo->accStartPoint.position;

    //匀速运动速度，也是最大速度
    //加减速不对称的情况下: Vmax = (P - V0 * Tacc * 0.5 - V1 * Tdec * 0.5)/(T - 0.5 * (Tacc + Tdec))
    //加减速  对称的情况下: Vmax = (P - (V0 + V1) * Tacc * 0.5)/(T - Tacc)
    /*maxSpeed = (2.0f * postOffset - pScurveInfo->accStartPoint.speed * accTime - pScurveInfo->decEndPoint.speed * decTime) / 
               (2.0f * motionTime - (accTime + decTime));*/
    maxSpeed = (postOffset - (pScurveInfo->accStartPoint.speed + pScurveInfo->decEndPoint.speed) * accTime * 0.5f) / 
               (motionTime - accTime);

    /**********************************构建第一段曲线**********************************/
    //起点速度，终点速度
    polyPara.velocity[0] = pScurveInfo->accStartPoint.speed;
    polyPara.velocity[1] = maxSpeed;
    
    //起点时间，终点时间
    polyPara.time[0] = 0;
    polyPara.time[1] = accTime;

    //参考点时间，参考点位置
    polyPara.refPoint[0] = 0;
    polyPara.refPoint[1] = pScurveInfo->accStartPoint.position;
    
    buildResult = pvtStepPolynomialBuild(&(pScurveInfo->polyCoeff[0]), polyPara);
    if (buildResult != SOLVING)    //构建失败
    {
        return buildResult;
    }

    //计算第一段曲线末端信息(位置，速度，时间)
    pScurveInfo->accEndPoint.position = pvtPolynomialCalc(pScurveInfo->polyCoeff[0].postCoeff, accTime, POSTMAXDEGREE, 0);
    pScurveInfo->accEndPoint.speed    = pvtPolynomialCalc(pScurveInfo->polyCoeff[0].velcCoeff, accTime, VELCMAXDEGREE, 0);
    pScurveInfo->accEndPoint.time     = accTime;
    /**********************************构建第一段曲线**********************************/


    /**********************************构建第三段曲线**********************************/
    //起点速度，终点速度
    polyPara.velocity[0] = maxSpeed;
    polyPara.velocity[1] = pScurveInfo->decEndPoint.speed;
    
    //起点时间，终点时间
    polyPara.time[0] = 0;
    polyPara.time[1] = accTime;

    //参考点时间，参考点位置
    polyPara.refPoint[0] = accTime;
    polyPara.refPoint[1] = pScurveInfo->decEndPoint.position;
    
    buildResult = pvtStepPolynomialBuild(&(pScurveInfo->polyCoeff[2]), polyPara);
    if (buildResult != SOLVING)    //构建失败
    {
        return buildResult;
    }

    //计算第三段曲线始端信息(位置，速度，时间)
    pScurveInfo->decStartPoint.time = pScurveInfo->decEndPoint.time - accTime;
    pScurveInfo->decStartPoint.position = pvtPolynomialCalc(pScurveInfo->polyCoeff[2].postCoeff, 0, POSTMAXDEGREE, 0);
    pScurveInfo->decStartPoint.speed    = pvtPolynomialCalc(pScurveInfo->polyCoeff[2].velcCoeff, 0, VELCMAXDEGREE, 0);
    /**********************************构建第三段曲线**********************************/


    /**********************************构建第二段曲线**********************************/
    //起点速度，终点速度
    polyPara.position[0] = pScurveInfo->accEndPoint.position;
    polyPara.position[1] = pScurveInfo->decStartPoint.position;
    
    //起点时间，终点时间
    polyPara.time[0] = 0;
    polyPara.time[1] = motionTime - 2.0f * accTime;
    
    buildResult = pvtLinearPolynomialBuild(&(pScurveInfo->polyCoeff[1]), polyPara);

    return buildResult;
}


/*********************************************************************************************
函 数 名: solveQuartic
实现功能: 计算四次多项式的值
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*********************************************************************************************/
void solveLinear(f32 coeff[], SolutionInfoStruct *pSolution)
{
	double a, b, err;

	err = 1e-10;		// 定义误差
	a = coeff[0];		// 一次项系数
	b = coeff[1];		// 常数项系数

	if (fabs(a) < err)		// 无解
	{
		pSolution->solutionNum = 0;
		pSolution->solution[0] = 0;
	}
	else		// 唯一解
	{
		pSolution->solutionNum = 1;
		pSolution->solution[0] = -b/a;
	}
}


/*********************************************************************************************
函 数 名: solveQuartic
实现功能: 计算四次多项式的值
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*********************************************************************************************/
void solveQuadratic(f32 coeff[], SolutionInfoStruct *pSolution)
{
	f32 a, b, c, err;
	f32 coeffLinear[2] = {coeff[1], coeff[2]};
	f32 det;

	err = 1e-20;		// 定义误差
	a = coeff[0];		// 二次次项系数
	b = coeff[1];		// 一次项系数
	c = coeff[2];		// 常数项系数

	if (fabs(a)<err||fabs(a/b)<1e-10)		// 无解
	{
		solveLinear(coeffLinear, pSolution);		// 线性多项式求解
	}
	else		// 唯一解
	{
		det = b*b - 4.0*a*c;		// 判别式
		if (det < -err)		// 无解
		{
			pSolution->solutionNum = 0;
			pSolution->solution[0] = 0;
		}
		else
		{
			det = fabs(det);
			if (det < err)		// 一个重根
			{
				pSolution->solutionNum = 1;
				pSolution->solution[0] = -b/(2.0*a);
			}
			else		// 两个重根
			{
				pSolution->solutionNum = 2;
				det = sqrt(det);
				pSolution->solution[0] = (-b+det)/(2.0*a);
				pSolution->solution[1] = (-b-det)/(2.0*a);
			}
		}
	}
}


/*********************************************************************************************
函 数 名: solveQuartic
实现功能: 计算四次多项式的值
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*********************************************************************************************/
void solveCubic(f32 coeff[], SolutionInfoStruct *pSolution)
{
	f32 a, b, c, d, err;
	f32 coeffQuadratic[3] = {coeff[1], coeff[2], coeff[3]};
	f32 A, B, C, det, K, K1, K2, sgn1, sgn2, theta;

	err = 1e-30;		// 定义误差
	a = coeff[0];		// 三次次项系数
	b = coeff[1];		// 二次次项系数
	c = coeff[2];		// 一次项系数
	d = coeff[3];		// 常数项系数

	if (fabs(a) < err)
		solveQuadratic(coeffQuadratic,pSolution);		// 二次多项式求解
	else
	{
		A = b*b - 3.0*a*c;
		B = b*c - 9.0*a*d;
		C = c*c - 3.0*b*d;
		det = B*B - 4.0*A*C;

		if (fabs(det) < err)		// det=0
		{
			if (fabs(A) < err)		// A=B=0,一个三重根
			{
				pSolution->solutionNum = 1;
				pSolution->solution[0] = -b/(3.0*a);
			}
			else		// A!=0: 两个根
			{
				K = B/A;

				pSolution->solutionNum = 2;
				pSolution->solution[0] = -b/a + K;
				pSolution->solution[1] = -K/2.0;
			}
		}
		else if(det>0)
		{
			det = sqrt(det);
			K1 = A*b + 3.0*a*(-B + det)/2.0;
			K2 = A*b + 3.0*a*(-B - det)/2.0;
			sgn1 = 1.0;
			sgn2 = 1.0;

			if (K1 < 0)
				sgn1 = -1.0;
			if (K2 < 0)
				sgn2 = -1.0;

			pSolution->solutionNum = 1;		// 一个根
			pSolution->solution[0] = (-b - sgn1*pow(K1, 1/3.0) - sgn2*pow(K2, 1/3.0))/(3.0*a);
		}
		else		// det>0
		{
			if (A > 0)
			{
				K = (2.0*A*b - 3.0*a*B)/(2.0*A*sqrt(A));
				if (K <= 1 && K >= -1)
				{
					theta = acos(K)/3.0;
					A = sqrt(A);

					pSolution->solutionNum = 3;		// 三个根
					pSolution->solution[0] = (-b - 2.0*cos(theta)*A)/(3.0*a);
					pSolution->solution[1] = (-b + (cos(theta) + sin(theta) * sqrt(3.0))*A)/(3.0*a);
					pSolution->solution[2] = (-b + (cos(theta) - sin(theta) * sqrt(3.0))*A)/(3.0*a);
				}
				else
				{
					pSolution->solutionNum = 0;		// 无实根
					pSolution->solution[0] = 0;
				}
			}
			else
			{
				//pSolution->solutionNum = -1;		// 无实根
				pSolution->solutionNum = 0;
				pSolution->solution[0] = 0;
			}
		}
	}
}


/*********************************************************************************************
函 数 名: solveQuartic
实现功能: 计算四次多项式的值
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*********************************************************************************************/
void solveQuartic(f32 coeff[], SolutionInfoStruct *pSolution)
{
	f32 a, b, c, d, e, err;
	f32 coeffCubic[4] = { coeff[1], coeff[2], coeff[3], coeff[4] };
	f32 A, B, C, D, E, G, F, det, K, K1, K2, K3, sgn, sgn1, sgn2, T, theta, Y1, Y2, W;
	f32 temp, temp1, temp2;

	err = 1e-30;		// 定义误差
	a = coeff[0];		// 四次次项系数
	b = coeff[1];		// 三次次项系数
	c = coeff[2];		// 二次次项系数
	d = coeff[3];		// 一次项系数
	e = coeff[4];		// 常数项系数

	if (fabs(a) < err)		// 三次多项式求解
		solveCubic(coeffCubic, pSolution);
	else
	{
		D = -(3.0*b*b-8.0*a*c);
		E = 3.0*pow(b,4.0)+16.0*a*a*c*c-16.0*a*b*b*c+16.0*a*a*b*d-64.0*pow(a,3)*e;
		G = -(pow(b,3.0)-4.0*a*b*c+8.0*a*a*d);
		F = -G*G;

		A = D*D-3.0*E;
		B = D*E-9.0*F;
		C = E*E-3.0*D*F;

		det = B*B-4.0*A*C;		// 总判别式
		sgn = 1.0;
		if(G<0)
			sgn = -1.0;

		if (fabs(D) < err&&fabs(E) < err&&fabs(F) < err)		// D=E=F: 一个四重根
		{
			pSolution->solutionNum = 1;		
			pSolution->solution[0] = -b/(4.0*a);
		}
		else
		{
			if (fabs(E) < err&&fabs(F) < err)		// D!=0,E=F=0: 两个两重根
			{
				if (D < 0)
				{
					temp = sqrt(-D);

					pSolution->solutionNum = 2;
					pSolution->solution[0] = (-b + temp)/(4.0*a);
					pSolution->solution[1] = (-b - temp)/(4.0*a);
				}
				else		// 无解
				{
					pSolution->solutionNum = 0;
					pSolution->solution[0] = 0;
				}
			}
			else
			{
				if (fabs(det) < err)		// det = 0
				{
					if (fabs(A) < err)		// A = 0: 两个根
					{
						temp1 = -b/(4.0*a);
						temp2 = -G/ (4.0*a*D);

						pSolution->solutionNum = 2;
						pSolution->solution[0] = temp1-9.0*temp2;
						pSolution->solution[1] = temp1+3.0*temp2;
					}
					else
					{
						K1 = -D+B/A;
						K2 = -B/(2.0*A);

						if(fabs(K1)<err)
							K1 = 0;
						if(fabs(K2)<err)
							K2 = 0;

						if (K1 >= 0)
						{
							temp1 = sqrt(K1);
							if (K2 >= 0)		// 三个根
							{
								temp2 = sqrt(K2);

								pSolution->solutionNum = 3;
								pSolution->solution[0] = (-b + sgn*temp1 + 2.0*temp2)/(4.0*a);
								pSolution->solution[1] = (-b + sgn*temp1 - 2.0*temp2)/(4.0*a);
								pSolution->solution[2] = (-b - sgn*temp1)/(4.0*a);
							}
							else		// 一个根
							{
								pSolution->solutionNum = 1;
								pSolution->solution[0] = (-b + sgn*temp1)/(4.0*a);
							}
						}
						else		// 无解
						{
							//(*solution).flag = -1;
							pSolution->solutionNum = 0;
							pSolution->solution[0] = 0;
						}
					}
				}
				else
				{
					if (det < 0)
					{
						if (A > 0)
						{
							T = (2.0*A*D-3.0*B)/(2.0*A*sqrt(A));
							if (T > 1 || T < -1)		// 无解
							{
								//(*solution).flag = -2;
								pSolution->solutionNum = 0;
								pSolution->solution[0] = 0;
							}
							else
							{
								theta = acos(T)/3.0;
								K = 2*sqrt(A);
								K1 = -(D + K*cos(theta))/3.0;
								K2 = -(D + K*cos(theta + 2.0/3.0*PI_VALUE))/3.0;
								K3 = -(D + K*cos(theta - 2.0/3.0*PI_VALUE))/3.0;
							}

							if(fabs(K1)<err)
								K1 = 0;
							if (fabs(K2)<err)
								K2 = 0;
							if (fabs(K2)<err)
								K2 = 0;

							if (K1 >= 0 && K2 >= 0 && K3 >= 0)		// 四个根
							{
								K1 = sgn*sqrt(K1);
								K2 = sgn*sqrt(K2);
								K3 = sgn*sqrt(K3);

								pSolution->solutionNum = 4;
								pSolution->solution[0] = (-b + K1 + K2 + K3)/(4.0*a);
								pSolution->solution[1] = (-b + K1 - K2 - K3)/(4.0*a);
								pSolution->solution[2] = (-b - K1 - K2 + K3)/(4.0*a);
								pSolution->solution[3] = (-b - K1 + K2 - K3)/(4.0*a);
							}
							else		// 无解
							{
								//(*solution).flag = -3;
								pSolution->solutionNum = 0;
								pSolution->solution[0] = 0;
							}
						}
						else		// 无解
						{
							//(*solution).flag = -4;
							pSolution->solutionNum = 0;
							pSolution->solution[0] = 0;
						}
					}
					else		// det>0
					{
						det = sqrt(det);
						Y1 = A*D + (-B + det)*1.5;
						Y2 = A*D + (-B - det)*1.5;
						sgn1 = 1;
						sgn2 = 1;
						if(Y1<0)
							sgn1 = -1.0;
						if (Y2<0)
							sgn2 = -1.0;

						Y1 = sgn1*pow(fabs(Y1), 1/3.0);
						Y2 = sgn2*pow(fabs(Y2), 1/3.0);

						K1 = (-2.0*D+Y1+Y2)/6.0;
						K2 = sqrt(3.0)*(Y1-Y2)/6.0;
						K = -(D+Y1+Y2)/3.0;

						temp = 2.0*sqrt(K1*K1+K2*K2);
						W = sqrt(2.0*K1+temp);

						if(fabs(K)<err)
							K = 0;

						if (K >= 0)			// 两个根
						{
							K = sgn*sqrt(K);

							pSolution->solutionNum = 2;
							pSolution->solution[0] = (-b + K + W)/(4.0*a);
							pSolution->solution[1] = (-b + K - W)/(4.0*a);
						}
						else		// 无解
						{ 
							//(*solution).flag = -5;
							pSolution->solutionNum = 0;
							pSolution->solution[0] = 0;
						}
					}
				}
			}
		}
	}
}


/*********************************************************************************************
函 数 名: isInRegion
实现功能: 检测参数是否在区间内
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*********************************************************************************************/
bool isInRegion(f32 value, f32 maxValue, f32 minValue)
{
	if ((value >= minValue) && (value <= maxValue))
	{
	    return true;
	}
	else
	{
	    return false;
	}
}


/*********************************************************************************************
函 数 名: pvtScurveSolution
实现功能: S曲线计算，先根据PVT生成三段曲线:加速段，匀速段和减速段
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*********************************************************************************************/
bool pvtSingalCurveCalc(PolynomialCoeffStruct polyCoeff, f32 *pCalcTime, f32 startTime, f32 endTime)
{
	u8   i;
	bool inRegion;
	SolutionInfoStruct solution;

			
	solveQuartic(polyCoeff.postCoeff, &solution);    // 求解四次及以下次多项式

	// 判断解的有效性
	if (solution.solutionNum > 0)		// 多项式在全局范围内有解
	{
		for (i = 0;i < solution.solutionNum;i++)		//判断解是否在指定区间内
		{
			inRegion = isInRegion(solution.solution[i], endTime, startTime);    // 判断解是否在区间内
			
			if (inRegion)    // 解在区间内
			{
				*pCalcTime = solution.solution[i];    // 保存唯一解
				
				return true;    // 退出循环
			}
		}
	}
	
	return false;
}


/*********************************************************************************************
函 数 名: pvtScurveSolution
实现功能: S曲线计算，先根据PVT生成三段曲线:加速段，匀速段和减速段
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*********************************************************************************************/
u8 pvtCurveSolution(u8 chanNum, ScurveInfoStruct scurveInfo, PvtCalcStruct *pPvtCalcData, CurvetYPEEnum curveType)
{
    u8   errorCode = PVT_CALC_NO_ERROR;
    bool solutionResult;
    u32  i;
    u32  startPosition;
    u32  endPosition;
    u32  poseOffset;
    
    f32  calcOutput;
    f32  startTime;
    f32  endTime;
    f32  calcTime;
    PolynomialCoeffStruct realPolyCoeff;
    PolynomialCoeffStruct stanPolyCoeff;


	if (CURVE_ACC == curveType)
	{
	    startPosition = (u32)(scurveInfo.accStartPoint.position * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);    //四舍五入
        endPosition   = (u32)(scurveInfo.accEndPoint.position   * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);

        startTime = scurveInfo.accStartPoint.time;
        endTime   = scurveInfo.accEndPoint.time;
        
        realPolyCoeff = scurveInfo.polyCoeff[0];
    }
    else if (CURVE_UNF == curveType)
    {
        startPosition = (u32)(scurveInfo.accEndPoint.position   * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);    //四舍五入
        endPosition   = (u32)(scurveInfo.decStartPoint.position * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);

        startTime = scurveInfo.accEndPoint.time;
        endTime   = scurveInfo.decStartPoint.time;

        realPolyCoeff = scurveInfo.polyCoeff[1];
    }
    else if (CURVE_DEC == curveType)
    {
        startPosition = (u32)(scurveInfo.decStartPoint.position * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);    //四舍五入
        endPosition   = (u32)(scurveInfo.decEndPoint.position   * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);

        startTime = scurveInfo.decStartPoint.time;
        endTime   = scurveInfo.decEndPoint.time;

        realPolyCoeff = scurveInfo.polyCoeff[2];
    }
    poseOffset = abs(endPosition - startPosition);

    if (poseOffset > 1)    //小于等于一步时直接进入最后一步计算
    {
        for (i = 0;i < (poseOffset - 1);i++)    //最后一步不用算
        {
            startPosition++;

            stanPolyCoeff.postCoeff[VELCMAXDEGREE] = realPolyCoeff.postCoeff[VELCMAXDEGREE] - startPosition;		// 系数转换为标准形式
			solutionResult = pvtSingalCurveCalc(stanPolyCoeff, &calcTime, startTime, endTime);

			if (solutionResult)		// 有解
			{
				//归一化处理
				//时间的补偿问题:理论上讲用时间作为速度是没有误差的，而误差来源于归一化后的四舍五入
                calcOutput = calcTime * FPGA_PWM_CLOCK + calcTime * pPvtCalcData->fpgaPwmClock;

                //做个上下限保护
                if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
                {
                    calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
                    
                    //系统错误码置位
                    errorCode = PVT_CALC_GREAT_UPER_LIMIT;
                }
                else if (calcOutput < OUTPUT_PERIOD_MIN)
                {
                    calcOutput = OUTPUT_PERIOD_MIN;
                    
                    //系统错误码置位
                    errorCode = PVT_CALC_LESS_LOWER_LIMIT;
                }

                calcOutput += pPvtCalcData->errorTime;
                pPvtCalcData->errorTime = calcOutput - (u32)calcOutput;

                pPvtCalcData->outpBufferFill(chanNum, 
                                             (OutpDataTypeEnum)pPvtCalcData->lastStepDir, 
                                             (u32)calcOutput, 
                                             BUFFOPERT_WRITE, 
                                             pPvtCalcData->pContext);
			}
			else    // 求解失败，终止求解
			{
				return solutionResult;    // 返回
			}
        }
    }

	//最后一步
	//calcTime = ;
	//时间的补偿问题:理论上讲用时间作为速度是没有误差的，而误差来源于归一化后的四舍五入
    calcOutput = calcTime * FPGA_PWM_CLOCK + calcTime * pPvtCalcData->fpgaPwmClock;

    //做个上下限保护
    if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
    {
        calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
        
        //系统错误码置位
        errorCode = PVT_CALC_GREAT_UPER_LIMIT;
    }
    else if (calcOutput < OUTPUT_PERIOD_MIN)
    {
        calcOutput = OUTPUT_PERIOD_MIN;
        
        //系统错误码置位
        errorCode = PVT_CALC_LESS_LOWER_LIMIT;
    }

    calcOutput += pPvtCalcData->errorTime;
    pPvtCalcData->errorTime = calcOutput - (u32)calcOutput;

    pPvtCalcData->outpBufferFill(chanNum, 
                                 (OutpDataTypeEnum)pPvtCalcData->lastStepDir, 
                                 (u32)calcOutput, 
                                 BUFFOPERT_WRITE, 
                                 pPvtCalcData->pContext);

    return errorCode;
}


/*********************************************************************************************
函 数 名: pvtScurveSolution
实现功能: S曲线计算，先根据PVT生成三段曲线:加速段，匀速段和减速段
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*********************************************************************************************/
u8 pvtScurveCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData)
{
    u8  buildResult;
    ScurveInfoStruct scurveInfo;
    
    
    /*******构建S曲线(根据△P，△T，始末速度，加速减速占比和项数计算三段曲线的各次项系数)*******/
    scurveInfo.accStartPoint = pPvtCalcData->startPoint;
    //scurveInfo.accEndPoint   = ;
    scurveInfo.accScale      = 50;    //目前暂定50%    NICK MARK
    
    //scurveInfo.decStartPoint = ;
    scurveInfo.decEndPoint   = pPvtCalcData->endPoint;
    scurveInfo.decScale      = 50;    //目前暂定50%    NICK MARK

    memset(&scurveInfo.polyCoeff, 0, sizeof(scurveInfo.polyCoeff));
    scurveInfo.polyCoeff[0].type = 2;    //目前暂定2      NICK MARK
    scurveInfo.polyCoeff[1].type = 0;    //目前暂定0      NICK MARK
    scurveInfo.polyCoeff[2].type = 2;    //目前暂定2      NICK MARK
    
    buildResult = pvtScurveBuild(&scurveInfo);
    if (buildResult != SOLVING)
    {
        return buildResult;
    }
    /*******构建S曲线(根据△P，△T，始末速度，加速减速占比和项数计算三段曲线的各次项系数)*******/
    

    /****************************************计算加速段*****************************************/
    buildResult = pvtCurveSolution(chanNum, scurveInfo, pPvtCalcData, CURVE_ACC);
    /****************************************计算加速段*****************************************/
    

    /****************************************计算匀速段*****************************************/
    buildResult = pvtCurveSolution(chanNum, scurveInfo, pPvtCalcData, CURVE_UNF);
    /****************************************计算匀速段*****************************************/
    

    /****************************************计算减速段*****************************************/
    buildResult = pvtCurveSolution(chanNum, scurveInfo, pPvtCalcData, CURVE_DEC);
    /****************************************计算减速段*****************************************/

    return buildResult;
}



/*******************************************文件尾********************************************/
