#ifndef LOGOUT_H
#define LOGOUT_H

#include <QtWidgets>

namespace Ui {
class logOut;
}

class logOut : public QWidget
{
    Q_OBJECT

public:
    explicit logOut(QWidget *parent = 0);
    ~logOut();

protected:
    virtual QSize sizeHint() const;
    virtual void changeEvent(QEvent *);

Q_SIGNALS:
    void sig_log_item( const QString &str );

protected Q_SLOTS:
    void on_btnExport_clicked();
    void slot_log_item( const QString &str );


public:
    void logIn( const QString &str );

private slots:
    void on_btnHide_toggled(bool checked);

private:
    Ui::logOut *ui;
    QMutex mMutex;
};

#endif // LOGOUT_H
