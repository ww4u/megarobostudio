
#include <QApplication>
#include <QDebug>

#include "../../include/mcstd.h"

#include "../../arith/delta/dllMain.h"

int main(int argc, char *argv[] )
{
    QApplication app( argc, argv );


    delta_test();

    return 0;
}
