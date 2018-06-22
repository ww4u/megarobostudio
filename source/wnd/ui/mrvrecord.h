#ifndef MRVRECORD_H
#define MRVRECORD_H

#include <QWidget>

namespace Ui {
class MrvRecord;
}

class MrvRecord : public QWidget
{
    Q_OBJECT

public:
    explicit MrvRecord(QWidget *parent = 0);
    ~MrvRecord();

private:
    Ui::MrvRecord *ui;
};

#endif // MRVRECORD_H
