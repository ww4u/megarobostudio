#ifndef MRVSYSTEM_H
#define MRVSYSTEM_H

#include <QWidget>
#include "mrvview.h"

namespace Ui {
class MrvSystem;
}

class MrvSystem : public mrvView
{
    Q_OBJECT

public:
    explicit MrvSystem(QWidget *parent = 0);
    ~MrvSystem();

public:
    virtual int setApply();
public:
    virtual void modelChanged();

public:
    QString deviceName();

protected:
    void spyEdited();

    int apply();
    int updateUi();

private:
    Ui::MrvSystem *ui;
};

#endif // MRVSYSTEM_H
