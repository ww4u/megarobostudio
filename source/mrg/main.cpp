#include <QCoreApplication>

#include "mrxt4.h"
#include "link.h"

int main()
{
    MrxT4 t41, t42;
    Link l1;

    t41.attachSuccessor( &l1 );
    l1.attachSuccessor( &t42 );

    t41.traverse();

    return 0;
}
