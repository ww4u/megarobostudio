#ifndef CFGTAB_H
#define CFGTAB_H

#include <QtWidgets>

namespace Ui {
class cfgTab;
}

class cfgTab : public QTabWidget
{
    Q_OBJECT

public:
    explicit cfgTab(QWidget *parent = 0);
    ~cfgTab();

private:
    Ui::cfgTab *ui;
};

#endif // CFGTAB_H
