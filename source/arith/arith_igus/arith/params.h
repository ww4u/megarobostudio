#pragma once
typedef struct
{
	// �˳�
	double armLength[4];
	// �ؽڳ�ʼ�Ƕȣ��ؽڳ�ʼλ��
	double offset[2];
	// �ؽڽǶ�Ϊ0��Ӧ�Ŀռ��
	double P0[3];
	// �ؽڽǶ�����
	double posLim[2];
	// ����
	double scal;
	// �ؽ�����ٶ�
	double vM;
	// 0:���� 1�����
	int mode;
}Igus;

/*extern "C" __declspec(dllexport) */int  GetIgusPvtLen(Igus *systemParam,double* posIn, double* vIn, double* tIn,int len,int* resLen);
/*extern "C" __declspec(dllexport) */int  GetIgusPvtInfo(double* resOut);
