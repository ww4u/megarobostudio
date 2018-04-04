#ifndef MEGAMESSAGEBOX_H
#define MEGAMESSAGEBOX_H

#include <QMessageBox>

class MegaMessageBox : public QMessageBox
{
public:
    MegaMessageBox( QWidget *parent = Q_NULLPTR );

    MegaMessageBox( const QString &info,
                    QWidget *parent = Q_NULLPTR );

};

#endif // MEGAMESSAGEBOX_H
