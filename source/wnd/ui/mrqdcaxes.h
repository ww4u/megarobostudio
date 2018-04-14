#ifndef MRQDCAXES_H
#define MRQDCAXES_H

#include <mrqview.h>

namespace Ui {
class MrqDcAxes;
}

class MrqDcAxes : public mrqView
{
    Q_OBJECT

public:
    explicit MrqDcAxes(QWidget *parent = 0);
    ~MrqDcAxes();

protected:
    void spyEdited();

private:
    Ui::MrqDcAxes *ui;
};

#endif // MRQDCAXES_H
