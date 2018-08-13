#ifndef TABLETOOLBAR_H
#define TABLETOOLBAR_H

#include <QWidget>

namespace Ui {
class TableToolbar;
}

class TableToolbar : public QWidget
{
    Q_OBJECT

public:
    explicit TableToolbar(QWidget *parent = 0);
    ~TableToolbar();

Q_SIGNALS:
    void signal_add_clicked();
    void signal_del_clicked();
    void signal_clr_clicked();

public:
    void setAddEnable( bool b );
    void setDelEnable( bool b );
    void setclrEnable( bool b );

private:
    Ui::TableToolbar *ui;
};

#endif // TABLETOOLBAR_H
