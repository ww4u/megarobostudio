//#include "params.h"
#include "math.h"
#include <vector>

// ��������
// ������1:�����PVT��,2��PVT����,3:�켣�滮��ʽ(0:���� 1:���� 2:S��),4�Ӽ���ռ��(���κ�S����Ч),5:��������PVT�����е�����
// �������������0��������1���쳣��
//! pvt[0] = p 
//! pvt[1] = v 
//! pvt[2] = t

int  GetCalcResult(double* pvt, int len, int mode, double* percent,int* index)
{
	bool flg = true;
	for (int i = 0; i < len-1; i++)
	{
		double startPost = pvt[3*i];
		double totalPost = pvt[3*(i+1)] - pvt[3 * i];
		double startSpeed = pvt[3 * i+1];
		if(fabs(startSpeed)<1e-4)
		{
			startSpeed = 0;
		}
		double endSpeed = pvt[3 *(i+1) + 1];
		if (fabs(endSpeed)<1e-4)
		{
			endSpeed = 0;
		}
		double startTime = pvt[3 * i + 2];
		double totalTime = pvt[3 *(i+1) + 2]- pvt[3 * i + 2];
		if(fabs(startSpeed)<1e-4 && fabs(endSpeed)<1e-4)
		{
			continue;
		}
		// λ��Ϊ�����ٶ�Ϊ��;λ��Ϊ�����ٶ�Ϊ��;�ٶ����Ϊ����滮������
		if (totalPost > 0 && endSpeed < 0 || totalPost < 0 && endSpeed>0 || startSpeed*endSpeed<0)
		{
			flg = false;
			index[i] = 1;
			continue;
		}
		// ���ζ���ʽ�滮
		if (mode == 0)
		{
			double cubeTime = totalTime * totalTime * totalTime;
			double squareTime = totalTime * totalTime;
			double postDiffer = totalPost* 3.0;
			double tempValue = -totalPost * 2.0 / cubeTime +
				(startSpeed + endSpeed) * 1.0 / squareTime;
			for (int j = 1; j < 50; j++)
			{
				double pointTime = totalTime / 50.0*j;
				double tempPost1 = tempValue * pointTime*pointTime*pointTime;
				double tempPost2 = (postDiffer / squareTime
					- (startSpeed * 2.0 + endSpeed) / totalTime) * pointTime*pointTime;
				//����λ��
				double curPost = tempPost1 + tempPost2 + startSpeed * pointTime + startPost;
				if (totalPost > 0 && (curPost<startPost || curPost >startPost +totalPost)
					||totalPost < 0 && (curPost<startPost + totalPost || curPost >startPost))
				{
					flg = false;
					index[i] = 1;
					break;
				}
			}
		}
		// ���ι滮
		else if (mode == 1)
		{
			while (true)
			{
				double acctimehalf = 0.5 * percent[2 * i] * totalTime;
				double dectimehalf = 0.5 * percent[2 * i + 1] * totalTime;
				double uniformspeed = (totalPost - (startSpeed * acctimehalf + endSpeed * dectimehalf)) /
					(totalTime - acctimehalf - dectimehalf);
				// λ��Ϊ�������ٶ��ٶ�Ϊ��������λ��Ϊ�������ٶ��ٶ�Ϊ��
				if ((totalPost > 0 && uniformspeed < 0) || (totalPost < 0 && uniformspeed>0))
				{
					percent[2 * (i + 1)] = 2*totalPost/((startSpeed+endSpeed)*totalTime);
					percent[2 * (i + 1) + 1] = 2 * totalPost / ((startSpeed + endSpeed)*totalTime);
					break;
				}
				else
				{
					if (fabs(startSpeed - uniformspeed) < 1e-4 && fabs(endSpeed - uniformspeed) < 1e-4)
					{
						percent[2 * (i+1)] = 0;
						percent[2 * (i + 1) + 1] = 0;
						break;
					}
					else if ((fabs(startSpeed - uniformspeed) < 1e-4 && fabs(endSpeed - uniformspeed) > 1e-4)
						|| ((fabs(startSpeed - uniformspeed) > 1e-4 && fabs(endSpeed - uniformspeed) < 1e-4)))
					{
						percent[2 * (i + 1)] = percent[2 * (i + 1)] / 2;
						percent[2 * (i + 1) + 1] = percent[2 * (i + 1) + 1] / 2;
						break;
					}
					else
					{
						break;
					}
				}
			}
		}
		// S�͹滮
		else
		{
			while (true)
			{
				double acctimehalf = 0.5 * percent[2 * i]*totalTime;
				double dectimehalf = 0.5 * percent[2 * i + 1]*totalTime;
				double uniformspeed = (totalPost - (startSpeed * acctimehalf + endSpeed * dectimehalf)) /
					(totalTime - acctimehalf - dectimehalf);
				// λ��Ϊ�������ٶ��ٶ�Ϊ��������λ��Ϊ�������ٶ��ٶ�Ϊ��
				if ((totalPost > 0 && uniformspeed < 0) || (totalPost < 0 && uniformspeed>0))
				{
					percent[2 * (i + 1)] = 2 * totalPost / ((startSpeed + endSpeed)*totalTime);
					percent[2 * (i + 1) + 1] = 2 * totalPost / ((startSpeed + endSpeed)*totalTime);
					break;
				}
				else
				{
					if (fabs(startSpeed - uniformspeed) < 1e-4 && fabs(endSpeed - uniformspeed) < 1e-4)
					{
						percent[2 * (i + 1)] = 0;
						percent[2 * (i + 1) + 1] = 0;
						break;
					}
					else if((fabs(startSpeed - uniformspeed) < 1e-4 && fabs(endSpeed - uniformspeed) > 1e-4)
						|| ((fabs(startSpeed - uniformspeed) > 1e-4 && fabs(endSpeed - uniformspeed) < 1e-4)))
					{
						percent[2 * (i + 1)] = percent[2 * (i + 1)]/2;
						percent[2 * (i + 1) + 1] = percent[2 * (i + 1)+1] / 2;
						break;
					}
					else 
					{
						break;
					}
				}
			}
		}
	}
	if (flg)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}
