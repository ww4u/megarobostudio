#ifndef QUERYVIEW_H
#define QUERYVIEW_H

#include <QtWidgets>

namespace Ui {
class queryView;
}

class queryView : public QDialog
{
    Q_OBJECT

public:
    explicit queryView(QWidget *parent = 0);
    ~queryView();

protected:
    virtual void changeEvent( QEvent *event );

public:
    void setModel( QAbstractItemModel *pModel,
                   const QString &name="",
                   const QString &comment="" );

private slots:
    void on_btnExport_clicked();

    void on_cmbViewFmt_currentIndexChanged(int index);

private:
    Ui::queryView *ui;
};

#endif // QUERYVIEW_H
