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
public:
    void setDeviceNames( const QStringList &strList );
    void setCurrentName( const QString &str );
    void setCurrentName( const int index );

    void setDeviceCHs( const QStringList &strList );
    void setCurrentCH( const QString &str );
    void setCurrentCH( const int index );

public:
    QComboBox *getCombName();
    QComboBox *getCombCH();

private:
    Ui::axesConnection *ui;
};

#endif // AXESCONNECTION_H
