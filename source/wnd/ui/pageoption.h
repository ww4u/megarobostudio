#ifndef PAGEOPTION_H
#define PAGEOPTION_H

#include <QtWidgets>

namespace Ui {
class PageOption;
}

class PageOption : public QWidget
{
    Q_OBJECT

public:
    explicit PageOption(QWidget *parent = 0);
    ~PageOption();

public:
    QComboBox *getComb();

    void setPage( int pageIndex );
    int page();

private:
    Ui::PageOption *ui;
};

#endif // PAGEOPTION_H
