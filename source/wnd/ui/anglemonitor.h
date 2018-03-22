#ifndef ANGLEMONITOR_H
#define ANGLEMONITOR_H

#include <QDialog>
#include <QtWidgets>

#include "../../model/mcmodel.h"

namespace Ui {
class AngleMonitor;
}

class AngleMonitor : public QDialog
{
    Q_OBJECT

public:
    explicit AngleMonitor(QWidget *parent = 0);
    ~AngleMonitor();

public:
    void setModel( mcModel *pModle );
    void setConnections( const QStringList &conn );

    void setDataId( int id );
    void setRange( int mi, int ma );

protected:
    void addAnAngle();

    void sampleProc();

    float sampleUnit( const QString &conn,
                      int &err );

Q_SIGNALS:
    void sig_anglewidget_changed();

private slots:
    void on_btnAdd_clicked();

    void slot_anglewidget_close( QWidget * pWig );

    void slot_timeout();

    void on_spinInterval_valueChanged(int arg1);

    void on_checkBox_clicked(bool checked);

    void on_toolButton_clicked();

    void slot_anglewidget_changed();

private:
    Ui::AngleMonitor *ui;

    QList< QWidget *> mAngleWidgets;
    QTimer mTimer;

    mcModel *m_pModel;
    QStringList mConns;

    int mDataId;
    int mMin, mMax;
};

#endif // ANGLEMONITOR_H
