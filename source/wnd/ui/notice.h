#ifndef NOTICE_H
#define NOTICE_H

#include <QtWidgets>


namespace Ui {
class Notice;
}

class Notice : public QDialog
{
    Q_OBJECT

public:
    explicit Notice(QWidget *parent = 0);
    ~Notice();

    bool load( QString &file );

    bool nextShowAble();

private:
    Ui::Notice *ui;
};

#endif // NOTICE_H
