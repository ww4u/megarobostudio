#ifndef MRQINFO_H
#define MRQINFO_H

#include <QWidget>
#include <QTableWidgetItem>

#include "mrqview.h"

namespace Ui {
class mrqInfo;
}

class mrqInfo : public mrqView
{
    Q_OBJECT

public:
    explicit mrqInfo(QWidget *parent = 0);
    ~mrqInfo();    

protected:
    void buildConnection();

    void initModel();
    void deinitModel();

public:
    void updateUi();

protected:
    virtual void modelChanged();

private:
    Ui::mrqInfo *ui;

protected:
    QList< QTableWidgetItem * > mTableItems;

};

#endif // MRQINFO_H
