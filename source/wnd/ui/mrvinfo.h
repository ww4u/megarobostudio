#ifndef MRVINFO_H
#define MRVINFO_H

#include <QWidget>
#include <QTableWidgetItem>

#include "mrvview.h"

namespace Ui {
class mrvInfo;
}

class mrvInfo : public mrvView
{
    Q_OBJECT

public:
    explicit mrvInfo(QWidget *parent = 0);
    ~mrvInfo();

protected:
    virtual void changeEvent( QEvent *event );

protected:
    void buildConnection();

    void initModel();
    void deinitModel();

    void translate();

public:
    void updateUi();

public:
    virtual void modelChanged();

private:
    Ui::mrvInfo *ui;

protected:
    QList< QTableWidgetItem * > mTableItems;

};

#endif // MRQINFO_H
