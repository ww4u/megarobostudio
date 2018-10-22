#ifndef APPPROCESS_H
#define APPPROCESS_H

#include <QProcess>

class AppProcess : public QProcess
{
    Q_OBJECT
public:
    AppProcess( QObject *parent = Q_NULLPTR );

Q_SIGNALS:
    void signal_finished( QProcess *pObj, int exitCode, QProcess::ExitStatus exitStatus );
    void signal_finished( QProcess *pObj, QProcess::ProcessError error);

    void signal_readyReadStandardError( QProcess *pObj );
    void signal_readyReadStandardOutput( QProcess *pObj );

    void signal_errorOccurred( QProcess *pObj, QProcess::ProcessError error);


protected slots:
    void slot_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void slot_errorOccurred(QProcess::ProcessError error);

    void slot_readyReadStandardError();
    void slot_readyReadStandardOutput();

};

#endif // APPPROCESS_H
