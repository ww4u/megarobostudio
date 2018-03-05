#ifndef RUNTOOL_H
#define RUNTOOL_H

#include <QtWidgets>

namespace Ui {
class runTool;
}

class runTool : public QWidget
{
    Q_OBJECT

public:
    explicit runTool(QWidget *parent = 0);
    ~runTool();

Q_SIGNALS:
    void sigForceStop();

private slots:
    void on_toolStop_clicked();

private:
    Ui::runTool *ui;
};

#endif // RUNTOOL_H
