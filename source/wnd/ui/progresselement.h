#ifndef PROGRESSELEMENT_H
#define PROGRESSELEMENT_H

#include <QtWidgets>

namespace Ui {
class ProgressElement;
}

class ProgressElement : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressElement(QWidget *parent = 0);
    ~ProgressElement();

protected:
    virtual void hideEvent(QHideEvent *event );

public:
    virtual void setVisible( bool b );

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

    bool progress( int &max, int &min, int &now );

private slots:
    void on_btnCancel_clicked();

private:
    Ui::ProgressElement *ui;

    bool mbStateVisible;

    QString mName;
    int mId;

    QTime mLatestTime;
    int mLatestProg;

};

#endif // PROGRESSELEMENT_H
