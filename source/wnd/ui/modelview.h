#ifndef MODELVIEW_H
#define MODELVIEW_H

#include <QWidget>

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

protected:
    void buildConnection();

Q_SIGNALS:
    void sigClose( QWidget * );

    //! ui attr
public:
    bool isCanceAble();
    bool isOkAble();
    bool isApplyAble();

public:
    virtual void setModelObj( mcModelObj *pObj );
    mcModelObj *getModelObj();

    virtual bool matchModelObj( mcModelObj *pObj );
    virtual int  save( QString &outFileName );
    virtual int  saveAs( QString &outFileName );

    virtual int setApply();

    QStringList & filePattern();

    virtual void setMcModel( mcModel *pMcModel );

    void setModified( bool b );
    bool modified();

    virtual void syncData();

protected:
    mcModelObj *m_pModelObj;

    mcModel *m_pmcModel;

    QStringList mFilePattern;   //!desc-ext

    bool mbModified;
    quint32 mAttributes;

};

#endif // MODELVIEW_H
