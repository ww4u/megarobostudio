#ifndef DELTAPREF_H
#define DELTAPREF_H

#include <QtWidgets>

#include "modelview.h"

namespace Ui {
class deltapref;
}

class DeltaPref : public modelView
{
    Q_OBJECT

public:
    explicit DeltaPref(QWidget *parent = 0);
    ~DeltaPref();

private:
    Ui::deltapref *ui;
};

#endif // DELTAPREF_H
