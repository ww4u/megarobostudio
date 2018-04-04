#include "megamessagebox.h"

MegaMessageBox::MegaMessageBox( QWidget *parent ) : QMessageBox( parent )
{

}

MegaMessageBox::MegaMessageBox( const QString &info,
                QWidget *parent ) : QMessageBox( parent )
{
    setText( info );
//    setWindowTitle( tr("OK?") );
    setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel );
    setDefaultButton(QMessageBox::Ok);
}
