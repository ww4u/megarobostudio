#include "params.h"
#include "math.h"
#include "string.h"

struct ArmLength
{
	double Lw;
	double Lh;
	double Lp;
	double Lb;
};
struct Sysparam
{
	int m;
	int c;
	int dir;
	int mode;
	int moveMode;
};
struct CorrectData
{
	double X;
	double Y;
	double deltAngleX;
	double deltAngleY;
};
//��ֱ��
double d;
double angleLim[4];
double deltP[2];
int direction;
int mode;
int moveMode;
ArmLength armLen;
Sysparam param;
CorrectData correctData;
double workSpace[4];
int PositiveCalc(double* pIn, double* t, int length, double* result);
int OppositeCalc(double* pIn, double* v, double* t, int length, double* result);
void SetStructInfo(double* armLength, int* param);
int JudgeAngle(double* p, int len);
void AngleTrans(double *p, int len, int mode);
/// ��⺯���ĵ�������
/// ���������1��armLength -- ���᳤�ȣ�[0]:�ⳤ��[1]:���[2]:���鳤��[3]:�����
/// 2��inputParam--���������[0]:ģ������:(2M),[1]:����,[2]dir �������ת�������� 0��˳ʱ�룬1����ʱ��,[3]:��ⷽʽ,[4]:�˶���ʽ(0����ת�˶���1��ֱ���˶�) 
/// 3��P0--ĩ�˳�ʼλ�ã�����ɸѡ��
/// 4:posIn -- ���Ϊ�����ĩ�˵㣬����Ϊ�ؽڽǶȣ� 5��vin -- ����Ϊ0 ���Ϊĩ���ٶȣ�6:len -- ����ĸ�����7��mode --���ģʽ 0Ϊ���� ��1Ϊ���
/// 8:movementMode 0:��ת�˶���1��ֱ���˶�
/// ���������res-- ���ķ���ֵ
extern "C" __declspec(dllexport) int SolveStructureKinematics(double* armLength, int* inputParam, double* P0, double* posIn, double*vIn, double* tIn, int len, double* res)
{
	memset(deltP, 0, 2 * sizeof(double));
	deltP[0] = P0[0];
	deltP[1] = P0[1];
	SetStructInfo(armLength, inputParam);
	// ����Ƕ�ת��
	if (mode == 0)
	{
		if (moveMode == 0)
		{
			for (int i = 0; i < len; i++)
			{
				posIn[3 * i] = posIn[3 * i] * PI / 180;
				posIn[3 * i + 1] = posIn[3 * i + 1] * PI / 180;
			}
		}
	}
	// ������������
	if (mode == 0)
	{
		// ����
		memset(res, 0, 3 * len * sizeof(double));
		return PositiveCalc(posIn, tIn, len, res);
	}
	else
	{
		//���
		memset(res, 0, 5 * len * sizeof(double));
		return OppositeCalc(posIn, vIn, tIn, len, res);
	}
}
int PositiveCalc(double* pIn, double* t, int length, double* result)
{
	// �Ƕ�ת��
	AngleTrans(pIn, length, 0);
	double tmp1 = 0;
	double tmp2 = 0;
	/* �ж�ĩ��ִ�����Ƿ��ڹ��������� */
	for (int i = 0; i < length; i++)
	{
		// ��ʱ��
		if (direction == 1)
		{
			if (moveMode == 0)
			{
				tmp1 = d * pIn[3 * i];
				tmp2 = d * pIn[3 * i + 1];
			}
			else
			{
				tmp1 = 2*pIn[3 * i];
				tmp2 = 2*pIn[3 * i + 1];
			}
			// ����x
			result[3 * i] = 0.25 * (tmp1 - tmp2) - deltP[0];
			// ���������ж�												
			if (fabs(result[2 * i] - workSpace[0]) < 0
				|| fabs(result[2 * i] - workSpace[1]) < 0)
			{
				return ERROR_OUT_OF_WORK_SPACE;
			}
			// ����y
			result[3 * i + 1] = 0.25 * (tmp1 + tmp2) - deltP[1];
			// T
			result[3 * i + 2] = t[i];
			if (fabs(result[2 * i + 1] - workSpace[2]) < 0 || fabs(result[2 * i + 1] - workSpace[3]) < 0)
			{
				return ERROR_OUT_OF_WORK_SPACE;
			}
		}
		// ˳ʱ��
		else
		{
			if (moveMode == 0)
			{
				tmp1 = -d * pIn[3 * i];
				tmp2 = -d * pIn[3 * i + 1];
			}
			else
			{
				tmp1 = -2*pIn[3 * i];
				tmp2 = -2 * pIn[3 * i + 1];
			}
			// ����x
			result[3 * i] = 0.25 * (tmp1 - tmp2) - deltP[0];
			// ���������ж�												
			if (fabs(result[2 * i] - workSpace[0]) < 0
				|| fabs(result[2 * i] - workSpace[1]) < 0)
			{
				return ERROR_OUT_OF_WORK_SPACE;
			}
			// ����y
			result[3 * i + 1] = 0.25 * (tmp1 + tmp2) - deltP[1];
			// T
			result[3 * i + 2] = t[i];
			if (fabs(result[2 * i + 1] - workSpace[2]) < 0 || fabs(result[2 * i + 1] - workSpace[3]) < 0)
			{
				return ERROR_OUT_OF_WORK_SPACE;
			}
		}
	}
	return 0;
}
int OppositeCalc(double* pIn, double* v, double* t, int length, double* result)
{
	double temp[2];
	memset(temp, 0, 2 * sizeof(double));
	// �ж�ĩ��ִ�����Ƿ��ڹ���������
	for (int i = 0; i < length; i++)
	{
		// ��ʱ��
		if (direction == 1)
		{
			pIn[3 * i] = pIn[3 * i] + deltP[0];
			pIn[3 * i + 1] = pIn[3 * i + 1] + deltP[1];
			/* x������������ */
			if (pIn[3 * i] - workSpace[0] < 0
				|| pIn[3 * i] - workSpace[1]>0)
			{
				return ERROR_OUT_OF_WORK_SPACE;
			}
			/* y������������ */
			if (pIn[3 * i + 1] - workSpace[2] < 0 || pIn[3 * i + 1] - workSpace[3]>0)
			{
				return ERROR_OUT_OF_WORK_SPACE;
			}
			if (moveMode == 0)
			{
				// ������
				temp[0] = 2.0 * (pIn[3 * i] + pIn[3 * i + 1]) / d;
				temp[1] = 2.0 * (-pIn[3 * i] + pIn[3 * i + 1]) / d;
				result[5 * i] = temp[0] * 180 / PI;
				result[5 * i + 1] = temp[1] * 180 / PI;
				// �ٶ����
				result[5 * i + 2] = 2.0 * (v[3 * i] + v[3 * i + 1]) / d * 180 / PI;
				result[5 * i + 3] = 2.0 * (-v[3 * i] + v[3 * i + 1]) / d * 180 / PI;
				result[5 * i + 4] = t[i];
			}
			else
			{
				// ������
				temp[0] = pIn[3 * i] + pIn[3 * i + 1];
				temp[1] = -pIn[3 * i] + pIn[3 * i + 1];
				result[5 * i] = temp[0];
				result[5 * i + 1] = temp[1];
				// �ٶ����
				result[5 * i + 2] = v[3 * i] + v[3 * i + 1];
				result[5 * i + 3] = -v[3 * i] + v[3 * i + 1];
				result[5 * i + 4] = t[i];
			}
		}
		else
		{
			pIn[3 * i] = pIn[3 * i] + deltP[0];
			pIn[3 * i + 1] = pIn[3 * i + 1] + deltP[1];
			/* x������������ */
			if (pIn[3 * i] - workSpace[0] < 0
				|| pIn[3 * i] - workSpace[1]>0)
			{
				return ERROR_OUT_OF_WORK_SPACE;
			}
			/* y������������ */
			if (pIn[3 * i + 1] - workSpace[2] < 0 || pIn[3 * i + 1] - workSpace[3]>0)
			{
				return ERROR_OUT_OF_WORK_SPACE;
			}
			if (moveMode == 0)
			{
				// ������
				temp[0] = 2.0 * (pIn[3 * i] + pIn[3 * i + 1]) / d;
				temp[1] = 2.0 * (-pIn[3 * i] + pIn[3 * i + 1]) / d;
				result[5 * i] = temp[0] * 180 / PI;
				result[5 * i + 1] = temp[1] * 180 / PI;
				// �ٶ����
				result[5 * i + 2] = -2.0 * (v[3 * i] + v[3 * i + 1]) / d * 180 / PI;
				result[5 * i + 3] = -2.0 * (-v[3 * i] + v[3 * i + 1]) / d * 180 / PI;
				result[5 * i + 4] = t[i];
			}
			else
			{
				// ������
				temp[0] = 2.0 * (pIn[3 * i] + pIn[3 * i + 1]);
				temp[1] = 2.0 * (-pIn[3 * i] + pIn[3 * i + 1]);
				result[5 * i] = temp[0] * 180 / PI;
				result[5 * i + 1] = temp[1] * 180 / PI;
				// �ٶ����
				result[5 * i + 2] = -2.0 * (v[3 * i] + v[3 * i + 1]);
				result[5 * i + 3] = -2.0 * (-v[3 * i] + v[3 * i + 1]);
				result[5 * i + 4] = t[i];
			}
		}

	}
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (fabs(result[5 * i + j]) < ERROR)
			{
				result[5 * i + j] = 0;
			}
		}
	}
	return SUCCESS;
}
void SetStructInfo(double* armLength, int* param)
{
	// ������������
	int m = param[0];
	int c = param[1];
	direction = param[2];
	mode = param[3];
	moveMode = param[4];
	armLen.Lw = armLength[0];
	armLen.Lh = armLength[1];
	armLen.Lp = armLength[2];
	armLen.Lb = armLength[3];
	correctData.X = deltP[0];
	correctData.Y = deltP[1];
	d = m*c / PI;
	if (moveMode == 0)
	{
		correctData.deltAngleX = -2 * (deltP[0] + deltP[1]) / d;
		correctData.deltAngleY = -2 * (-deltP[0] + deltP[1]) / d;
	}
	else
	{
		correctData.deltAngleX = deltP[0] + deltP[1];
		correctData.deltAngleY = -deltP[0] + deltP[1];
	}
	// ��������
	double tmp1 = 0;
	double tmp2 = 0;
	tmp1 = armLen.Lb / 2.0;
	// Xmin
	workSpace[0] = tmp1;
	// Xmax
	workSpace[1] = tmp1 + armLen.Lh - armLen.Lb;

	tmp2 = (armLen.Lw - armLen.Lp) / 2;
	// Ymin
	workSpace[2] = -tmp2;
	// Ymax
	workSpace[3] = tmp2;
}
int JudgeAngle(double* p, int len)
{
	double temp[2];
	for (int i = 0; i < len; i++)
	{
		temp[0] = p[3 * i];
		temp[1] = p[3 * i + 1];
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

void AngleTrans(double *p, int len, int mode)
{
	int i;
	if (mode == 0)		// ����
	{
		// ˳ʱ��
		if (direction == 0)
		{
			for (i = 0; i < len; i++)		// ����/���Ƕ�ת��
			{
				p[3 * i] = -p[3 * i] - correctData.deltAngleX;		// ����/���Ƕ�1ת��
				p[3 * i + 1] = -p[3 * i + 1] - correctData.deltAngleY;		// ����/���Ƕ�2ת��
			}
		}
		//��ʱ��
		else
		{
			for (i = 0; i < len; i++)		// ����/���Ƕ�ת��
			{
				p[3 * i] = p[3 * i] - correctData.deltAngleX;		// ����/���Ƕ�1ת��
				p[3 * i + 1] = p[3 * i + 1] - correctData.deltAngleY;		// ����/���Ƕ�2ת��
			}
		}
	}
	else		//���
	{
		// ˳ʱ��
		if (direction == 0)
		{
			p[0] = -p[0] - correctData.deltAngleX;
			p[1] = -p[1] - correctData.deltAngleY;
		}
		else
		{
			p[0] = p[0] + correctData.deltAngleX;
			p[1] = p[1] + correctData.deltAngleY;
		}

	}
}