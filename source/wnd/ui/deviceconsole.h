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

protected Q_SLOTS:
    void on_btnWrite_clicked();
    void on_btnRead_clicked();

private slots:
    void on_comboBox_editTextChanged(const QString &arg1);

    void on_btnRst_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

protected:
    void doWrite( const QString &str );

private:
    Ui::deviceConsole *ui;

    scpiShell *m_pScpiShell;
};

#endif // DEVICECONSOLE_H
