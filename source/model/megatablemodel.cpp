#include "megatablemodel.h"

MegaTableModel::MegaTableModel( const QString &className,
                                const QString &name )
{
    mClassName = className;

    mbStepAble = false;
    mbPrefAble = false;
    mbAutoTimeAble = false;

    mSectionAble<<true;     //! joint,hand,geometry

    mbSmartEditable = false;
    mRpcReq = RpcRequest::e_req_none;
    mRpcType = RpcRequest::e_type_none;

    connect( this, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             this, SIGNAL(signal_data_changed()));

    connect( this, SIGNAL(rowsInserted(QModelIndex,int,int)),
             this, SIGNAL(signal_data_changed()));

    connect( this, SIGNAL(rowsRemoved(QModelIndex,int,int)),
             this, SIGNAL(signal_data_changed()));

    connect( this, SIGNAL(modelReset()),
             this, SIGNAL(signal_data_changed()));
}

void MegaTableModel::reverse()
{}

void MegaTableModel::autoTime( double speed, double speedT )
{}

QString MegaTableModel::className()
{ return mClassName; }

void MegaTableModel::setStepAble( bool b )
{ mbStepAble = b; }
bool MegaTableModel::stepAble()
{ return mbStepAble; }

void MegaTableModel::setPrefAble( bool b )
{ mbPrefAble = b; }
bool MegaTableModel::prefAble()
{ return mbPrefAble; }

void MegaTableModel::setAutoTimeAble( bool b )
{ mbAutoTimeAble = b; }

bool MegaTableModel::autoTimeAble()
{ return mbAutoTimeAble; }

bool MegaTableModel::smartEditable()
{ return mbSmartEditable; }

RpcRequest::EnumRequest MegaTableModel::rpcRequest()
{ return mRpcReq; }
RpcRequest::EnumParaType MegaTableModel::rpcParaType()
{ return mRpcType; }

int MegaTableModel::sectionSize()
{ return mSectionAble.size(); }
bool MegaTableModel::sectionAble( int iSec )
{ return mSectionAble.at(iSec); }
void MegaTableModel::setSectionAble( int iSec, bool b )
{
    Q_ASSERT( iSec < mSectionAble.size() && iSec >=0 );

    mSectionAble[ iSec ] = b;
}
QList<bool> &MegaTableModel::sectionAbleList()
{ return mSectionAble; }

QStringList MegaTableModel::exportOptList()
{ return mExportOptList; }

QString MegaTableModel::fmtString( const QStringList &list )
{
    QStringList strList;
    for ( int i = 1; i <= list.size(); i++ )
    {
        strList<<QString("\%%1").arg( i );
    }

    return strList.join('/');
}

void MegaTableModel::setRpc( int row, RpcRequest &req )
{}

//! align to .5
double MegaTableModel::aligndT( double t )
{
    int normt;
    //! align time
    normt = (int)(t * 10);
    if ( normt == 0 )
    { normt = 1; }

    return (((normt + 4)/5)*5) / 10.0;
}

