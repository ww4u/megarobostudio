#include "operatorlib.h"
#include "ui_operatorlib.h"

#include "../../mrg/opitem.h"

#include "../../widget/opliblistwidget.h"

#include "../../include/mydebug.h"

OperatorLib::OperatorLib(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OperatorLib)
{
    ui->setupUi(this);
}

OperatorLib::~OperatorLib()
{
    delete ui;
}

int OperatorLib::loadLib( const QString &name )
{
    OpLib opLib;
    if ( opLib.load( name ) )
    { logDbg()<<name<<opLib.mGroups.size(); }
    else
    {
        return -1;
    }

    //! add group
    for ( int i = 0; i < opLib.mGroups.size(); i++ )
    {
        //! tab
        OpLibListWidget *pWig;

        pWig = new OpLibListWidget();
        ui->tabWidget->addTab( pWig, opLib.mGroups[i]->mName );

        //! sub item
        QListWidgetItem *pItem;
        for ( int j = 0; j < opLib.mGroups[i]->mItems.size(); j++ )
        {
            pItem = new QListWidgetItem();
            if ( NULL == pItem )
            { return -1; }

            pItem->setText(  opLib.mGroups[i]->mItems[j]->mName  );
            pItem->setIcon( QIcon( opLib.mGroups[i]->mItems[j]->mRes) );

            pItem->setData( Qt::UserRole, opLib.mGroups[i]->mItems[j]->mRes );

            pWig->addItem( pItem );
        }
    }

    return 0;
}
