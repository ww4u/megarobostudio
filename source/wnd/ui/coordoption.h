#ifndef COORDOPTION_H
#define COORDOPTION_H

#include <QtWidgets>

namespace Ui {
class CoordOption;
}

class CoordOption : public QWidget
{
    Q_OBJECT

public:
    explicit CoordOption(QWidget *parent = 0);
    ~CoordOption();

protected:
    virtual void changeEvent( QEvent *event );

public:
    void setOption( int index );
    int option();

Q_SIGNALS:
    void signal_option_changed( int index );

protected slots:
    void slot_option_changed();

private:
    Ui::CoordOption *ui;

    QList< QRadioButton *> _buttons;
};

#endif // COORDOPTION_H
