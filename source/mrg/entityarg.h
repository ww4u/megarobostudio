#ifndef ENTITYARG_H
#define ENTITYARG_H

#include <QtCore>

class Arg_AxPageFile
{
public:
    int ax, page;
    QString file;

public:
    Arg_AxPageFile()
    {
        ax = 0;
        page = 0;
    }
};

class Arg_AxPage
{
public:
    int ax, page;
public:
    Arg_AxPage()
    {
        ax = 0;
        page = 0;
    }
};


#endif // ENTITYARG_H
