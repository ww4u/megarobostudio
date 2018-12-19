#ifndef ROBOSKETCH_H
#define ROBOSKETCH_H

#include <QWidget>

#include <QtWidgets>
#include "modelview.h"

namespace Ui {
class RoboSketch;
}

class RoboSketch : public modelView
{
    Q_OBJECT

public:
    explicit RoboSketch(QWidget *parent = 0);
    ~RoboSketch();

protected:
    virtual void changeEvent( QEvent *event );

public:
    void setSketch( const QString &rsrc );

private:
    Ui::RoboSketch *ui;
};

#endif // ROBOSKETCH_H
