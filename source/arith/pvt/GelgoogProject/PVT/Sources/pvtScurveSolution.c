/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvtScurveSolution.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.11.23;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "arm_math.h"
#include "pvtScurveSolution.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/
#define    MAXDEGREE        3
#define    VELCMAXDEGREE    MAXDEGREE + 1
#define    POSTMAXDEGREE    MAXDEGREE + 2

#define    SOLVING      0
#define    BUILDERROR   1



/*************************************�ֲ����������Ͷ���**************************************/
typedef enum
{
    CURVE_ACC = 0,
    CURVE_UNF,
    CURVE_DEC
    
}CurvetYPEEnum;

typedef struct
{
    f32  postCoeff[POSTMAXDEGREE];    // λ�ö���ʽϵ��
	f32  velcCoeff[VELCMAXDEGREE];    // �ٶȶ���ʽϵ��
	
	int type;
    
}PolynomialCoeffStruct;

typedef struct
{
    PvtPointStruct accStartPoint;    //���ٶ���ʼλ��  
    PvtPointStruct accEndPoint;      //���ٶν���λ��
  
    u8  accScale;        //���ٶ�ռ�ȣ�������50%
    
    PvtPointStruct decStartPoint;    //���ٶ���ʼλ��
    PvtPointStruct decEndPoint;      //���ٶν���λ��
    
    u8  decScale;        //���ٶ�ռ�ȣ�������50%

    PolynomialCoeffStruct polyCoeff[3];    //�������ߵĶ���ʽϵ��
      
}ScurveInfoStruct;

typedef struct
{
    f32 velocity[2];
    f32 time[2];
    f32 refPoint[2];

    f32 position[2];    //���ٶ�ʹ��
    
}PolynomialParaStruct;

typedef struct
{
    f32 solution[MAXDEGREE + 1];

    u8  solutionNum;
    
}SolutionInfoStruct;



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvtPolynomialCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: �������ʽ���䵼��ֵ;
*********************************************************************************************/
int reviseCoefficient(f32 coeff[], int arrayLength)
{
	f32 err = 1e-20;		// �������
	int i;

	if (arrayLength == 0)		// ���鳤��Ϊ�㣬ֱ�ӷ���
		return 0;

	for (i=0;i<arrayLength;i++)
		if (fabs(coeff[i])> 0)		// ��¼��Ч�ߴ���ϵ��
			break;

	if (fabs(coeff[i]) < err)		// ϵ����Ч������0
		return 0;
	else
		return arrayLength - i;		// �����Ч����
}


/*********************************************************************************************
�� �� ��: pvtPolynomialCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: �������ʽ���䵼��ֵ;
*********************************************************************************************/
f32 pvtPolynomialCalc(f32 coeff[], f32 x, int arrayLength, int mode)
{
	f32 b[6], c;
	int i;
	int length, index;

	length = reviseCoefficient(coeff, arrayLength);		// ��ȡ��Ч����
	index = arrayLength - length;		// ��Чϵ����ʼ�±�

	if (length == 0)		// ����Чϵ��
		return 0;

	b[index] = coeff[index];
	for (i = index+1;i < arrayLength;i++)
		b[i] = coeff[i] + x*b[i-1];
	if (mode == 0)		
		return b[i-1];		// ���ض���ʽ��ֵ
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
		return c;		// ���ص���ֵ
	}
}


/*********************************************************************************************
�� �� ��: pvtLinearPolynomialBuild;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ����ʼĩ��λ�ù������Ժ���;
*********************************************************************************************/
u8 pvtLinearPolynomialBuild(PolynomialCoeffStruct *coefficient, PolynomialParaStruct polyPara)
{
	f32 slope;


	slope = (polyPara.position[1] - polyPara.position[0]) / 
	        (polyPara.time[1]     - polyPara.time[0]);

	coefficient->velcCoeff[3] = slope;		// �����ٶȶ���ʽ����

	// ����λ�ö���ʽ����
	coefficient->postCoeff[3] = coefficient->velcCoeff[3];
	coefficient->postCoeff[4] = polyPara.position[0] - slope * polyPara.time[0];

	coefficient->type = 0;		// ���ñ�־λ

	return SOLVING;		// ����
}


/*********************************************************************************************
�� �� ��: pvtStepPolynomialBuild;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ����ʼĩ���ٶȹ���STEP����ʽ;
*********************************************************************************************/
u8 pvtStepPolynomialBuild(PolynomialCoeffStruct *coefficient, PolynomialParaStruct polyPara)
{
    f32 slope;


    switch(coefficient->type)		// ���������Ķ���ʽ�����Ƿ񳬳����巶Χ
    {
        case 0:			// ��ζ���ʽ��������
            if (MAXDEGREE < 0)		// ����Ķ���ʽ����Ϊ�����˳�
            {
                return BUILDERROR;
            }
          break;
          
        case 1:			// һ�ζ���ʽ
            if (MAXDEGREE < 1)		// �����Ķ���ʽ����������Χ���˳�
            {
                return BUILDERROR;
            }
          break;
        
        case 2:			// ���ζ���ʽ
            if (MAXDEGREE < 3)		// �����Ķ���ʽ����������Χ���˳�
            {
                return BUILDERROR;
            }
          break;
        
        case 3:			// ��ζ���ʽ
            if (MAXDEGREE < 5)		// �����Ķ���ʽ����������Χ���˳�
            {
                return BUILDERROR;
            }
          break;
        
        default :				// �����Ķ���ʽ�����������˳�
            return BUILDERROR;
          break;
    }

    switch (coefficient->type)
    {
        case 1:			// STEP1����ʽ���
            slope = 1.0f / (polyPara.time[1] - polyPara.time[0]);
            coefficient->velcCoeff[MAXDEGREE - 1] = slope * (polyPara.velocity[1] - polyPara.velocity[0]);		// �����ٶȶ���ʽ����
            coefficient->velcCoeff[MAXDEGREE]     = slope * (polyPara.velocity[0] * polyPara.time[1] - 
                                                             polyPara.velocity[1] * polyPara.time[0]);

            coefficient->postCoeff[MAXDEGREE - 1] = coefficient->velcCoeff[2] * 0.5f;		// ����λ�ö���ʽ����
            coefficient->postCoeff[MAXDEGREE]     = coefficient->velcCoeff[3];
            coefficient->postCoeff[MAXDEGREE + 1] = polyPara.refPoint[1] - 
                                                    pvtPolynomialCalc(coefficient->postCoeff, 
                                                                      polyPara.refPoint[0], 
                                                                      POSTMAXDEGREE, 
                                                                      0);		// �����ֳ�����
          break;

        case 2:			// STEP3����ʽ���
            slope = (polyPara.velocity[1] - polyPara.velocity[0]) / pow((polyPara.time[1] - polyPara.time[0]), 3.0);
            coefficient->velcCoeff[MAXDEGREE - 3] = -2.0f * slope;		// �����ٶȶ���ʽ����
            coefficient->velcCoeff[MAXDEGREE - 2] =  3.0f * slope * (polyPara.time[0] + polyPara.time[1]);
            coefficient->velcCoeff[MAXDEGREE - 1] = -6.0f * slope * polyPara.time[0] * polyPara.time[1];
            coefficient->velcCoeff[MAXDEGREE]     = slope * polyPara.time[0] * polyPara.time[0] * 
                                                    (3.0f * polyPara.time[1] - polyPara.time[0]) + polyPara.velocity[1];

            coefficient->postCoeff[MAXDEGREE - 3] = coefficient->velcCoeff[0] * 0.25f;		// ����λ�ö���ʽ����
            coefficient->postCoeff[MAXDEGREE - 2] = coefficient->velcCoeff[1] / 3.0;
            coefficient->postCoeff[MAXDEGREE - 1] = coefficient->velcCoeff[2] * 0.5f;
            coefficient->postCoeff[MAXDEGREE]     = coefficient->velcCoeff[3];
            coefficient->postCoeff[MAXDEGREE + 1] = polyPara.refPoint[1] - 
                                                    pvtPolynomialCalc(coefficient->postCoeff, 
                                                                      polyPara.refPoint[0], 
                                                                      POSTMAXDEGREE, 
                                                                      0);		// �����ֳ�����
          break;
    }

    return SOLVING;		// ����
}


/*********************************************************************************************
�� �� ��: pvtScurveBuild;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ����S����: ���켣����ʽϵ�����ڵ����;
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
    

    //�˶�ʱ��
    motionTime = pScurveInfo->decEndPoint.time - pScurveInfo->accStartPoint.time;
    accTime = motionTime * pScurveInfo->accScale / 100.0f;
    decTime = motionTime * pScurveInfo->decScale / 100.0f;

    //�˶�����
    postOffset = pScurveInfo->decEndPoint.position - pScurveInfo->accStartPoint.position;

    //�����˶��ٶȣ�Ҳ������ٶ�
    //�Ӽ��ٲ��ԳƵ������: Vmax = (P - V0 * Tacc * 0.5 - V1 * Tdec * 0.5)/(T - 0.5 * (Tacc + Tdec))
    //�Ӽ���  �ԳƵ������: Vmax = (P - (V0 + V1) * Tacc * 0.5)/(T - Tacc)
    /*maxSpeed = (2.0f * postOffset - pScurveInfo->accStartPoint.speed * accTime - pScurveInfo->decEndPoint.speed * decTime) / 
               (2.0f * motionTime - (accTime + decTime));*/
    maxSpeed = (postOffset - (pScurveInfo->accStartPoint.speed + pScurveInfo->decEndPoint.speed) * accTime * 0.5f) / 
               (motionTime - accTime);

    /**********************************������һ������**********************************/
    //����ٶȣ��յ��ٶ�
    polyPara.velocity[0] = pScurveInfo->accStartPoint.speed;
    polyPara.velocity[1] = maxSpeed;
    
    //���ʱ�䣬�յ�ʱ��
    polyPara.time[0] = 0;
    polyPara.time[1] = accTime;

    //�ο���ʱ�䣬�ο���λ��
    polyPara.refPoint[0] = 0;
    polyPara.refPoint[1] = pScurveInfo->accStartPoint.position;
    
    buildResult = pvtStepPolynomialBuild(&(pScurveInfo->polyCoeff[0]), polyPara);
    if (buildResult != SOLVING)    //����ʧ��
    {
        return buildResult;
    }

    //�����һ������ĩ����Ϣ(λ�ã��ٶȣ�ʱ��)
    pScurveInfo->accEndPoint.position = pvtPolynomialCalc(pScurveInfo->polyCoeff[0].postCoeff, accTime, POSTMAXDEGREE, 0);
    pScurveInfo->accEndPoint.speed    = pvtPolynomialCalc(pScurveInfo->polyCoeff[0].velcCoeff, accTime, VELCMAXDEGREE, 0);
    pScurveInfo->accEndPoint.time     = accTime;
    /**********************************������һ������**********************************/


    /**********************************��������������**********************************/
    //����ٶȣ��յ��ٶ�
    polyPara.velocity[0] = maxSpeed;
    polyPara.velocity[1] = pScurveInfo->decEndPoint.speed;
    
    //���ʱ�䣬�յ�ʱ��
    polyPara.time[0] = 0;
    polyPara.time[1] = accTime;

    //�ο���ʱ�䣬�ο���λ��
    polyPara.refPoint[0] = accTime;
    polyPara.refPoint[1] = pScurveInfo->decEndPoint.position;
    
    buildResult = pvtStepPolynomialBuild(&(pScurveInfo->polyCoeff[2]), polyPara);
    if (buildResult != SOLVING)    //����ʧ��
    {
        return buildResult;
    }

    //�������������ʼ����Ϣ(λ�ã��ٶȣ�ʱ��)
    pScurveInfo->decStartPoint.time = pScurveInfo->decEndPoint.time - accTime;
    pScurveInfo->decStartPoint.position = pvtPolynomialCalc(pScurveInfo->polyCoeff[2].postCoeff, 0, POSTMAXDEGREE, 0);
    pScurveInfo->decStartPoint.speed    = pvtPolynomialCalc(pScurveInfo->polyCoeff[2].velcCoeff, 0, VELCMAXDEGREE, 0);
    /**********************************��������������**********************************/


    /**********************************�����ڶ�������**********************************/
    //����ٶȣ��յ��ٶ�
    polyPara.position[0] = pScurveInfo->accEndPoint.position;
    polyPara.position[1] = pScurveInfo->decStartPoint.position;
    
    //���ʱ�䣬�յ�ʱ��
    polyPara.time[0] = 0;
    polyPara.time[1] = motionTime - 2.0f * accTime;
    
    buildResult = pvtLinearPolynomialBuild(&(pScurveInfo->polyCoeff[1]), polyPara);

    return buildResult;
}


/*********************************************************************************************
�� �� ��: solveQuartic
ʵ�ֹ���: �����Ĵζ���ʽ��ֵ
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
void solveLinear(f32 coeff[], SolutionInfoStruct *pSolution)
{
	double a, b, err;

	err = 1e-10;		// �������
	a = coeff[0];		// һ����ϵ��
	b = coeff[1];		// ������ϵ��

	if (fabs(a) < err)		// �޽�
	{
		pSolution->solutionNum = 0;
		pSolution->solution[0] = 0;
	}
	else		// Ψһ��
	{
		pSolution->solutionNum = 1;
		pSolution->solution[0] = -b/a;
	}
}


/*********************************************************************************************
�� �� ��: solveQuartic
ʵ�ֹ���: �����Ĵζ���ʽ��ֵ
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
void solveQuadratic(f32 coeff[], SolutionInfoStruct *pSolution)
{
	f32 a, b, c, err;
	f32 coeffLinear[2] = {coeff[1], coeff[2]};
	f32 det;

	err = 1e-20;		// �������
	a = coeff[0];		// ���δ���ϵ��
	b = coeff[1];		// һ����ϵ��
	c = coeff[2];		// ������ϵ��

	if (fabs(a)<err||fabs(a/b)<1e-10)		// �޽�
	{
		solveLinear(coeffLinear, pSolution);		// ���Զ���ʽ���
	}
	else		// Ψһ��
	{
		det = b*b - 4.0*a*c;		// �б�ʽ
		if (det < -err)		// �޽�
		{
			pSolution->solutionNum = 0;
			pSolution->solution[0] = 0;
		}
		else
		{
			det = fabs(det);
			if (det < err)		// һ���ظ�
			{
				pSolution->solutionNum = 1;
				pSolution->solution[0] = -b/(2.0*a);
			}
			else		// �����ظ�
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
�� �� ��: solveQuartic
ʵ�ֹ���: �����Ĵζ���ʽ��ֵ
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
void solveCubic(f32 coeff[], SolutionInfoStruct *pSolution)
{
	f32 a, b, c, d, err;
	f32 coeffQuadratic[3] = {coeff[1], coeff[2], coeff[3]};
	f32 A, B, C, det, K, K1, K2, sgn1, sgn2, theta;

	err = 1e-30;		// �������
	a = coeff[0];		// ���δ���ϵ��
	b = coeff[1];		// ���δ���ϵ��
	c = coeff[2];		// һ����ϵ��
	d = coeff[3];		// ������ϵ��

	if (fabs(a) < err)
		solveQuadratic(coeffQuadratic,pSolution);		// ���ζ���ʽ���
	else
	{
		A = b*b - 3.0*a*c;
		B = b*c - 9.0*a*d;
		C = c*c - 3.0*b*d;
		det = B*B - 4.0*A*C;

		if (fabs(det) < err)		// det=0
		{
			if (fabs(A) < err)		// A=B=0,һ�����ظ�
			{
				pSolution->solutionNum = 1;
				pSolution->solution[0] = -b/(3.0*a);
			}
			else		// A!=0: ������
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

			pSolution->solutionNum = 1;		// һ����
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

					pSolution->solutionNum = 3;		// ������
					pSolution->solution[0] = (-b - 2.0*cos(theta)*A)/(3.0*a);
					pSolution->solution[1] = (-b + (cos(theta) + sin(theta) * sqrt(3.0))*A)/(3.0*a);
					pSolution->solution[2] = (-b + (cos(theta) - sin(theta) * sqrt(3.0))*A)/(3.0*a);
				}
				else
				{
					pSolution->solutionNum = 0;		// ��ʵ��
					pSolution->solution[0] = 0;
				}
			}
			else
			{
				//pSolution->solutionNum = -1;		// ��ʵ��
				pSolution->solutionNum = 0;
				pSolution->solution[0] = 0;
			}
		}
	}
}


/*********************************************************************************************
�� �� ��: solveQuartic
ʵ�ֹ���: �����Ĵζ���ʽ��ֵ
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
void solveQuartic(f32 coeff[], SolutionInfoStruct *pSolution)
{
	f32 a, b, c, d, e, err;
	f32 coeffCubic[4] = { coeff[1], coeff[2], coeff[3], coeff[4] };
	f32 A, B, C, D, E, G, F, det, K, K1, K2, K3, sgn, sgn1, sgn2, T, theta, Y1, Y2, W;
	f32 temp, temp1, temp2;

	err = 1e-30;		// �������
	a = coeff[0];		// �Ĵδ���ϵ��
	b = coeff[1];		// ���δ���ϵ��
	c = coeff[2];		// ���δ���ϵ��
	d = coeff[3];		// һ����ϵ��
	e = coeff[4];		// ������ϵ��

	if (fabs(a) < err)		// ���ζ���ʽ���
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

		det = B*B-4.0*A*C;		// ���б�ʽ
		sgn = 1.0;
		if(G<0)
			sgn = -1.0;

		if (fabs(D) < err&&fabs(E) < err&&fabs(F) < err)		// D=E=F: һ�����ظ�
		{
			pSolution->solutionNum = 1;		
			pSolution->solution[0] = -b/(4.0*a);
		}
		else
		{
			if (fabs(E) < err&&fabs(F) < err)		// D!=0,E=F=0: �������ظ�
			{
				if (D < 0)
				{
					temp = sqrt(-D);

					pSolution->solutionNum = 2;
					pSolution->solution[0] = (-b + temp)/(4.0*a);
					pSolution->solution[1] = (-b - temp)/(4.0*a);
				}
				else		// �޽�
				{
					pSolution->solutionNum = 0;
					pSolution->solution[0] = 0;
				}
			}
			else
			{
				if (fabs(det) < err)		// det = 0
				{
					if (fabs(A) < err)		// A = 0: ������
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
							if (K2 >= 0)		// ������
							{
								temp2 = sqrt(K2);

								pSolution->solutionNum = 3;
								pSolution->solution[0] = (-b + sgn*temp1 + 2.0*temp2)/(4.0*a);
								pSolution->solution[1] = (-b + sgn*temp1 - 2.0*temp2)/(4.0*a);
								pSolution->solution[2] = (-b - sgn*temp1)/(4.0*a);
							}
							else		// һ����
							{
								pSolution->solutionNum = 1;
								pSolution->solution[0] = (-b + sgn*temp1)/(4.0*a);
							}
						}
						else		// �޽�
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
							if (T > 1 || T < -1)		// �޽�
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

							if (K1 >= 0 && K2 >= 0 && K3 >= 0)		// �ĸ���
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
							else		// �޽�
							{
								//(*solution).flag = -3;
								pSolution->solutionNum = 0;
								pSolution->solution[0] = 0;
							}
						}
						else		// �޽�
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

						if (K >= 0)			// ������
						{
							K = sgn*sqrt(K);

							pSolution->solutionNum = 2;
							pSolution->solution[0] = (-b + K + W)/(4.0*a);
							pSolution->solution[1] = (-b + K - W)/(4.0*a);
						}
						else		// �޽�
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
�� �� ��: isInRegion
ʵ�ֹ���: �������Ƿ���������
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
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
�� �� ��: pvtScurveSolution
ʵ�ֹ���: S���߼��㣬�ȸ���PVT������������:���ٶΣ����ٶκͼ��ٶ�
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
bool pvtSingalCurveCalc(PolynomialCoeffStruct polyCoeff, f32 *pCalcTime, f32 startTime, f32 endTime)
{
	u8   i;
	bool inRegion;
	SolutionInfoStruct solution;

			
	solveQuartic(polyCoeff.postCoeff, &solution);    // ����Ĵμ����´ζ���ʽ

	// �жϽ����Ч��
	if (solution.solutionNum > 0)		// ����ʽ��ȫ�ַ�Χ���н�
	{
		for (i = 0;i < solution.solutionNum;i++)		//�жϽ��Ƿ���ָ��������
		{
			inRegion = isInRegion(solution.solution[i], endTime, startTime);    // �жϽ��Ƿ���������
			
			if (inRegion)    // ����������
			{
				*pCalcTime = solution.solution[i];    // ����Ψһ��
				
				return true;    // �˳�ѭ��
			}
		}
	}
	
	return false;
}


/*********************************************************************************************
�� �� ��: pvtScurveSolution
ʵ�ֹ���: S���߼��㣬�ȸ���PVT������������:���ٶΣ����ٶκͼ��ٶ�
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
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
	    startPosition = (u32)(scurveInfo.accStartPoint.position * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);    //��������
        endPosition   = (u32)(scurveInfo.accEndPoint.position   * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);

        startTime = scurveInfo.accStartPoint.time;
        endTime   = scurveInfo.accEndPoint.time;
        
        realPolyCoeff = scurveInfo.polyCoeff[0];
    }
    else if (CURVE_UNF == curveType)
    {
        startPosition = (u32)(scurveInfo.accEndPoint.position   * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);    //��������
        endPosition   = (u32)(scurveInfo.decStartPoint.position * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);

        startTime = scurveInfo.accEndPoint.time;
        endTime   = scurveInfo.decStartPoint.time;

        realPolyCoeff = scurveInfo.polyCoeff[1];
    }
    else if (CURVE_DEC == curveType)
    {
        startPosition = (u32)(scurveInfo.decStartPoint.position * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);    //��������
        endPosition   = (u32)(scurveInfo.decEndPoint.position   * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);

        startTime = scurveInfo.decStartPoint.time;
        endTime   = scurveInfo.decEndPoint.time;

        realPolyCoeff = scurveInfo.polyCoeff[2];
    }
    poseOffset = abs(endPosition - startPosition);

    if (poseOffset > 1)    //С�ڵ���һ��ʱֱ�ӽ������һ������
    {
        for (i = 0;i < (poseOffset - 1);i++)    //���һ��������
        {
            startPosition++;

            stanPolyCoeff.postCoeff[VELCMAXDEGREE] = realPolyCoeff.postCoeff[VELCMAXDEGREE] - startPosition;		// ϵ��ת��Ϊ��׼��ʽ
			solutionResult = pvtSingalCurveCalc(stanPolyCoeff, &calcTime, startTime, endTime);

			if (solutionResult)		// �н�
			{
				//��һ������
				//ʱ��Ĳ�������:�����Ͻ���ʱ����Ϊ�ٶ���û�����ģ��������Դ�ڹ�һ�������������
                calcOutput = calcTime * FPGA_PWM_CLOCK + calcTime * pPvtCalcData->fpgaPwmClock;

                //���������ޱ���
                if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
                {
                    calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
                    
                    //ϵͳ��������λ
                    errorCode = PVT_CALC_GREAT_UPER_LIMIT;
                }
                else if (calcOutput < OUTPUT_PERIOD_MIN)
                {
                    calcOutput = OUTPUT_PERIOD_MIN;
                    
                    //ϵͳ��������λ
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
			else    // ���ʧ�ܣ���ֹ���
			{
				return solutionResult;    // ����
			}
        }
    }

	//���һ��
	//calcTime = ;
	//ʱ��Ĳ�������:�����Ͻ���ʱ����Ϊ�ٶ���û�����ģ��������Դ�ڹ�һ�������������
    calcOutput = calcTime * FPGA_PWM_CLOCK + calcTime * pPvtCalcData->fpgaPwmClock;

    //���������ޱ���
    if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
    {
        calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
        
        //ϵͳ��������λ
        errorCode = PVT_CALC_GREAT_UPER_LIMIT;
    }
    else if (calcOutput < OUTPUT_PERIOD_MIN)
    {
        calcOutput = OUTPUT_PERIOD_MIN;
        
        //ϵͳ��������λ
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
�� �� ��: pvtScurveSolution
ʵ�ֹ���: S���߼��㣬�ȸ���PVT������������:���ٶΣ����ٶκͼ��ٶ�
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
u8 pvtScurveCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData)
{
    u8  buildResult;
    ScurveInfoStruct scurveInfo;
    
    
    /*******����S����(���ݡ�P����T��ʼĩ�ٶȣ����ټ���ռ�Ⱥ����������������ߵĸ�����ϵ��)*******/
    scurveInfo.accStartPoint = pPvtCalcData->startPoint;
    //scurveInfo.accEndPoint   = ;
    scurveInfo.accScale      = 50;    //Ŀǰ�ݶ�50%    NICK MARK
    
    //scurveInfo.decStartPoint = ;
    scurveInfo.decEndPoint   = pPvtCalcData->endPoint;
    scurveInfo.decScale      = 50;    //Ŀǰ�ݶ�50%    NICK MARK

    memset(&scurveInfo.polyCoeff, 0, sizeof(scurveInfo.polyCoeff));
    scurveInfo.polyCoeff[0].type = 2;    //Ŀǰ�ݶ�2      NICK MARK
    scurveInfo.polyCoeff[1].type = 0;    //Ŀǰ�ݶ�0      NICK MARK
    scurveInfo.polyCoeff[2].type = 2;    //Ŀǰ�ݶ�2      NICK MARK
    
    buildResult = pvtScurveBuild(&scurveInfo);
    if (buildResult != SOLVING)
    {
        return buildResult;
    }
    /*******����S����(���ݡ�P����T��ʼĩ�ٶȣ����ټ���ռ�Ⱥ����������������ߵĸ�����ϵ��)*******/
    

    /****************************************������ٶ�*****************************************/
    buildResult = pvtCurveSolution(chanNum, scurveInfo, pPvtCalcData, CURVE_ACC);
    /****************************************������ٶ�*****************************************/
    

    /****************************************�������ٶ�*****************************************/
    buildResult = pvtCurveSolution(chanNum, scurveInfo, pPvtCalcData, CURVE_UNF);
    /****************************************�������ٶ�*****************************************/
    

    /****************************************������ٶ�*****************************************/
    buildResult = pvtCurveSolution(chanNum, scurveInfo, pPvtCalcData, CURVE_DEC);
    /****************************************������ٶ�*****************************************/

    return buildResult;
}



/*******************************************�ļ�β********************************************/
