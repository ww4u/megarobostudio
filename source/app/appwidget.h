#ifndef APPWIDGET_H
#define APPWIDGET_H

#include <QWidget>
#include "../../include/mcstd.h"
#include "../../com/robomsg.h"

enum appMsg
{
    msg_unk = 0,
    msg_exit,
    msg_search_device,
    msg_upload_device,
    msg_import_device,

    msg_download_pvt,
    msg_start_pvt,
    msg_stop_pvt,

    msg_download_pt,
    msg_start_pt,
    msg_stop_pt,

    msg_download_motion,
    msg_start_motion,
    msg_stop_motion,

    msg_mrq_property_apply,
    msg_mrv_property_apply,
    msg_robo_property_apply,

    msg_mrq_property_ok,
    msg_mrv_property_ok,
    msg_robo_property_ok,

    msg_robo_property_reset,

    msg_mgr_motor_test,
    msg_mgr_robo_test,
};

class appWidget : public QWidget
{
    Q_OBJECT
public:
    explicit appWidget(QWidget *parent = nullptr);
    virtual ~appWidget()
    {}

    typedef int (appWidget::*apiPostDo)( appMsg msg, void *pPara );
    typedef void(appWidget::*apiBeginDo)( void *pPara );
    typedef void(appWidget::*apiEndDo)( int ret, void *pPara );
    typedef void(appWidget::*apiProgDo)( int now, int from, int to, void *pPara );

    struct apiGroup
    {
        appWidget::apiPostDo  mPostDo;
        appWidget::apiBeginDo mBeginDo;
        appWidget::apiEndDo   mEndDo;
        appWidget::apiProgDo  mProgDo;
    };

    struct servContext
    {
        appWidget *m_pBase;
        appWidget::apiGroup mGroup;
        appMsg mMsg;
        void *m_pPara;
    };      

Q_SIGNALS:
    void signal_post_do( appWidget::servContext context );

protected Q_SLOTS:
    void slot_begin_do( appWidget::servContext context );
    void slot_end_do( appWidget::servContext context,
                      int ret );
    void slot_prog_do( appWidget::servContext context,
                           int now,
                           int from,
                           int to );

    void slot_net_event( const QString &name,
                         int axes,
                         RoboMsg msg );
protected:
    virtual void onNetEvent( const QString &name,
                             int axes,
                             RoboMsg &msg );

public:
    void setAgentName( const QString &name );
    QString getAgentName();

    void setAgentAxes( int axes );
    int getAgentAxes();

    void setAgent( const QString &name, int axes = -1 );

    void setChildAgents( const QStringList &childAgents );
    QStringList childAgents();

    void setLink( bool b );
    bool getLink();

protected:
    QString mAgentName;  //! device name
    int  mAgentAxes;

    QStringList mChildAgentNames;

    bool mbLink;
};

Q_DECLARE_METATYPE( appWidget::servContext )


#define API_GROUP( gp, postDo, beginDo, endDo, progDo )     appWidget::apiGroup gp; \
                                                    gp.mPostDo = (appWidget::apiPostDo)postDo; \
                                                    gp.mBeginDo = (appWidget::apiBeginDo)beginDo; \
                                                    gp.mEndDo = (appWidget::apiEndDo)endDo; \
                                                    gp.mProgDo = (appWidget::apiProgDo)progDo;

#define SERV_CONTEXT( context, base, gp, msg, para )    appWidget::servContext context; \
                                                    context.m_pBase = base;\
                                                    context.mGroup = gp;\
                                                    context.mMsg = msg;\
                                                    context.m_pPara = para;

#define post_request( msg, cls, api )  \
                                                    API_GROUP( _gp, &cls::post##api, &cls::begin##api, &cls::end##api, NULL );\
                                                    SERV_CONTEXT( _context, this, _gp, msg, 0 );\
                                                    signal_post_do( _context );

#define post_request_prog( msg, cls, api )  \
                                                    API_GROUP( _gp, &cls::post##api, &cls::begin##api, &cls::end##api, &cls::prog##api );\
                                                    SERV_CONTEXT( _context, this, _gp, msg, 0 );\
                                                    signal_post_do( _context );

#endif // APPWIDGET_H
