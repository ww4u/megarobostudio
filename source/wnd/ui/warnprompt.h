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

protected:
    virtual void changeEvent( QEvent *event );

public:
    void setInfo( const QString &str );
    void addInfo( const QString &str );

protected:
    virtual void hideEvent(QHideEvent *event);

private:
    Ui::WarnPrompt *ui;

    int mCapcity;
};

#endif // WARNPROMPT_H
