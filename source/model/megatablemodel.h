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

    int sectionSize();
    bool sectionAble( int iSec );
    void setSectionAble( int iSec, bool b );
    QList<bool> &sectionAbleList();

    QStringList exportOptList();

Q_SIGNALS:
    void signal_data_changed();

protected:
    QString mClassName;

    bool mbStepAble;
    bool mbPrefAble;

    QList <bool> mSectionAble;       //! 0,1,2
    QStringList mExportOptList;
};

#endif // MEGATABLEMODEL_H
