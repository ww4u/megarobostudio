#ifndef MODELVIEW_H
#define MODELVIEW_H

#include <QtWidgets>

#include "../../model/mcmodelobj.h"
#include "../../model/modelsyspref.h"
#include "../../model/mcmodel.h"

#include "../../app/appwidget.h"
#include "../../app/dpcobj.h"

#define OK_ABLE_BIT     0
#define CANCEL_ABLE_BIT 1
#define APPLY_ABLE_BIT  2

#define SET_INFO_ATTR()     \
                        unset_bit( mAttributes, OK_ABLE_BIT );\
                        unset_bit( mAttributes, APPLY_ABLE_BIT );
class modelView : public appWidget
{
    Q_OBJECT

protected:
    static dpcObj *m_pDpcObj;

public:
    static void attachDpcObj( dpcObj *pObj );

public:
    modelView( QWidget *parent = 0 );
    virtual ~modelView();

public:
    virtual void retranslate();

protected:
    void buildConnection();

Q_SIGNALS:
    void sigClose( QWidget * );
    void sigSaveRequest( QWidget * );

    void sigModified( bool b );
    void sigModified( modelView *pView, bool b );

    void sigActiveDeviceChanged( const QString &devName );

protected Q_SLOTS:
    void slot_modified();
    void slotModified( bool b );

    void slot_request( const RpcRequest &req );

    //! ui attr
public:
    bool isCanceAble();
    bool isOkAble();
    bool isApplyAble();

    QString name();

    void setReqSpy( bool b );
    bool reqSpy();

public:
    virtual void setModelObj( mcModelObj *pObj );
    mcModelObj *getModelObj();

    virtual bool matchModelObj( mcModelObj *pObj );
    virtual int  save( QString &outFileName );
    virtual int  saveAs( QString &outFileName );

    virtual int setApply();

    QStringList & filePattern();

    virtual void setMcModel( mcModel *pMcModel );

    bool modified();

    virtual void syncData();

    virtual void updateScreen();
    virtual void updateModel();

    virtual void setActive();

protected:
    virtual void onRequest( RpcRequest &req );

protected:
    mcModelObj *m_pModelObj;
    bool mbGc;

    mcModel *m_pmcModel;

    QStringList mFilePattern;   //!desc-ext

    bool mbModified;
    quint32 mAttributes;
    QString mName;

    bool mbReqSpy;

};

//! dir = true :: to screen
#define exchange_check( control, val, dir )  \
                                        if ( dir )\
                                        { ui->control->setChecked( val ); } \
                                        else \
                                        { val = ui->control->isChecked(); }


#define exchange_spin( control, val, dir )  \
                                        if ( dir )\
                                        { ui->control->setValue( val ); } \
                                        else \
                                        { val = ui->control->value(); }

#define exchange_combox( control, val, dir )    \
                                        if ( dir )\
                                        { ui->control->setCurrentIndex( val ); } \
                                        else \
                                        { val = ui->control->currentIndex(); }

#define exchange_container_check( control, val, dir )  \
                                        if ( dir )\
                                        { control->setChecked( val ); } \
                                        else \
                                        { val = control->isChecked(); }


#define exchange_container_spin( control, val, dir )  \
                                        if ( dir )\
                                        { control->setValue( val ); } \
                                        else \
                                        { val = control->value(); }


//! spy
#define spy_control_edit( control )   connect( control, SIGNAL(editingFinished()), this, SLOT(slot_modified()));
#define spy_control_combox( control ) connect( control, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_modified()));
#define spy_control_checkbox( control ) connect( control, SIGNAL(clicked()), this, SLOT(slot_modified()));



#define install_spy()       \
for ( int i = 0; i < sizeof_array(checkBoxes); i++ )\
{ spy_control_checkbox( checkBoxes[i] ); }\
for ( int i = 0; i < sizeof_array(radBoxes); i++ )\
{ spy_control_checkbox( radBoxes[i] ); }\
for ( int i = 0; i < sizeof_array(edits); i++ )\
{ spy_control_edit( edits[i] ); }\
\
for ( int i = 0; i < sizeof_array(spinBoxes); i++ )\
{ spy_control_edit( spinBoxes[i] ); }\
\
for ( int i = 0; i < sizeof_array(doubleSpinBoxes); i++ )\
{ spy_control_edit( doubleSpinBoxes[i] ); }\
\
for ( int i = 0; i < sizeof_array(comboxes); i++ )\
{ spy_control_combox( comboxes[i] ); }

#define LINK_MODIFIED( subview )    connect( subview, \
                                    SIGNAL(sigModified(bool)), \
                                    this, \
                                    SLOT(slotModified(bool)));

#endif // MODELVIEW_H
