#ifndef DLGAPP_H
#define DLGAPP_H

#include <QDialog>

namespace Ui {
class DlgApp;
}

class DlgApp : public QDialog
{
    Q_OBJECT

public:
    explicit DlgApp(QWidget *parent = 0);
    ~DlgApp();

private:
    Ui::DlgApp *ui;
};

#endif // DLGAPP_H
