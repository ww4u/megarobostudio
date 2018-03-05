#ifndef DLGWNDLIST_H
#define DLGWNDLIST_H

#include <QtWidgets>

namespace Ui {
class dlgWndList;
}

class dlgWndList : public QDialog
{
    Q_OBJECT

public:
    explicit dlgWndList(QWidget *parent = 0);
    ~dlgWndList();

public:
    void addWnd( const QString &str );
    void addWnd( const QStringList &strList );

    void clear();
Q_SIGNALS:
    void signal_activeWnd( QString str );
    void signal_closeAll();

private slots:
    void on_btnCloseAll_clicked();

    void on_btnActivate_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_itemSelectionChanged();

private:
    Ui::dlgWndList *ui;
};

#endif // DLGWNDLIST_H
