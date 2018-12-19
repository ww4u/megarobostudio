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

protected:
    virtual void changeEvent( QEvent *event );

public:
    void setModel( mcModel *pModle );
    void setConnections( const QStringList &conn );

    void setDataId( int id );

    void setDataIds( QMap<int, QString> &subItem );

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

    void on_comboBox_activated(int index);

private:
    Ui::AngleMonitor *ui;

    QList< QWidget *> mAngleWidgets;
    QTimer mTimer;

    mcModel *m_pModel;
    QStringList mConns;

    int mDataId;
};

#endif // ANGLEMONITOR_H
