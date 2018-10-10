#include "mprogressbar.h"

MProgressBar::MProgressBar( QWidget *parent ) : QProgressBar( parent )
{

}

void MProgressBar::mouseReleaseEvent( QMouseEvent *event )
{
    emit signal_clicked();
}
