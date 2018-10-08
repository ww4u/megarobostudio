#ifndef PROGRESSGROUP_H
#define PROGRESSGROUP_H

#include <QDialog>

#include "progresselement.h"

namespace Ui {
class ProgressGroup;
}

//class ProgressGroup : public QDialog
class ProgressGroup : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressGroup(QWidget *parent = 0);
    ~ProgressGroup();

Q_SIGNALS:
    void sigCancel( const QString &name, int id );

public:
    void progressInfo( const QString &name, int id, const QString &str );
    void progressProg( const QString &name, int id, int n );
    void progressRange( const QString &name,int id, int a, int b );

    void progressShow( const QString &name, int id, bool b=true );

protected:
    ProgressElement * progress( const QString &name, int id );

private:
    Ui::ProgressGroup *ui;

    QList<ProgressElement*> *m_pProgList;
};

#endif // PROGRESSGROUP_H
