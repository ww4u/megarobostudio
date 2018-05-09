#ifndef STATEDIAGRAM_H
#define STATEDIAGRAM_H

#include <QDialog>

namespace Ui {
class StateDiagram;
}

class StateDiagram : public QDialog
{
    Q_OBJECT

public:
    explicit StateDiagram(QWidget *parent = 0);
    ~StateDiagram();

private:
    Ui::StateDiagram *ui;
};

#endif // STATEDIAGRAM_H
