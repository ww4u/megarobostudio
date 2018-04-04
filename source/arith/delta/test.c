
#include "configure.h"
#include "dllMain.h"

void delta_test()                    
{
	/* 此为运动学解算库的输入参数 */
	double structureParams[4] = {132, 262.5, 100, 10};
	double angleLim[4] = {0, 1.9*PI, 0, 1.9*PI};
	double P0[2] = {0, -230}, angle0[2] = {0, 0};
	double pos1[N] = {-0.5236, 0.4488}, pos2[N] = {-2.6180, -1.9635};
	double vel1[N] = {0.01, 0.01}, vel2[N] = {0.02, 0.02};
	double acce1[N] = {0.01, 0.01}, acce2[N] = {0.02, 0.02};
	double err = 1e-5;
    int solveSet[4] = {FORWARD_KINEMATICS, TYPE1, ANTICLOCKWISE, N};		// mode, type, direction, length
//    int solveSet[4] = {INVERSE_KINEMATICS, TYPE1, ANTICLOCKWISE, N};		// mode, type, direction, length
	int flag, i;

	flag = solveDeltaKinematics(structureParams, angleLim, P0, angle0, pos1, pos2, vel1, vel2, acce1, acce2, solveSet, err);

	switch(flag)
	{
		case SOLVE_OVER:
			for(i = 0;i < solveSet[3];i ++)
			{
				printf("Position %d: %0.5f, %0.5f\r\n", i, pos1[i], pos2[i]);		// 显示位置信息
				printf("Velocity %d: %0.5f, %0.5f\r\n", i, vel1[i], vel2[i]);		// 显示速度信息
				printf("Acceleration %d: %0.5f, %0.5f\r\n", i, acce1[i], acce2[i]);		// 显示加速度信息

				printf("\r\n");
			}
			break;
		case INPUT_ERROR:
			printf("Error: Input error!\r\n");		// 显示错误信息
			break;
		case SOLVE_ERROR:
			printf("Error: Solve error!\r\n");		// 显示错误信息
			break;
		case SINGULAR_POSITION:
			printf("Error: Singular position!\r\n");		// 显示错误信息
			break;
		case EXCEED_LIMIT:
			printf("Error: Angle out of range!\r\n");		// 显示错误信息
			break;
	}
}
