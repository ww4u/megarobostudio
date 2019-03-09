#ifndef TPEDIT_H
#define TPEDIT_H

#include <QWidget>

#include "tableedit.h"
#include "../model/tpgroup.h"
#include "../model/dspindelegate.h"

#include "tpvplot.h"

namespace Ui {
class TpEdit;
}

class TpEdit : public tableEdit
{
    Q_OBJECT

public:
    explicit TpEdit(QWidget *parent = 0);
    ~TpEdit();

protected:
    virtual void changeEvent( QEvent *event );

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int save( QString &outFileName );
    virtual int saveAs( QString &outFileName );

protected:
    virtual void context_remove();
    virtual void context_add_before();
    virtual void context_add_below();
    virtual void context_clear();

public:
    virtual void settingChanged( enumSetting setting, const QVariant &v );

protected:
    virtual void onMcModelUpdated();

protected:
    virtual QString activeName();
    virtual int activeAxes();

    virtual void onNetEvent(const QString &name,
                            int axes,
                            RoboMsg &msg);

    void onMotionStatus( int axes, MRQ_MOTION_STATE_2 stat );

protected:
    bool checkChan( const QString &name,
                    int axesId );
    bool checkChan();

    MegaDevice::deviceMRV *currentDevice( int &ax );

    void gcLine();
    bool checkLine();
    int buildLine( );

    void updatePlot();

    int postDownload( appMsg msg, void *pPara );
    void beginDownload( void *pPara);
    void endDownload( int ret, void *pPara );

Q_SIGNALS:
    void sigLineChanged();

protected Q_SLOTS:
    void slot_data_changed();

    void slot_download_cancel();
    void slot_line_changed();

    void on_btnBuild_clicked();     //! build line

    void on_btnDown_clicked();
    void on_btnStart_clicked();
    void on_btnStop_clicked();

    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_btnClr_clicked();
    void on_btnGraph_clicked();

    void on_spinLoop_valueChanged(int arg1);

    void on_btnPref_clicked();

private slots:
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::TpEdit *ui;

    TpGroup *m_pTpGroup;
    dSpinDelegate *m_ptDelegate;

    tpvPlot *m_pPlot;

    QList<QPointF> mTPs;
};

#endif // TPEDIT_H
