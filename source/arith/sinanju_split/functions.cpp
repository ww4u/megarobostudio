

#include "math.h"
#include <vector>
#include <algorithm>

using namespace std;

//! configs
#define ARITH_ERROR (1E-4)
#define ANGLE_ERROR (1E-3)
#define POSITION_ERROR  (0.1)

//#define armLen0  247.75
//#define armLen1  255
//#define armLen2  250
//#define armLen3 0
//#define armLen4 0
//#define armLen5 0

#define PI 3.1415926
#define E 2.7182818

struct ResInfo
{
    double P[4];
    double V[4];
    double T;
};

struct tpvPoint
{
    double mT;
    double mP;
    double mV;

    tpvPoint( double t = 0, double p = 0, double v = 0 )
    {
        mT = t;
        mP = p;
        mV = v;
    }
};

//! a * pi/180
static void degToRad(double *degs,
    int count,
    double *rads)
{
    for (int i = 0; i < count; i++)
    {
        rads[i] = degs[i] * PI / 180;
    }
}

//! a * 180/pi
static void radToDeg(double *rads,
    int count,
    double *degs)
{
    for (int i = 0; i < count; i++)
    {
        degs[i] = rads[i] * 180 / PI;
    }
}

//! vector op
static void vectorAdd(double *pIn1,
    double *pIn2,
    int count,
    double *pOut)
{
    for (int i = 0; i < count; i++)
    {
        pOut[i] = pIn1[i] + pIn2[i];
    }
}

static void vectorSub(double *pIn1,
    double *pIn2,
    int count,
    double *pOut)
{
    for (int i = 0; i < count; i++)
    {
        pOut[i] = pIn1[i] - pIn2[i];
    }
}

//! normalize to [ -PI, PI ]
static double normalize_pi(double a)
{
    while (a > PI)
    {
        a -= PI * 2;
    }

    while (a < -PI)
    {
        a += PI * 2;
    }

    return a;
}

static void normalize_pis(double *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        p[i] = normalize_pi(p[i]);
    }
}

//! linear x by a, b, m
static double linearSlove( double a, double b, double m,
                      double c, double d )
{
    //! ( m - a ) / ( b - m ) = ( x-c)/(d-x)

    if ( fabs( b - a ) < ARITH_ERROR )
    { return (c+d)/2; }

    if ( fabs( d - c ) < ARITH_ERROR )
    { return (c+d)/2; }

    if ( fabs( m - a ) < ARITH_ERROR )
    { return c; }

    if ( fabs( m - b ) < ARITH_ERROR )
    { return d; }

    //! linear poly
    double x;

    x = ( ( m - a) * d + ( b - m ) *c ) / ( b - a );

    return x;
}

static bool CheckError(double data)
{
    //! over range
    if (data < (-1 - ARITH_ERROR) || data>(1 + ARITH_ERROR))
    {
        return false;
    }
    else
    {
        return true;
    }
}

static int CheckPos(int flg[2], double input[4], int len)
{
    //! 存在有效解
    if (flg[0] == 1 || flg[1] == 1)
    {
        //! 存在两个有效解
        //! 1 1
        if (flg[0] == 1 && flg[1] == 1)
        {
            for (int i = 0; i < len; i++)
            {
                if (input[i] > PI || input[i] < -PI)
                {
                    return 0;
                }
            }
            return 3;
        }
        //! 1 0
        else if (flg[0] == 1)
        {
            for (int j = 0; j < 4; j++)
            {
                if (input[j] > PI || input[j] < -PI)
                {
                    return 0;
                }
            }
            return 1;
        }
        //! 0 0
        //! 0 1
        else
        {
            for (int j = 4; j < len; j++)
            {
                if (input[j] > PI || input[j] < -PI)
                {
                    return 0;
                }
            }
            return 2;
        }
    }

    return 0;
}

static double* CalPose(double* armLength, int armCount,
    double* pIn, double* pLast, int index,
    double pose[16])
{
    //! 末端相对水平的夹角
    double theta = pLast[1] + pLast[2] + pLast[3];
    double d = armLength[4];
    double px = pIn[0];
    double py = pIn[1];
    double temp = sqrt(px * px + py * py);
    double det = -armLength[5] * sin(theta) + armLength[3] * cos(theta);
    double t1 = 0;

    if (index == 0)
    {
        if (d != 0)
        {
            if (det >= 0)
            {
                t1 = atan2(py, px) - asin(-d / temp);
            }
            else
            {
                t1 = atan2(-py, -px) - asin(d / temp);
            }
        }
        else
        {
            if (det >= 0)
            {
                t1 = atan2(py, px);
            }
            else
            {
                t1 = atan2(-py, -px);
            }
        }
    }
    //! index != 0
    else
    {
        if (d != 0)
        {
            if (det > 0)
            {
                t1 = atan2(-py, -px) - asin(d / temp);
            }
            else
            {
                t1 = atan2(py, px) - asin(-d / temp);
            }
        }
        else
        {
            if (det > 0)
            {
                t1 = atan2(-py, -px);
            }
            else
            {
                t1 = atan2(py, px);
            }
        }
    }

    //! 末端姿态
    pose[0] = cos(t1) * cos(theta);
    pose[1] = -cos(t1) * sin(theta);
    pose[2] = sin(t1);
    pose[3] = pIn[0];

    pose[4] = sin(t1)* cos(theta);
    pose[5] = -sin(t1) * sin(theta);
    pose[6] = -cos(t1);
    pose[7] = pIn[1];

    pose[8] = sin(theta);
    pose[9] = cos(theta);
    pose[10] = 0;
    pose[11] = pIn[2];

    pose[12] = 0;
    pose[13] = 0;
    pose[14] = 0;
    pose[15] = 1;

    return pose;
}

static double* GetPosition(int index, double* input, double res[4])
{
    if (index == 1)
    {
        for (int i = 0; i < 4; i++)
        {
            res[i] = input[2 * i];
        }

        return res;
    }
    else if (index == 2)
    {
        for (int i = 0; i < 4; i++)
        {
            res[i] = input[2 * i + 1];
        }

        return res;
    }
    else
    {
        return NULL;
    }
}

static double* SelPos(double* pLast, double* input, double result[4])
{
    double temp[2];

    //! sum
    for (int i = 0; i < 2; i++)
    {
        temp[i] = 0;
        for (int j = 0; j < 4; j++)
        {
            temp[i] = temp[i] + pow(input[2 * j + i] - pLast[j], 2);
        }
    }

    //! check
    if (temp[0] > temp[1])
    {
        return GetPosition(2, input, result);
    }
    else
    {
        return GetPosition(1, input, result);
    }
}

//! return -- NULL: no slove
static double* CalPosition(double* armLength, int armCount,
    double* posIn,     //! [16]
    double* posLast,   //! [4]

    double result[4])
{
    //	int len = 4*2;
    //	double* posOrg = new double[len];
    //	memset(posOrg, 0, len * sizeof(double));

    //! 初始化位姿矩阵
    double posOrg[8];

    //	int* mark = new int[2];

        //! 初始化标志
    int mark[2] = { 0, 0 };

    //! 关节1角度
    //! [0] [1]
    posOrg[0] = atan2(posIn[2], -posIn[6]);
    posOrg[1] = posOrg[0];

    //! local vars
    double c0 = cos(posOrg[0]);
    double s0 = sin(posOrg[0]);
    double bx = posIn[3] - c0 * armLength[3] * posIn[9] - armLength[4] * s0 + c0 * armLength[5] * posIn[8];
    double by = posIn[7] - s0 * armLength[3] * posIn[9] + armLength[4] * c0 + s0 * armLength[5] * posIn[8];
    double bz = posIn[11] - armLength[3] * posIn[8] - armLength[5] * posIn[9];

    double A, B, C;
    if (fabs(c0) > POSITION_ERROR)
    {
        //! 计算各关节位置
        A = pow(c0, 2)*(pow(bz - armLength[0], 2)
            - pow(armLength[1], 2)
            - pow(armLength[2], 2))
            + pow(bx, 2);
        B = 2 * pow(c0, 2)* armLength[1] * armLength[2];
    }
    else
    {
        A = pow(s0, 2)*(pow(bz - armLength[0], 2)
            - pow(armLength[1], 2)
            - pow(armLength[2], 2))
            + pow(by, 2);
        B = 2 * pow(s0, 2)* armLength[1] * armLength[2];
    }

    double delt = A / B;
    bool flg = CheckError(delt);        //! <-1 || > 1

    //! slove
    if (flg)
    {
        posOrg[4] = acos(delt);
        posOrg[5] = -acos(delt);

        //! mark
        mark[0] = 1;
        mark[1] = 1;

        //! 唯一解
        if (posOrg[4] - posOrg[5] < ARITH_ERROR)
        {
            mark[1] = 0;
        }
        else
        {
        }
    }
    //! fail
    else
    {
        return NULL;
    }

    for (int i = 0; i < 2; i++)
    {
        //! [4] [5]
        double c2 = cos(posOrg[4 + i]);
        double s2 = sin(posOrg[4 + i]);
        if (fabs(c0) > POSITION_ERROR)
        {
            //! 计算各关节位置
            A = -bx * s2* armLength[2] + (bz - armLength[0])*c0*(armLength[1] + c2 * armLength[2]);
            B = (armLength[1] + c2 * armLength[2])*bx + c0 * s2* armLength[2] * (bz - armLength[0]);
            C = c0 * (pow(armLength[1], 2) + 2 * c2* armLength[1] * armLength[2] + pow(armLength[2], 2));
        }
        else
        {
            A = -by * s2* armLength[2] + (bz - armLength[0])*s0*(armLength[1] + c2 * armLength[2]);
            B = (armLength[1] + c2 * armLength[2])*by + s0 * s2* armLength[2] * (bz - armLength[0]);
            C = s0 * (pow(armLength[1], 2) + 2 * c2* armLength[1] * armLength[2] + pow(armLength[2], 2));
        }

        double s1 = A / C;
        double c1 = B / C;
        bool flg1 = CheckError(s1);
        bool flg2 = CheckError(c1);

        //! 有解
        if (flg1 && flg2)
        {
            //! posOrg[2], posOrg[3]
            posOrg[2 + i] = atan2(s1, c1);
            //! posOrg[6], posOrg[7]
            posOrg[6 + i] = atan2(posIn[8], posIn[9]) - posOrg[1 * 2 + i] - posOrg[2 * 2 + i];
        }
        //! fail
        else
        {
            mark[i] = 0;
            return NULL;
        }
    }

    //! 角度标准化
    normalize_pis(posOrg, 8);

    //! 解的个数及索引位置
    int count = CheckPos(mark, posOrg, 8);

    switch (count)
    {
        // 无解
    case 0:
        return NULL;

    case 1:
        return GetPosition(1, posOrg, result);

    case 2:
        return GetPosition(2, posOrg, result);

    case 3:
        return SelPos(posLast, posOrg, result);

    default:
        return NULL;
    }
}

//! return: 0 -- equal
static int CompareAngle(double data1, double date2)
{
    //    double n1, n2;

    //    n1 = normalize_pi( data1 );
    //    n2 = normalize_pi( date2 );

    //    if ( (n1 - n2) > ANGLE_ERROR )
    //    { return 1; }
    //    else if ( (n1 - n2) < -ANGLE_ERROR )
    //    { return -1; }
    //    else
    //    { return 0; }

    double delt = fabs(data1 - date2);
    if (delt > PI)
    {
        delt = fabs(delt - 2 * PI);
    }
    if (delt < 1e-3)
    {
        return 0;
    }
    return -1;
}

static void CalV(double pos[7],
    double v[3],

    double poseV[6]       //! out
)                         //! 0 -- v[0]
                          //! 1 -- v[1]
                          //! 2 -- v[2]
                          //! 3 --
                          //! 4 --
                          //! 5 -- tail speed
{
    //! 末端速度
    double c1 = cos(pos[0]);
    double s1 = sin(pos[0]);

    double px = pos[4];
    double py = pos[5];

    double vx = v[0];
    double vy = v[1];

    //! 保存末端速度
    poseV[0] = v[0];
    poseV[1] = v[1];
    poseV[2] = v[2];

    poseV[3] = 0;
    poseV[4] = 0;

    //! 计算末端角速度
    poseV[5] = (c1 * vy - s1 * vx) / (c1 * px + s1 * py);
}

static double BizarreAvoid(double input)
{
    double temp;
    if (fabs(input) < POSITION_ERROR)
    {
        temp = 0.1 * (1 - fabs(input) / 0.1);       //! \todo why 0.1?
    }
    else
    {
        temp = 0;
    }

    return input / (pow(input, 2) + pow(E, -fabs(input)) * pow(temp, 2));
}

static void CalVelocity(double *armLength, int armCount,
    double pos[4],
    double v[6],

    double vOut[4]
)
{
    //! local vars
    double s1 = sin(pos[0]);
    double c1 = cos(pos[0]);
    double s2 = sin(pos[1]);
    double c2 = cos(pos[1]);
    double s3 = sin(pos[2]);
    double s2a3 = sin(pos[1] + pos[2]);
    double c2a3 = cos(pos[1] + pos[2]);
    double s2a3a4 = sin(pos[1] + pos[2] + pos[3]);
    double c2a3a4 = cos(pos[1] + pos[2] + pos[3]);

    //! ---------------速度雅克比矩阵计算---------------
    double J[6][4];

    double temp1 = -armLength[3] * c1*s2a3a4 - armLength[5] * c1*c2a3a4;
    J[0][0] = -armLength[1] * s1 * c2 - armLength[2] * s1 * c2a3 - s1 * armLength[3] * c2a3a4 + armLength[4] * c1 + armLength[5] * s1*s2a3a4;
    J[0][1] = -armLength[1] * c1 * s2 - armLength[2] * c1 * s2a3 + temp1;
    J[0][2] = -armLength[2] * c1 * s2a3 + temp1;
    J[0][3] = temp1;

    double temp2 = -armLength[3] * s1 * s2a3a4 - armLength[5] * s1 * c2a3a4;
    J[1][0] = armLength[1] * c1 * c2 + armLength[2] * c1 * c2a3 + c1 * armLength[3] * c2a3a4 + armLength[4] * s1 - armLength[5] * c1*s2a3a4;
    J[1][1] = -armLength[1] * s1 * s2 - armLength[2] * s1 * s2a3 + temp2;
    J[1][2] = -armLength[2] * s1*s2a3 + temp2;
    J[1][3] = temp2;

    double temp3 = armLength[3] * c2a3a4 - armLength[5] * s2a3a4;
    J[2][1] = armLength[1] * c2 + armLength[2] * c2a3 + temp3;
    J[2][2] = armLength[2] * c2a3 + temp3;
    J[2][3] = temp3;

    J[3][1] = s1;
    J[3][2] = s1;
    J[3][3] = s1;

    J[4][1] = -c1;
    J[4][2] = -c1;
    J[4][3] = -c1;

    J[5][0] = 1;

    //	double* vOut = new double[4];
    //	memset(vOut, 0, 4 * sizeof(double));

        //! v out
    vOut[0] = v[5];

    //! 奇偶规避
    double P = BizarreAvoid(s3);
    if (fabs(c1) > POSITION_ERROR)
    {
        double A = J[0][1] - J[0][3];
        double B = J[0][2] - J[0][3];
        double C = J[2][1] - J[2][3];
        double D = J[2][2] - J[2][3];

        double b1 = v[0] - (J[0][0] * vOut[0] + J[0][3] * v[4] / J[4][3]);
        double b2 = v[2] - v[4] * J[2][3] / J[4][3];

        P = P / (armLength[1] * armLength[2] * c1);
        vOut[1] = (D*b1 - B * b2)*P;
        vOut[2] = -(C*b1 - A * b2)*P;
        vOut[3] = v[4] / J[4][3] - vOut[1] - vOut[2];
    }
    else
    {
        double A = J[1][1] - J[1][3];
        double B = J[1][2] - J[1][3];
        double C = J[2][1] - J[2][3];
        double D = J[2][2] - J[2][3];

        double b1 = v[1] - (J[1][0] * vOut[0] + J[1][3] * v[3] / J[3][3]);
        double b2 = v[2] - v[4] * J[2][3] / J[3][3];

        P = P / (armLength[1] * armLength[2] * s1);
        vOut[1] = (D*b1 - B * b2)*P;
        vOut[2] = -(C*b1 - A * b2)*P;
        vOut[3] = v[3] / J[3][3] - vOut[1] - vOut[2];
    }
}

static void GetContinueAngle(double* last,
    double* pos,        //! [0,count)
    int count,

    double* pRet)
{
    //	double* res = new double[4];

    for (int i = 0; i < count; i++)
    {
        double temp1 = last[i] - floor(last[i] / (2 * PI)) * 2 * PI;
        double temp2 = pos[i] - floor(pos[i] / (2 * PI)) * 2 * PI;
        double delt = temp2 - temp1;

        delt = normalize_pi(delt);

        pRet[i] = last[i] + delt;
    }
}

static void appendTpvLine( vector<tpvPoint> &tpvLine,

                        double t,
                        double p,
                        double v
                       )
{
    tpvLine.push_back( tpvPoint( t, p, v ) );
}

//! v stick to 0
static void stickTpvLine( vector<tpvPoint> &tpvLine )
{
    for ( int i = 0; i < tpvLine.size(); i++ )
    {
        if ( fabs(tpvLine.at(i).mV) < ARITH_ERROR )
        { tpvLine.at(i).mV = 0; }
    }
}

//! find the cross point
static void zeroCrossTpvLine( vector<tpvPoint> &tpvLine,
                              vector<tpvPoint> &outLine )
{
    double t,p,v,t1;
    double dT;

    //! more than 2
    if ( tpvLine.size() < 2 )
    { return; }

    //! for each
    for ( int i = 0; i < tpvLine.size() - 1; i++ )
    {
        outLine.push_back( tpvLine.at(i) );

        //! cross zero
        if ( tpvLine.at(i).mV * tpvLine.at(i+1).mV < 0 )
        {
            //! calc the new point
            t = 0;
            p = 0;
            v = 0;

            //! guess the p at v=0
            dT = tpvLine.at( i + 1 ).mT - tpvLine.at( i ).mT;
            t1 = dT * ( -tpvLine.at( i ).mV / ( tpvLine.at(i+1).mV - tpvLine.at( i ).mV ) );

            t = tpvLine.at(i).mT + t1;
            p = tpvLine.at(i).mP + tpvLine.at(i).mV * t1 / 2;

            outLine.push_back( tpvPoint( t, p, v ) );
        }
        else
        {

        }
    }

    //! the last one
    outLine.push_back( tpvLine.at( tpvLine.size() - 1 ) );
}

//! union all the line
static void unionTpvLineTime( vector<tpvPoint> &tpvLine,
                              vector<double> &timeLine )
{
    for ( int i = 0; i < tpvLine.size(); i++ )
    {
        timeLine.push_back( tpvLine.at(i).mT );
    }
}

//! sort and trim by err
//! asc
static void trimTpvLineTime( vector<double> &timeLine,
                             double err = ARITH_ERROR )
{
    //! <
    sort( timeLine.begin(), timeLine.end() );

    //! trim by resolution
    vector<double> backup = timeLine;

    //! clear
    timeLine.clear();

    double tNow;

    //! the first
    tNow = backup.at(0);
    timeLine.push_back( tNow );

    //! for next
    for ( int i = 1; i < backup.size(); i++ )
    {
        if ( (backup.at(i) - tNow ) > err )
        {
            tNow = backup.at(i);
            timeLine.push_back( tNow );
        }
    }
}

//! sample the line by time
static void resampleTpvLine( vector<tpvPoint> &tpvLine,
                             vector<double> &timeLine,
                             vector<tpvPoint> &tpvOutLine )
{
    double t;

    double xt, xp, xv;

    //! init
    tpvOutLine.clear();

    for ( int i = 0; i < timeLine.size(); i++ )
    {
        t = timeLine.at(i);

        do
        {
            //! 1. low than the first
            if ( t <= tpvLine.at(0).mT )
            {
                //! stick to the first
                tpvOutLine.push_back( tpvLine.at(0) );
                break;
            }
            else
            { }

            //! 2. find t
            for ( int j = 1; j < tpvLine.size(); j++ )
            {
                //! more than
                if ( t > tpvLine.at(j).mT )
                {}
                //! t <= tj
                else
                {
                    xp = linearSlove( tpvLine.at(j-1).mT, tpvLine.at(j).mT, t,
                                      tpvLine.at(j-1).mP, tpvLine.at(j).mP );
                    xv = linearSlove( tpvLine.at(j-1).mT, tpvLine.at(j).mT, t,
                                      tpvLine.at(j-1).mV, tpvLine.at(j).mV );

                    tpvOutLine.push_back( tpvPoint( t, xp, xv ) );
                    goto _nnext;
                }
            }

            //! 3. last one, t not in the range
            tpvOutLine.push_back( tpvPoint( t,
                                            tpvLine.at( tpvLine.size() -1 ).mP,
                                            tpvLine.at( tpvLine.size() -1 ).mV
                                            ) );

        }while(0);
_nnext:
        //! next iter
        ;
    }
}

static void zeroCrossProc( vector<ResInfo> &infoList,
                           int lineCount )
{
    vector<tpvPoint> tpvLine[4];
//    lineCount = sizeof(tpvLine)/sizeof( tpvLine[0] );

    //! 1. to line
    for ( int i = 0; i < infoList.size(); i++ )
    {
        for ( int j = 0; j < lineCount; j++ )
        {
            appendTpvLine( tpvLine[j], infoList.at(i).T, infoList.at(i).P[j], infoList.at(i).V[j] );
        }
    }

    //! 2. foreach line stick
    for( int i = 0; i < lineCount; i++ )
    {
        stickTpvLine( tpvLine[i] );
    }

    //! 3. zcp line
    vector<tpvPoint> tpvOutLine[4];
    for ( int i = 0; i < lineCount; i++ )
    {
        zeroCrossTpvLine( tpvLine[i], tpvOutLine[i] );
    }

    //! time line
    vector<double> timeLine;
    for ( int i = 0; i < lineCount; i++ )
    {
        unionTpvLineTime( tpvOutLine[i], timeLine );
    }
    trimTpvLineTime( timeLine );

    //! resample
    for ( int i = 0; i < lineCount; i++ )
    {
        tpvLine[i].clear();
        resampleTpvLine( tpvOutLine[i], timeLine, tpvLine[i] );
    }

    //! export
    infoList.clear();
    ResInfo localInfo;
    for ( int i = 0; i < timeLine.size(); i++ )
    {
        //! set info
        localInfo.T = timeLine.at(i);

        for ( int j = 0; j < lineCount; j++ )
        {
            localInfo.P[j] = tpvLine[j].at(i).mP;
            localInfo.V[j] = tpvLine[j].at(i).mV;
        }

        infoList.push_back( localInfo );
    }
}

#ifdef QT_VERSION
//! debug
#include <QDebug>
#include <QFile>

int zeroCrossTest()
{
    //! data
//
    double ps[]={ +0, 1, 2, +1, };

    double vs0[]={ -1, 1, 1, -1, };
    double vs1[]={ -1, 2, 4, -3, };
    double vs2[]={ -2, 3, 2, -5, };
    double vs3[]={ -0, 1, 3, -1, };

//    double vs0[]={ +1, +2, +3, +4, +5, +6, +7 };
//    double vs1[]={ +1, +2, +3, +4, +5, +6, +7 };
//    double vs2[]={ +1, +2, +3, +4, +5, +6, +7 };
//    double vs3[]={ +1, +2, +3, +4, +5, +6, +7 };
//    double ps[]={ +0, +1, +2, +3, +4, +5, +6 };

//    double vs0[]={ -1, -2, -3, -4, -5, -6, -7 };
//    double vs1[]={ -1, -2, -3, -4, -5, -6, -7 };
//    double vs2[]={ -1, -2, -3, -4, -5, -6, -7 };
//    double vs3[]={ -1, -2, -3, -4, -5, -6, -7 };
//    double ps[]={ +0, -1, -2, -3, -4, -5, -6 };

    vector<ResInfo> infoList;
    ResInfo aInfo;
    for( int i = 0; i < sizeof(vs0)/sizeof(vs0[0]); i++ )
    {
        for ( int j = 0; j < 1; j++ )
        {
            aInfo.V[j] = vs0[i];
//            aInfo.V[j] = vs1[i];
            aInfo.P[j] = ps[i];
        }

        for ( int j = 1; j < 2; j++ )
        {
            aInfo.V[j] = vs1[i];
            aInfo.P[j] = ps[i];
        }

        for ( int j = 2; j < 3; j++ )
        {
            aInfo.V[j] = vs2[i];
            aInfo.P[j] = ps[i];
        }

        for ( int j = 3; j < 4; j++ )
        {
            aInfo.V[j] = vs3[i];
            aInfo.P[j] = ps[i];
        }


        aInfo.T = i;

        infoList.push_back( aInfo );
    }

    //! calc, cross check only
    int lineCnt = 4;
    zeroCrossProc( infoList, lineCnt );

    if ( infoList.size() > 0 )
    { }
    else
    { return -1; }

    //! for save
    QFile file("E:/trash/a.csv");
    file.open( QIODevice::WriteOnly );
    QTextStream textStream( &file );

    for ( int i = 0; i < infoList.size(); i++ )
    {
        textStream<<infoList.at(i).T<<",";
        for ( int j = 0; j < lineCnt; j++ )
        {
            textStream<<infoList.at(i).P[j]<<","<<infoList.at(i).V[j]<<",";
        }

        textStream<<"\n";
    }

    file.close();

    return 0;
}
#endif

// 正解求末端位置
// armLength 机械臂的臂长
// angles--四个关节的当前角度
// res--正解的末端位置(X,Y,Z)

//! arm count: 6
//!
//!               2
//!             ____
//!            /     | 3
//!         1 /      |
//!          /    4------
//!         ||          | 5
//!         || 0        |
//!         ||
//! return: 0 -- no error
int MEGA_EXPORT GetEndPosition(
    double* armLengthArray,     //! foreach arm
    int armCount,		//! arm count: 6

//    double* deltAngles,		//! delta angle: degree
//    double*

    double *rotAngles,
    double *archAngles,
    double *deltaAngles,

//    double* angles,		//! current angle 0~360
    int angleCount,             //! 4

    double* res)		//! out: x, y, z
{
    if (NULL == armLengthArray
        || NULL == rotAngles
        || NULL == archAngles
        || NULL == deltaAngles
        || NULL == res)
    {
        return -1;
    }

    double localRads[4];

    //! delta + arch
    vectorAdd( deltaAngles, archAngles, angleCount, localRads );

    //! - rot Angle
    vectorSub( localRads, rotAngles, angleCount, localRads );
                                                        //! Ms Builder can not variable array
                                                        //! compensation

    degToRad(localRads, angleCount, localRads);       //! convert angle to rad

    normalize_pis( localRads, angleCount );
                                                        //! const vars
    double s1, c1, s2, c2, s2a3, c2a3, s2a3a4, c2a3a4;

    s1 = sin(localRads[0]);     						//! sin(theta1)
    c1 = cos(localRads[0]);     						//! cos(theta1)
    s2 = sin(localRads[1]);     						//! sin(theta2)
    c2 = cos(localRads[1]);     						//! cos(theta2)
    s2a3 = sin(localRads[1] + localRads[2]);      		//! sin(theta2 + theta3)
    c2a3 = cos(localRads[1] + localRads[2]);      		//! cos(theta2 + theta3)
    s2a3a4 = sin(localRads[1] + localRads[2] + localRads[3]);      //! sin(theta2 + theta3 + theta4)
    c2a3a4 = cos(localRads[1] + localRads[2] + localRads[3]);      //! cos(theta2 + theta3 + theta4)

                                                        //! end pos
    double endOut[4][4];
    //	double endX, endY, endZ;

    //	endOut[0][0] = c1 * c2a3a4;   						// 计算位姿矩阵
    //	endOut[0][1] = -c1 * s2a3a4;
    //	endOut[0][2] = s1;
    endOut[0][3] = armLengthArray[4] * s1 - armLengthArray[5] * c1 * s2a3a4 + c1 * (armLengthArray[1] * c2 + armLengthArray[2] * c2a3 + armLengthArray[3] * c2a3a4);

    //	endOut[1][0] = s1* c2a3a4;
    //	endOut[1][1] = -s1* s2a3a4;
    //	endOut[1][2] = -c1;
    endOut[1][3] = -c1 * armLengthArray[4] - armLengthArray[5] * s1 * s2a3a4 + s1 * (armLengthArray[1] * c2 + armLengthArray[2] * c2a3 + armLengthArray[3] * c2a3a4);

    //	endOut[2][0] = s2a3a4;
    //	endOut[2][1] = c2a3a4;
    endOut[2][3] = armLengthArray[5] * c2a3a4 + armLengthArray[0] + armLengthArray[1] * s2 + armLengthArray[2] * s2a3 + armLengthArray[3] * s2a3a4;

    //	endOut[3][3] = 1;

    res[0] = endOut[0][3];
    res[1] = endOut[1][3];
    res[2] = endOut[2][3];

    return 0;
}

// 逆解运算
// posLast -- 参考位置，用于确定最优路径,输入为4个关节的角度
// posIn -- 用于逆解的末端位置(X,Y,Z)
// vIn--末端速度(VX,VY,VZ)
// tIn--时间节点
// 求解的点的个数
// 求解的返回值

//! return : 0 -- no error
int sloveArmPosition(
    double* armLength, int armCount,

//    double* deltAngles, int angleCount,    //! angles: [0~3]
//    double* posRef,                        //!

    double *rotAngles,
    double *archAngles,

    int angleCount,

    double* posIn, int skipP,
    double* vIn, int skipV,
    double* tIn, int skipT,
    int len,                                //! points len in

    vector<ResInfo> &resInfo )
{
    double posLast[4];

    //! rotate the angle
    vectorSub( archAngles, rotAngles, angleCount, posLast );

    //! to rad
    degToRad(posLast, angleCount, posLast);                   //! last * 180/pi

    //! normalize
    normalize_pis(posLast, angleCount);

    //! cache clear
    resInfo.clear();

    //! for each point
    for (int i = 0; i < len; i++)
    {
        //! foreach axis pv
        //!
        double P[3];
        double V[3];
        for (int j = 0; j < 3; j++)
        {
            P[j] = posIn[skipP * i + j];
            V[j] = vIn[skipV * i + j];
        }

        //! epoch
        //!
        double pose[16];
        double poseOut[4];
        for (int k = 0; k < 2; k++)
        {
            //! 末端位姿
            if (NULL == CalPose(armLength, armCount,
                P, posLast, k, pose))
            {
                return -1;
            }

            //! 求解关节位置
            if (NULL == CalPosition(armLength, armCount,
                pose, posLast, poseOut))
            {
                return -1;
            }

            //! same as before
            double p1 = posLast[1] + posLast[2] + posLast[3];
            double p2 = poseOut[1] + poseOut[2] + poseOut[3];

            //! angle == angle2
            if (0 == CompareAngle(p1, p2))
            {
                break;
            }
        }

        ResInfo info;
        // if (poseOut != NULL)
        {
            double allPos[7];
            allPos[0] = poseOut[0];
            allPos[1] = poseOut[1];
            allPos[2] = poseOut[2];
            allPos[3] = poseOut[3];
            allPos[4] = P[0];
            allPos[5] = P[1];
            allPos[6] = P[2];

            //! end v
            double endV[6];
            CalV(allPos, V, endV);        //! endV[3],endV[4] invalid

            double vOut[4];
            CalVelocity(armLength, armCount,
                        poseOut, endV,
                        vOut);

            //! pos
            double newPose[4];
            GetContinueAngle(posLast,
                            poseOut,
                            angleCount,
                            newPose
                            );

            memcpy( info.P, newPose, sizeof(newPose) );

            memcpy( info.V, vOut, sizeof(vOut) );

            info.T = tIn[i*skipT];
        }

        resInfo.push_back(info);
    }

    //! add
    zeroCrossProc( resInfo, 4 );

    return 0;
}

//! return: 0 -- no error
int  MEGA_EXPORT getArmPosition_Size(
    double* armLength, int armCount,

//    double* deltAngles, int angleCount,    //! angles: [0~3]
//    double* posRef,                        //!

    double *rotAngles,
    double *archAngles,

    int angleCount,

    double* posIn, int skipP,
    double* vIn, int skipV,
    double* tIn, int skipT,
    int len,                               //! points len in
    int *pOutSize
                                )
{
    vector<ResInfo> resInfo;

    int ret;
    ret = sloveArmPosition( armLength, armCount,
//                            deltAngles, angleCount,
//                            posRef,
                            rotAngles,
                            archAngles,
                            angleCount,

                            posIn, skipP,
                            vIn, skipV,
                            tIn, skipT,
                            len,
                            resInfo );

    if ( ret != 0 )
    { return ret; }

    *pOutSize = resInfo.size();
    return 0;
}

int  MEGA_EXPORT getArmPosition_Data(
    double* armLength, int armCount,

//    double* deltAngles, int angleCount,    //! angles: [0~3]
//    double* posRef,                        //!

    double *rotAngles,
    double *archAngles,

    int angleCount,

    double* posIn, int skipP,
    double* vIn, int skipV,
    double* tIn, int skipT,
    int len,                               //! points len in

    double *outRes,
    int nCap
    )
{
    //! slove
    vector<ResInfo> resInfo;

    int ret;
    ret = sloveArmPosition( armLength, armCount,
//                            deltAngles, angleCount,
//                            posRef,

                            rotAngles,
                            archAngles,
                            angleCount,

                            posIn, skipP,
                            vIn, skipV,
                            tIn, skipT,
                            len,
                            resInfo );

    if ( ret != 0 )
    { return ret; }

    //! checkSize
    if ( nCap < resInfo.size() )
    { return -2; }

    //! export
    for (int m = 0; m < resInfo.size(); m++)
    {
        //! p:rad
        memcpy( outRes + m * 9, resInfo.at(m).P, 4 * sizeof(double) );

        //! rad to deg
        //! [0~3)
        radToDeg(&outRes[m * 9], 4, &outRes[m * 9]);

        //! compensation
        vectorAdd(&outRes[m * 9], rotAngles, 4, &outRes[m * 9]);

        //! v
        memcpy( outRes + m * 9 + 4, resInfo.at(m).V, 4 * sizeof(double) );
        //! v rad to deg
        //! [4~7)
        radToDeg(&outRes[m * 9 + 4], 4, &outRes[m * 9 + 4] );

        //! t
        outRes[m * 9 + 8] = resInfo[m].T;
    }

    return 0;
}
