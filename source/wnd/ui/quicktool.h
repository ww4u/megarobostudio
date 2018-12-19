#ifndef QUICKTOOL_H
#define QUICKTOOL_H

#include <QWidget>

namespace Ui {
class QuickTool;
}

class QuickTool : public QWidget
{
    Q_OBJECT

public:
    explicit QuickTool(QWidget *parent = 0);
    ~QuickTool();

protected:
    virtual void changeEvent( QEvent *event );

private:
    Ui::QuickTool *ui;
};

#endif // QUICKTOOL_H
