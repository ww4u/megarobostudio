
#include "./header/configure.h"
#include "./header/dllMain.h"

void delta_test()                    
{
    /* 此为运动学解算库的输入参数 */
    double structureParams[5] = {132, 262.5, 100, 10, -10};
    double angleLim[6] = {0, 1.9*PI, 0, 1.9*PI, 0, 1.9*PI};
    double P0[3] = {0, -230, 0}, angle0[3] = {0, 0, 0.5};
    /*double pos1[2] = {0, 113.7092}, pos2[2] = {-230.8074, -187.3488};
    double vel1[2] = {2.3732, 2.6792}, vel2[2] = {-0.98, -0.0765};
    double acce1[2] = {2.3937, 2.6921}, acce2[2] = {-0.9685, -0.00421};*/
    double pos1[N] = {0, 113.7092, -60}, pos2[N] = {-230.8074, -187.3488, -150}, pos3[N] = {0, 10, 20};
    double vel1[N] = {-2.3732, -2.6792, 0}, vel2[N] = {0.9807, 0.0765, 0}, vel3[N] = {0.1, 0.2, 0.3};
    double acce1[N] = {-2.3937, -2.6921, 0}, acce2[N] = {0.9685, 0.00421, 0}, acce3[N] = {0.1, 0.2, 0.3};
    double err = 1e-5;
    int solveSet[4] = {INVERSE_KINEMATICS, TYPE1, ANTICLOCKWISE, N};		// mode, type, direction, length

    int flag, i;

    flag = solveDeltaKinematics(structureParams, angleLim, P0, angle0, pos1, pos2, pos3, vel1, vel2, vel3, acce1, acce2, acce3, solveSet, err);

    switch(flag)
    {
        case SOLVE_OVER:
            for(i = 0;i < solveSet[3];i ++)
            {
                printf("Position %d: %0.5f, %0.5f, %0.5f\r\n", i, pos1[i]*180/PI, pos2[i]*180/PI, pos3[i]*180/PI);		// 显示位置信息
                printf("Velocity %d: %0.5f, %0.5f, %0.5f\r\n", i, vel1[i], vel2[i], vel3[i]);		// 显示速度信息
                printf("Acceleration %d: %0.5f, %0.5f, %0.5f\r\n", i, acce1[i], acce2[i], acce3[i]);		// 显示加速度信息

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
