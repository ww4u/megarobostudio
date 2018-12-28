#include "robograph.h"

bool RoboGraph::diagramCompile()
{
    bool bRet;

    bRet = diagramCheck();
    if( bRet )
    {}
    else
    { return false; }

    bRet = diagramGen();
    if( bRet )
    {}
    else
    { return false; }

    return true;
}

bool RoboGraph::diagramCheck()
{
    return false;
}
bool RoboGraph::diagramGen()
{
    return false;
}
