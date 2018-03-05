#ifndef FILETOOL_H
#define FILETOOL_H

#include <QtWidgets>

namespace Ui {
class fileTool;
}

class fileTool : public QToolBar
{
    Q_OBJECT

public:
    explicit fileTool(QWidget *parent = 0);
    ~fileTool();

private:
    Ui::fileTool *ui;
};

#endif // FILETOOL_H
