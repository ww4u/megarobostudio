#ifndef ADDPOINT_H
#define ADDPOINT_H
// 常量及变量定义
#define error  1e-4
#define PI 3.1415926
#define E 2.7182818
void GetNewPvt(double* pIn, double*vIn, double*tIn, int length, int mode, double* pOut, double* vOut, double*tOut, int* pLen);
void CalZcp(double* p, double* v, double* t, double* scaIn, int index, int jointCount,int pvtCount, double* pOut, double* vOut, double* tOut);
void Rank(double* input, int len);
void DeleteRedundantPvt(double* inputP, double* inputV, double* inputT, int len, int joinCount, double* pOut, double* vOut, double* tOut,int * outLen);
#endif
