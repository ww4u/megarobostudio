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

Q_SIGNALS:
    void signal_data_changed();

protected:
    virtual void changeEvent( QEvent *event );

public:
    void setRoboNames( const QStringList &strList );
    void setCurrentName( const QString &str );
    void setCurrentName( const int index );

//    int page();

    int roboPage();
    QString roboName();

public:
    QComboBox *getCombName();
    QComboBox *getCombPage();

private slots:


private:
    Ui::roboConnection *ui;
};

#endif // ROBOCONNECTION_H
