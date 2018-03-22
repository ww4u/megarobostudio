#ifndef ANGLEWIDGET_H
#define ANGLEWIDGET_H

#include <QWidget>

namespace Ui {
class AngleWidget;
}

class AngleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AngleWidget(QWidget *parent = 0);
    ~AngleWidget();

public:
    void setConnections( const QStringList &conns );

    QString connection();

    void setRange( int mi, int ma );

    void setValue( float val );
    float value();

Q_SIGNALS:
    void signal_close( QWidget * );

private slots:
    void on_btnClose_clicked();

private:
    Ui::AngleWidget *ui;
};

#endif // ANGLEWIDGET_H
