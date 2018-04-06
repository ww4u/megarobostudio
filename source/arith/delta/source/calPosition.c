//////////////////////////////////////////////////////////////
//             位置计算模块 calPosition.c
//                    Ver 0.1
//
// 功能：位置的正逆解求解
//
// Created by StarSky at March 28, 2018
//////////////////////////////////////////////////////////////

#include "configure.h"

/*************************************************************
* 输入：需要转换的角度
* 输出：无
* 
* 功能：将输入角度转换成数学模型下的参数/输出角度转换成实际机构下的参数
**************************************************************/
void angleTrans(double *data1, double *data2, int length, int mode, int direction)
{
    int i;
    double sign;

    if(mode == FORWARD_KINEMATICS)        // 正解
    {
        sign = -1.0;
    }
    else        //逆解
    {
        sign = 1.0;
    }

    if(direction == ANTICLOCKWISE)
        for(i = 0;i < length;i ++)        // 实际机构角度逆时针为正
        {
            data1[i] += sign * deltaCorrectParams.detTheta[0];        // 输入/出角度1转换
            data2[i] += sign * deltaCorrectParams.detTheta[1];        // 输入/出角度2转换
        }
    else
        for(i = 0;i < length;i ++)        // 实际机构角度顺时针为正
        {
            data1[i] = -data1[i] + sign * deltaCorrectParams.detTheta[0];        // 输入/出角度1转换
            data2[i] = -data2[i] + sign * deltaCorrectParams.detTheta[1];        // 输入/出角度2转换
        }

    /* 标准化角度值 */
    angleStandard(data1, length);
    angleStandard(data2, length);
}


/*************************************************************
* 输入：需要转换的角速度/加速度
* 输出：无
* 
* 功能：将角速度/角加速度在数学模型和实际机构间进行转换
**************************************************************/
void velAcceTrans(double *data1, double *data2, int length, int mode, int direction)
{
    int i;

    if(direction == CLOCKWISE)        // 实际机构顺时针为正
    {
        for(i = 0;i < length;i ++)            // 改变速度或加速度方向
        {
            data1[i] = -1.0 * data1[i];        // 输入/出速度/加速度1转换
            data2[i] = -1.0 * data2[i];        // 输入/出速度/加速度2转换
        }
    }
}


/*************************************************************
* 输入：位置正逆运动学求解的输入参数，主动关节/末端解，
*      从动关节解，解的类型（主从杆的构型），求解模式
* 输出：求解标志位
* 
* 功能：进行位置正逆运动学求解
* 
* 说明：当关节角度为模型角度，未进行转换
**************************************************************/
int positionKinematics(double posIn[2], double posOut[6], int type, int mode)
{
    int i, tmp, flag, dataSize[2] = {2, 2};
    double s1, c1, s2, c2, c2i, s2i, L1, L2, L3, LL1, LL2, LL3;
    double A[8], B[2], solution[4];
    double sgn, E, F, G;
    double det;
    double angleRegion1[2], angleRegion2[2];

    /* 机构参数获取 */
    L1 = deltaStructureParams.L1;
    L2 = deltaStructureParams.L2;
    L3 = deltaStructureParams.e - deltaStructureParams.d;
    LL1 = L1 * L1;
    LL2 = L2 * L2;
    LL3 = L3 * L3;
    for(i = 0;i < 2;i ++)
    {
        angleRegion1[i] = deltaStructureParams.angleLim[i];
        angleRegion2[i] = deltaStructureParams.angleLim[2 + i];
    }

    /* 正解 */
    if(mode == FORWARD_KINEMATICS)
    {     
        /* 三角函数计算 */
        s1 = sin(posIn[0]);     // sin(t1)
        c1 = cos(posIn[0]);     // cos(t1)
        s2 = sin(posIn[1]);     // sin(t2)
        c2 = cos(posIn[1]);     // cos(t2)
        
        /* 计算方程左端系数矩阵 */
        A[0] = 1.0;     
        A[1] = 1.0;
        A[2] = -2.0 * (L1 * c1 + L3);
        A[3] = -2.0 * L1 * s1;
        
        A[4] = 1.0;
        A[5] = 1.0;
        A[6] = -2.0 * (L1 * c2 - L3);
        A[7] = -2.0 * L1 * s2;
        
        /* 计算方程右端系数矩阵 */
        B[0] = -L3 * (2.0 * L1 * c1 + L3) + LL2 - LL1;        
        B[1] = -L3 * (-2.0 * L1 * c2 + L3) + LL2 - LL1;

        /* 末端位置求解 */
        flag = binaryQuadratic(A, B, solution, deltaStructureParams.err);     
        if(flag != SOLVE_OVER)
            return flag;        // 返回：求解失败
        /* 末端位置选取 */
        tmp = selPosition(solution, dataSize);        // TYPE1:就近原则选取
        if(type != TYPE1)        // 就远原则选取
        {
            if(tmp < 1)
            {    tmp = 1; }
            else
            {    tmp = 0; }
        }

        /* 判断角度是否超出范围 */
        if(judgeAngle(angleRegion1, &posIn[0], 1) == EXCEED_REGION || judgeAngle(angleRegion2, &posIn[1], 1) == EXCEED_REGION)
            return EXCEED_REGION;        // 返回： 输入错误

        posOut[0] = posIn[0];    // 保存主动关节1角度
        posOut[1] = posIn[1];    // 存储主动关节2角度

        for(i = 0;i < 2;i ++)        // 从动关节1和2角度求解
        {
            if(i % 2 == 0)
                sgn = 1.0;
            else
                sgn = -1.0;

            c2i = (solution[2 * tmp] - sgn * L3 - L1 * cos(posIn[i])) / L2;     // cost(2i)
            s2i = (solution[2 * tmp + 1] - L1 * sin(posIn[i])) / L2;        // sin(t2i)
            posOut[2 + i] = atan2(s2i,c2i);        // 从动关节i角度值
            angleStandard(&posOut[2 + i], 1);    // 标准化角度值  
        }

        posOut[4] = solution[2 * tmp];        // 保存末端坐标x值
        posOut[5] = solution[2 * tmp + 1];        // 保存末端坐标y值

    }
    /* 逆解 */
    else 
    {
        posOut[4] = posIn[0];    // 保存末端坐标x
        posOut[5] = posIn[1];    // 存储末端坐标y

        for(i = 0;i < 2; i ++)     // 求解主动关节角度值（2个解）
        {
            if(i % 2 == 0)
                sgn = 1.0;
            else
                sgn = -1.0;

            E = -2.0 * L1 * posIn[1];        // 第i个自由度中间变量计算
            F = -2.0 * L1 * (posIn[0] - sgn * L3);
            G = posIn[0] * posIn[0] + posIn[1] * posIn[1] + LL1 - LL2 + LL3 - sgn * 2.0 * L3 * posIn[0];

            det = E* E + F * F - G * G;     // 判别是否有解
            if(det < 0)
                return SOLVE_ERROR;     // 返回：无解
            else
                det = sqrt(det);
            
            // 按构型选择解
            switch(type)
            {                 
                case TYPE1:
                    break;         // case 1: 主从关节构型为外外
                case TYPE2:      // 主从关节构型为内内
                    sgn = -1.0 * sgn;
                    break;
                case TYPE3:      // 主从关节构型为内外
                    if(i==0)
                        sgn = -1.0 * sgn;
                    break;
                case TYPE4:      // 主从关节构型为外内
                    if(i==1)
                        sgn = -1.0 * sgn;
                    break;
            }     
                
            posOut[i] = 2.0 * atan((-E + sgn * det)/(G - F));      // 求解主动关节1，2的角度值
            angleStandard(&posOut[i], 1);    // 标准化角度值

            /* 判断角度是否超出范围 */
            if(i < 1)
            {
                if(judgeAngle(angleRegion1, &posOut[i], 1) == EXCEED_REGION)
                    return EXCEED_LIMIT;        // 返回： 主动关节角度1超出限制
            }
            else
            {
                if(judgeAngle(angleRegion2, &posOut[i], 1) == EXCEED_REGION)
                    return EXCEED_LIMIT;        // 返回：主动关节角度2超出限制
            }
                
        }
        
        /* 关节2角度值求解 */
        for(i = 0;i < 2; i ++) 
        {
            if(i % 2 == 0)
                sgn = 1.0;
            else
                sgn = -1.0;

            s1 = sin(posOut[i]);      // sin(theta1i)
            c1 = cos(posOut[i]);      // cos(theta1i)

            c2i = (posIn[0] - sgn * L3 - L1 * c1) / L2;     // cost(2i)
            s2i = (posIn[1] - L1 * s1) / L2;        // sin(t2i)
            posOut[2 + i] = atan2(s2i,c2i);       // 计算关节角度值: t21,t22 
            angleStandard(&posOut[2 + i], 1);    // 标准化角度值
        }
    }

    return SOLVE_OVER;        // 返回：计算完成    
}


/*************************************************************
* 输入：多组解，解的大小[每组元素个数，组数]
* 输出：解下标
* 
* 功能：以连续性条件选择唯一解
**************************************************************/
int selPosition(double *solution, int dataSize[2])
{
    int i, j, label;
    double det, detLast, tmp;

    for(i = 0;i < dataSize[1];i ++)        // 每组进行计算
    {
        det = 0;
        for(j = 0;j < dataSize[0];j ++)        // 距离计算
        {
            tmp = solution[i * dataSize[0] + j] - deltaCorrectParams.P0[j];
            det = det + tmp * tmp; 
        }
        if(i < 1)        // 初始化
        {
            detLast = det;        
            label = 0;
        }
        else
        {
            if(det < detLast)        // 选取最小值的下标
            {
                detLast = det;
                label = i;
            }
        }
    }

    return label;
}

/*************************************************************
* 输入：二元二次方程组系数{a10,a11,a12,a13,a20,a21,a21,a13}，
*      右端常数{b1,b2}，解数组{x1,y1,x2,y2}，误差
* 输出：解标志位
* 
* 功能：求解二次项系数相等的二元二次方程组
**************************************************************/
int binaryQuadratic(double A[8], double b[2], double solution[4], double err)
{
    int i;
    double det, tmp1, tmp2, B, C, D, E, F;

    /* 参数预处理 */
    if(fabs(A[0] - A[1]) > err || fabs(A[4] - A[5]) > err)      // 方程组的对应二次项系数必须相等   
        return INPUT_ERROR;       // 返回：系数错误
    
    /* 简化系数 */
    tmp1 = A[0];
    tmp2 = A[4];
    b[0] /= tmp1;
    b[1] /= tmp2;
    for(i = 0;i < 4;i ++)
    {
        A[i] /= tmp1;
        A[i + 4] /= tmp2;
    }
    
    /* 方程组求解 */
    det = A[3] - A[7];        // 计算y系数之差
    if(fabs(det) < err)     // y系数相等
    {
        det = A[2] - A[6];        // 计算x系数之差
        if(fabs(det) < err )     // x和y系数都为零,无确定的解
            return INPUT_ERROR;       // 返回：系数错误
        else        // x系数不为零，y系数为零
        {
            solution[0] = (b[0] - b[1]) / det;      // 解x1
            solution[2] = solution[0];              // 解x2（与x1一致）

            D = A[1];     // 二元一次方程系数：D*y^2+E*y+F = 0
            E = A[3];
            F = -b[0] + solution[0] * solution[0] + A[2] * solution[0]; 
            
            det = E * E - 4 * D * F;        // 解判别式
            if(det < 0)  
                return SOLVE_ERROR;     // 返回：无效解

            solution[1] = (-E + sqrt(det)) / (2.0 * D);       // 解y1
            solution[3] = (-E - sqrt(det)) / (2.0 * D);       // 解y2
        }
    }
    else      // y系数不为零
    {
        B = (b[0] - b[1]) / det;        // 各中间系数计算
        C = (A[6] - A[2]) / det;        // y = B+C*x

        D = C * C + 1.0;      // 二元一次方程系数：D*x^2+E*x    +F = 0
        E = 2.0 * B * C + A[2] + A[3] * C;
        F = B * B + A[3] * B - b[0];

        det = E * E - 4.0 * D * F;        // 解判别式
        if(det < 0)   
            return SOLVE_ERROR;     // 返回：无效解

        solution[0] = (-E + sqrt(det)) / (2.0 * D);       // 解x1
        solution[1] = B + C * solution[0];                // 解y1

        solution[2] = (-E - sqrt(det)) / (2.0 * D);       // 解x2
        solution[3] = B + C * solution[2];       // 解y2
    }
    
    return SOLVE_OVER;     // 返回：求解完成

}

