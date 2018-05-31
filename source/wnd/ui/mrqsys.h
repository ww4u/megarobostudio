#ifndef MRQSYS_H
#define MRQSYS_H

#include <QWidget>
#include "mrqview.h"

namespace Ui {
class mrqSys;
}

class mrqSys : public mrqView
{
    Q_OBJECT

public:
    explicit mrqSys(QWidget *parent = 0);
    ~mrqSys();

public:
    virtual int setApply();
public:
    virtual void modelChanged();

protected:
    void spyEdited();
    void setupUi();
    void desetupUi();

    int apply();
    int updateUi();

    void adaptUi();

private slots:
    void on_chkLed_clicked(bool checked);

    void on_edtAlias_textEdited(const QString &arg1);

private:
    Ui::mrqSys *ui;
};

#endif // MRQSYS_H
