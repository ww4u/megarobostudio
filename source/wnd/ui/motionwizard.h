#ifndef MOTIONWIZARD_H
#define MOTIONWIZARD_H

#include <QtWidgets>

namespace Ui {
class MotionWizard;
}

class MotionWizard : public QDialog
{
    Q_OBJECT

public:
    explicit MotionWizard(QWidget *parent = 0);
    ~MotionWizard();

protected:
    virtual void changeEvent( QEvent *event );

public:
    QString motionName();


private slots:
    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MotionWizard *ui;
};

#endif // MOTIONWIZARD_H
