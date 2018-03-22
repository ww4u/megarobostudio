#ifndef MOTORSTATE_H
#define MOTORSTATE_H

#include <QWidget>

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

    void setState( const QString &stat );
    QString state();

    void setProgress( int mi, int ma, int n );
    void setProgress( bool b );

private:
    Ui::MotorState *ui;
};

#endif // MOTORSTATE_H
