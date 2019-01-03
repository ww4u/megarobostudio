#ifndef AXESCONNECTION_H
#define AXESCONNECTION_H

#include <QtWidgets>

namespace Ui {
class axesConnection;
}

class axesConnection : public QWidget
{
    Q_OBJECT

public:
    explicit axesConnection(QWidget *parent = 0);
    ~axesConnection();

protected:
    virtual void changeEvent( QEvent *event );

public:
//    void setDeviceNames( const QStringList &strList );
    void setDeviceNames( const QMap<QString,int> &map );
    void setCurrentName( const QString &str );
    void setCurrentName( const int index );

    void setDeviceCHs( const QStringList &strList );
    void setCurrentCH( const QString &str );
    void setCurrentCH( const int index );

    int page();

public:
    QComboBox *getCombName();
    QComboBox *getCombCH();
    QComboBox *getCombPage();

private slots:

    void on_cmbDeviceName_currentIndexChanged(int index);

private:
    Ui::axesConnection *ui;
};

#endif // AXESCONNECTION_H
