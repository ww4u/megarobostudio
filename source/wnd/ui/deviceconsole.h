#ifndef DEVICECONSOLE_H
#define DEVICECONSOLE_H

#include <QtWidgets>

#include "../../scpishell/scpishell.h"

namespace Ui {
class deviceConsole;
}

class deviceConsole : public QDialog
{
    Q_OBJECT

public:
    explicit deviceConsole(QWidget *parent = 0);
    ~deviceConsole();

    void setShell( scpiShell *pShell );
    scpiShell *getShell();

    void setMrq( bool b );

    void setClassName( const QString &cls );
    QString className();

Q_SIGNALS:
    void signal_classname_changed();

protected Q_SLOTS:
    void on_btnWrite_clicked();
    void on_btnRead_clicked();
    void slot_device_changed();
    void slot_classname_changed();

private slots:
    void on_comboBox_editTextChanged(const QString &arg1);

    void on_btnRst_clicked();
    void on_btnCls_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_btnCmdSet_clicked();

protected:
    void doWrite( const QString &str );

private:
    Ui::deviceConsole *ui;

    scpiShell *m_pScpiShell;
    QString mClassName;
    QString mCommandsetFullName;
};

#endif // DEVICECONSOLE_H
