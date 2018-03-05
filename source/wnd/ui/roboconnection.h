#ifndef ROBOCONNECTION_H
#define ROBOCONNECTION_H

#include <QtWidgets>

namespace Ui {
class roboConnection;
}

class roboConnection : public QWidget
{
    Q_OBJECT

public:
    explicit roboConnection(QWidget *parent = 0);
    ~roboConnection();

public:
    void setRoboNames( const QStringList &strList );
    void setCurrentName( const QString &str );
    void setCurrentName( const int index );

public:
    QComboBox *getCombName();

private slots:


private:
    Ui::roboConnection *ui;
};

#endif // ROBOCONNECTION_H
