#ifndef MEGATABLEMODEL_H
#define MEGATABLEMODEL_H

#include <QAbstractTableModel>

#include "scriptfile.h"

class MegaTableModel : public QAbstractTableModel, public scriptFile
{
    Q_OBJECT

public:
    MegaTableModel( const QString &className,
                    const QString &name="" );
    virtual ~MegaTableModel()
    {}

public:
    virtual void reverse();

public:
    QString className();

    void setStepAble( bool b );
    bool stepAble();

    void setPrefAble( bool b );
    bool prefAble();

    bool smartEditable();

    RpcRequest::EnumRequest rpcRequest();
    RpcRequest::EnumParaType rpcParaType();

    int sectionSize();
    bool sectionAble( int iSec );
    void setSectionAble( int iSec, bool b );
    QList<bool> &sectionAbleList();

    QStringList exportOptList();

    virtual void setRpc( int row, RpcRequest &req );

Q_SIGNALS:
    void signal_data_changed();

protected:
    QString mClassName;

    bool mbStepAble;
    bool mbPrefAble;

    QList <bool> mSectionAble;              //! 0,1,2
    QStringList mExportOptList;

    bool mbSmartEditable;
    QList< int > mSmartEditColumns;         //! 3,4,5
    RpcRequest::EnumRequest mRpcReq;
    RpcRequest::EnumParaType mRpcType;

    QStringList mHeaderList, mTitleList;
};

#endif // MEGATABLEMODEL_H
