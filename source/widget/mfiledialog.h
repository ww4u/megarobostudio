#ifndef MFILEDIALOG_H
#define MFILEDIALOG_H

#include <QFileDialog>

namespace Ui {
class MFileDialog;
}

//class MFileDialog : public QFileDialog
class MFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MFileDialog();
    ~MFileDialog();
private:
    Ui::MFileDialog *ui;
};

#endif // MFILEDIALOG_H
