#include "megamessagebox.h"
#include "../../source/sys/sysapi.h"

bool MegaMessageBox::_bZeroAffirm = false;

void MegaMessageBox::setZeroAffirm( bool b)
{ MegaMessageBox::_bZeroAffirm = b; }
bool MegaMessageBox::zeroAffirm()
{ return MegaMessageBox::_bZeroAffirm; }

MegaMessageBox::MegaMessageBox( QWidget *parent ) : QMessageBox( parent )
{

}

MegaMessageBox::MegaMessageBox( const QString &info,
                QWidget *parent ) : QMessageBox( parent )
{
    setText( info );

    setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel );
    setDefaultButton(QMessageBox::Ok);
}

MegaZeroAffirmMessageBox::MegaZeroAffirmMessageBox( QWidget *parent ) : MegaMessageBox( parent )
{
    setText( tr("Affirm to zero?") );
    setWindowTitle( tr("Affirm") );
    setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel );
    setDefaultButton(QMessageBox::Ok);
}

int MegaZeroAffirmMessageBox::exec()
{
    if ( MegaMessageBox::_bZeroAffirm )
    {
        return MegaMessageBox::exec();
    }
    else
    {
        sysLog( tr("Auto zero affirm") );
        return QMessageBox::Ok;
    }
}

MegaDeleteAffirmMessageBox::MegaDeleteAffirmMessageBox( QWidget *parent ) : MegaMessageBox( parent )
{
    setText( tr("Affirm to delete?") );
    setWindowTitle( tr("Affirm") );
    setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel );
    setDefaultButton(QMessageBox::Ok);
}
