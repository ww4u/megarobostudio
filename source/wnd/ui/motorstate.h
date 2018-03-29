#ifndef MOTORSTATE_H
#define MOTORSTATE_H

#include <QtWidgets>

namespace Ui {
class MotorState;
}

class MotorState : public QWidget
{
    Q_OBJECT

public:
    explicit MotorState(QWidget *parent = 0);
    ~MotorState();

    void setName( const QString &name );
    QString name();

    void setState( const QString &stat,
                   int page = 0 );
    QString state( int page = 0 );

    void setProgress( int mi, int ma, int n );
    void setProgress( bool b );

private slots:
    void on_cmbRegion_activated(int index);

private:
    Ui::MotorState *ui;

    QMap<int,QString> mStates;
};

#endif // MOTORSTATE_H
