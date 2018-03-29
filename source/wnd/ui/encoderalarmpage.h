#ifndef ENCODERALARMPAGE_H
#define ENCODERALARMPAGE_H

#include <QWidget>

namespace Ui {
class EncoderAlarmPage;
}

class EncoderAlarmPage : public QWidget
{
    Q_OBJECT

public:
    explicit EncoderAlarmPage(QWidget *parent = 0);
    ~EncoderAlarmPage();

private:
    Ui::EncoderAlarmPage *ui;
};

#endif // ENCODERALARMPAGE_H
