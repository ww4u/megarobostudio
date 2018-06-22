#ifndef COMTHREAD_H
#define COMTHREAD_H

#include <QThread>
#include <QSerialPort>
#include <QMutex>

class ComThread : public QThread
{
    Q_OBJECT
public:
    explicit ComThread(QObject *parent = nullptr);

protected:
    virtual void run();

public:
    void setPort( const QString &str );
    QString port();

signals:
    void signal_receive( const QString &str );

public slots:
    void slot_transmit( const QByteArray &ary );

protected:
    QSerialPort mPort;
    QString mPortName;


};

#endif // COMTHREAD_H
