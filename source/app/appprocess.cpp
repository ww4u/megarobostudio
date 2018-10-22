#include "appprocess.h"

AppProcess::AppProcess( QObject *parent ) : QProcess( parent )
{
    connect( this, SIGNAL(finished(int,QProcess::ExitStatus)),
             this, SLOT(slot_finished(int,QProcess::ExitStatus)));
    connect( this, SIGNAL(error(QProcess::ProcessError)),
             this, SLOT(slot_errorOccurred(QProcess::ProcessError)));

    connect( this, SIGNAL(readyReadStandardOutput()),
             this, SLOT(slot_readyReadStandardOutput()));
    connect( this, SIGNAL(readyReadStandardError()),
             this, SLOT(slot_readyReadStandardError()) );
}

void AppProcess::slot_finished(int exitCode, QProcess::ExitStatus exitStatus)
{ emit signal_finished( this, exitCode, exitStatus ); }
void AppProcess::slot_errorOccurred(QProcess::ProcessError error)
{ emit signal_finished( this, error ); }

void AppProcess::slot_readyReadStandardError()
{ emit signal_readyReadStandardError(this); }
void AppProcess::slot_readyReadStandardOutput()
{ emit signal_readyReadStandardOutput(this); }

