#ifndef AXISPAGE_H
#define AXISPAGE_H

#include "valuecombobox.h"

//namespace Ui {
//class AxisPage;
//}

class AxisPage : public ValueComboBox
{
    Q_OBJECT

public:
    explicit AxisPage(QWidget *parent = 0);
    ~AxisPage();

    void setPage( int page );
    int page();

    void setMaxPage( int maxPage );
private:
//    Ui::AxisPage *ui;
    QStringList mPageList;
};

#endif // AXISPAGE_H
