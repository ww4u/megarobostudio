#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class LogIn;
}

class LogIn : public QDialog
{
    Q_OBJECT

public:
    explicit LogIn(QWidget *parent = 0);
    ~LogIn();

public:
    int getUserRole();
    bool getAutoLogin();

private:
    Ui::LogIn *ui;
};

#endif // LOGIN_H
