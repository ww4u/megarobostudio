
#include <QApplication>
#include <QDebug>

#include "../../include/mcstd.h"

#include "../../robot/megatron/megatron.h"

int main(int argc, char *argv[] )
{
    QApplication app( argc, argv );

//    kinematic_CrossTest();

//    kinematicFullTest();

    robotMegatron robo;

    robo.moveTest();

    return 0;
}
