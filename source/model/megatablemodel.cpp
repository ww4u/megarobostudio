#include "megatablemodel.h"

//! attrs
#define time_absolute   "time_absolute"
#define time_relative   "time_relative"

QString MegaTableModel::toString( timeType tp )
{
    if ( tp == time_abs )
    { return QStringLiteral( time_absolute ); }
    else if ( tp == time_rel )
    { return QStringLiteral( time_relative ); }
    else
    {logDbg()<<(int)tp;
        Q_ASSERT(false);
        return "";
    }
}

bool MegaTableModel::toValue( const QString &str, timeType &tp )
{
    if ( str_is(str, time_absolute) )
    {
        tp = time_abs;
        return true;
    }
    else if ( str_is(str, time_relative ) )
    {
        tp = time_rel;
        return true;
    }
    else
    { return false; }
}

MegaTableModel::MegaTableModel( const QString &className,
                                const QString &name )
{
    mClassName = className;

    mbStepAble = false;
    mbPrefAble = false;
    mbAutoTimeAble = false;
    mbToHereAble = false;

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

    mtType = time_abs;
}

void MegaTableModel::reverse()
{}

void MegaTableModel::autoTime( double speed, double speedT,
                               int align, int div )
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

bool MegaTableModel::toHereAble()
{ return mbToHereAble; }

bool MegaTableModel::smartEditable()
{ return mbSmartEditable; }

QList<int> MegaTableModel::checkColumnList()
{ return mCheckColumnList; }

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

void MegaTableModel::setTimeType( timeType tp )
{
    //! change
    if ( mtType != tp )
    {
        if ( tp == time_abs )
        { switchTimeType( mtType, tp ); }
        else if ( tp == time_rel )
        { switchTimeType( mtType, tp ); }
        else
        { Q_ASSERT( false ); }
    }
    else
    {}

    mtType = tp;
}
MegaTableModel::timeType MegaTableModel::getTimeType()
{ return mtType; }

tpvType MegaTableModel::getAbsT( int index )
{ return 0; }

//! align to .1
double MegaTableModel::aligndT( double t, int align, int div )
{
    int64 normt;
    //! align time
    normt = (int64)(t * div);
    if ( normt == 0 )
    { normt = 1; }

    if ( div == 1 )
    { return ceil( t ); }

    //! align to 0.1
//    return normt / 10.0;
//    return (((normt + 4)/5)*5) / 10.0;
    return  ( (( normt + align - 1)/align) * align ) / (double)div;
}

void MegaTableModel::switchTimeType( timeType pre, timeType nxt )
{}


TimebaseHelp::TimebaseHelp( MegaTableModel::timeType tp )
{
    mTimebase = tp;
    mAccT = 0;
}

tpvType TimebaseHelp::accT( int size, tpvType t )
{
    //! first t
    if ( size == 0 )
    { mAccT = t; }
    else
    {
        if ( mTimebase == MegaTableModel::time_abs )
        { mAccT = t; }
        else
        { mAccT +=t; }
    }
    return mAccT;
}

tpvType TimebaseHelp::getAccT( )
{ return mAccT; }

