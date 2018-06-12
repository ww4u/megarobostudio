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

private:
    Ui::QuickTool *ui;
};

#endif // QUICKTOOL_H
