#ifndef PROGRESSELEMENT_H
#define PROGRESSELEMENT_H

#include <QWidget>

namespace Ui {
class ProgressElement;
}

class ProgressElement : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressElement(QWidget *parent = 0);
    ~ProgressElement();

Q_SIGNALS:
    void sigCancel( const QString &name, int id );


public:
    void set( const QString &name, int id );

    QString name();
    int Id();

    bool match( const QString &name, int id );

    void progressInfo( const QString &str );
    void progressProg( int n );
    void progressRange( int a, int b );

private slots:
    void on_btnCancel_clicked();

private:
    Ui::ProgressElement *ui;

    QString mName;
    int mId;

};

#endif // PROGRESSELEMENT_H
