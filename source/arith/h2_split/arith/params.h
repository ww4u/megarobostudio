#pragma once
// 常量及变量定义
#define error  1e-4
#define PI 3.1415926
#define E 2.7182818
#define ERROR_OUT_OF_ANGLE_LIMIT -1
#define ERROR_OUT_OF_WORK_SPACE -2
#define SUCCESS 0
#define ERROR 1E-4

/// 求解函数的导出函数
/// 输入参数：1：armLength -- 各轴长度，[0]:外长，[1]:外宽，[2]:滑块长，[3]:滑块宽。
/// 2：inputParam--输入参数，[0]:模具类型:(2M),[1]:齿数,[2]dir 两电机旋转的正方向 0：顺时针，1：逆时针,[3]:求解方式,[4]:运动方式(0：旋转运动，1：直线运动) 
/// 3：P0--末端初始位置，用于筛选解
/// 4:posIn -- 逆解为输入的末端点，正解为关节角度， 5：vin -- 正解为0 逆解为末端速度，6:len -- 求解点的个数，7：mode --求解模式 0为正解 ，1为逆解
/// 8:movementMode 0:旋转运动，1：直线运动
/// 输出参数：res-- 求解的返回值
/*extern "C" __declspec(dllexport)*/ int SolveStructureKinematics(double* armLength, int* inputParam, double* P0, double* posIn, double*vIn, double* tIn, int len, double* res);

