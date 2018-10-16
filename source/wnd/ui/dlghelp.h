#ifndef DLGHELP_H
#define DLGHELP_H

#include <QDialog>

namespace Ui {
class DlgHelp;
}

class DlgHelp : public QDialog
{
    Q_OBJECT

public:
    explicit DlgHelp(QWidget *parent = 0);
    ~DlgHelp();

    void showHelp( const QString &info,
                   const QString &img );

    void setShowAgain( bool b );
    bool showAgain();
private:
    Ui::DlgHelp *ui;
};

#endif // DLGHELP_H
