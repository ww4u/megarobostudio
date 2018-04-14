#ifndef MRQIO_H
#define MRQIO_H

#include <QWidget>
#include "mrqview.h"

namespace Ui {
class mrqIo;
}

class mrqIo : public mrqView
{
    Q_OBJECT

public:
    explicit mrqIo(QWidget *parent = 0);
    ~mrqIo();

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

    int isoApply();
    int doApply();
    int aiApply();

private slots:
    void on_btnIsoApply_clicked();
    void on_btnDoApply_clicked();
    void on_btnAiApply_clicked();

private:
    Ui::mrqIo *ui;
};

#endif // MRQIO_H
