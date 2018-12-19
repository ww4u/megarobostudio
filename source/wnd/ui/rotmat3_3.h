#ifndef ROTMAT3_3_H
#define ROTMAT3_3_H

#include <QWidget>

namespace Ui {
class RotMat3_3;
}

class RotMat3_3 : public QWidget
{
    Q_OBJECT

public:
    explicit RotMat3_3(QWidget *parent = 0);
    ~RotMat3_3();

protected:
    virtual void changeEvent( QEvent *event );

public:
    void setRotateMatrix( double rot[3*3] );
    void roateMatrix( double rot[3*3] );

private:
    Ui::RotMat3_3 *ui;
};

#endif // ROTMAT3_3_H
