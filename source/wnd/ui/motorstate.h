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

    virtual QSize sizeHint() const;

protected:
    virtual void changeEvent( QEvent *event );

public:
    void setName( const QString &name );
    QString name();

    void setState( const QString &stat,
                   int page = 0 );
    QString state( int page = 0 );

    void setProgress( int page, int mi, int ma, int n );
    void setProgress( int page, bool b );

private slots:
    void on_cmbRegion_activated(int index);

private:
    Ui::MotorState *ui;

    QMap<int,QString> mStates;

    QList< QTime *> mRunningTimes;
    QList< bool > mTimerRunnings;

    int mLastProg;
};

#endif // MOTORSTATE_H
