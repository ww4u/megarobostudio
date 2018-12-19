#ifndef EMERGESTOP_H
#define EMERGESTOP_H

#include <QWidget>

namespace Ui {
class EmergeStop;
}

class EmergeStop : public QWidget
{
    Q_OBJECT

public:
    explicit EmergeStop(QWidget *parent = 0);
    ~EmergeStop();

private:
    Ui::EmergeStop *ui;
};

#endif // EMERGESTOP_H
