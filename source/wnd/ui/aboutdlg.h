#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>
#include <QKeyEvent>
#include "../../source/para/syspara.h"
namespace Ui {
class aboutDlg;
}

class aboutDlg : public QDialog
{
    Q_OBJECT

public:
    explicit aboutDlg( SysPara *pPara,
                       QWidget *parent = 0);
    ~aboutDlg();

protected:
    virtual void keyReleaseEvent(QKeyEvent *event );

private:
    Ui::aboutDlg *ui;

    SysPara *m_pPara;
};

#endif // ABOUTDLG_H
