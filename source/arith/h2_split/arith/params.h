#pragma once
// ��������������
#define error  1e-4
#define PI 3.1415926
#define E 2.7182818
#define ERROR_OUT_OF_ANGLE_LIMIT -1
#define ERROR_OUT_OF_WORK_SPACE -2
#define SUCCESS 0
#define ERROR 1E-4

/// ��⺯���ĵ�������
/// ���������1��armLength -- ���᳤�ȣ�[0]:�ⳤ��[1]:���[2]:���鳤��[3]:�����
/// 2��inputParam--���������[0]:ģ������:(2M),[1]:����,[2]dir �������ת�������� 0��˳ʱ�룬1����ʱ��,[3]:��ⷽʽ,[4]:�˶���ʽ(0����ת�˶���1��ֱ���˶�) 
/// 3��P0--ĩ�˳�ʼλ�ã�����ɸѡ��
/// 4:posIn -- ���Ϊ�����ĩ�˵㣬����Ϊ�ؽڽǶȣ� 5��vin -- ����Ϊ0 ���Ϊĩ���ٶȣ�6:len -- ����ĸ�����7��mode --���ģʽ 0Ϊ���� ��1Ϊ���
/// 8:movementMode 0:��ת�˶���1��ֱ���˶�
/// ���������res-- ���ķ���ֵ
/*extern "C" __declspec(dllexport)*/ int SolveStructureKinematics(double* armLength, int* inputParam, double* P0, double* posIn, double*vIn, double* tIn, int len, double* res);

