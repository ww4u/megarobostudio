#ifndef PORTOPTION_H
#define PORTOPTION_H

#include <QWidget>
#include "rsrcsearchthread.h"

namespace Ui {
class PortOption;
}

class PortOption : public QWidget
{
    Q_OBJECT

public:
    explicit PortOption(QWidget *parent = 0);
    ~PortOption();

protected:
    virtual void changeEvent( QEvent *event );

protected Q_SLOTS:
    void slot_search_finished();

public:
    void setPortType( int tpe, const QString &rsrcPatte="" );
    int portType();

    void setValidateEnable( bool b );
    bool validateEnable();

    void setIndicatorAble( bool b );
    bool indicatorAble();

    void setCurrentRsrc( const QString &rsrc );
    QString currentRsrc();

    void setRsrcs( const QStringList &rsrcs );
    QStringList rsrcs();

    void setTmo( int tmoms );
    int tmo();

    void searchRsrc( const QString &rsrcPattern,
                     QStringList &rsrcList );
    void searchOption( const QString &rsrcPattern );

protected:
    bool validateVisaRsrc( QString &strIdn );

private slots:
    void on_btnVerify_2_clicked();

    void on_btnAddT_clicked();

    void on_btnRemove_clicked();

    void on_btnClear_clicked();

    void on_cmbRsrcList_editTextChanged(const QString &arg1);

    void on_listMRTs_currentRowChanged(int currentRow);

    void on_btnScan_clicked();

    void on_tolIndicator_clicked(bool checked);

private:
    Ui::PortOption *ui;

    bool mbVerifyAble;
    bool mbIndicatAble;

    int mPortType;
    QString mRsrcPatt;

    RsrcSearchThread mSearchThread;
};

#endif // PORTOPTION_H
