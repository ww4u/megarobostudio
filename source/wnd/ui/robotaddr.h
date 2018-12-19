#ifndef ROBOTADDR_H
#define ROBOTADDR_H

#include <QWidget>

namespace Ui {
class RobotAddr;
}

class RobotAddr : public QWidget
{
    Q_OBJECT

public:
    explicit RobotAddr(QWidget *parent = 0);
    ~RobotAddr();

protected:
    virtual void changeEvent( QEvent *event );

public:
    void setAlias( QString &str );
    QString alias();

    void setGroupSel( int gp );
    int groupSel();

    void setGroupId( int id );
    int groupId();

    void setSubGroupIndex( int id );
    int subGroupIndex();

    void setSubGroupId( int id );
    int subGroupId();

private:
    Ui::RobotAddr *ui;
};

#endif // ROBOTADDR_H
