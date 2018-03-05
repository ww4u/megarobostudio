#ifndef MODELVIEW_H
#define MODELVIEW_H

#include <QWidget>

#include "../../model/mcmodelobj.h"
#include "../../model/modelsyspref.h"
#include "../../model/mcmodel.h"

#include "../../app/appwidget.h"
#include "../../app/dpcobj.h"

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
};

#endif // MODELVIEW_H
