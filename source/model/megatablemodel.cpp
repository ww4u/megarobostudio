#include "megatablemodel.h"

MegaTableModel::MegaTableModel( const QString &className,
                                const QString &name ) : scriptFile(name)
{
    mClassName = className;

    mbStepAble = false;
    mbPrefAble = false;

    mSectionAble<<true;     //! joint,hand,geometry

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

//bool MegaTableModel::verify()
//{ return false; }

//int  MegaTableModel::build()
//{ return 0; }
