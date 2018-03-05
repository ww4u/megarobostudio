#ifndef WARNPROMPT_H
#define WARNPROMPT_H

#include <QDialog>

namespace Ui {
class WarnPrompt;
}

class WarnPrompt : public QDialog
{
    Q_OBJECT

public:
    explicit WarnPrompt(QWidget *parent = 0);
    ~WarnPrompt();

    void setInfo( const QString &str );

private:
    Ui::WarnPrompt *ui;
};

#endif // WARNPROMPT_H
