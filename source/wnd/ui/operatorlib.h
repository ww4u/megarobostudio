#ifndef OPERATORLIB_H
#define OPERATORLIB_H

#include <QDialog>

namespace Ui {
class OperatorLib;
}

class OperatorLib : public QDialog
{
    Q_OBJECT

public:
    explicit OperatorLib(QWidget *parent = 0);
    ~OperatorLib();

public:
    int loadLib( const QString &name );

private:
    Ui::OperatorLib *ui;
};

#endif // OPERATORLIB_H
