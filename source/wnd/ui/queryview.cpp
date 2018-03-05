#include "queryview.h"
#include "ui_queryview.h"

#include "../../model/spyitemmodel.h"

queryView::queryView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::queryView)
{
    ui->setupUi(this);
    ui->tab->horizontalHeader()->setStretchLastSection(true);

    setAttribute( Qt::WA_DeleteOnClose );
}

queryView::~queryView()
{
    delete ui;
}

void queryView::setModel( QAbstractItemModel *pModel,
                          const QString &name,
                          const QString &comment )
{
    Q_ASSERT( NULL != pModel );
    ui->tab->setModel( pModel );
    ui->tabWidget->setTabText( 0, name );
    setWindowTitle( name );
    ui->labComment->setText( comment );

    spyItemModel *pRealModel;
    pRealModel = (spyItemModel*)pModel;
    Q_ASSERT( NULL != pRealModel );

    //! apply format
    pRealModel->setViewFmt( spyItem::toViewFormat( ui->cmbViewFmt->currentIndex() ) );
}

void queryView::on_btnExport_clicked()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("CSV (*.csv )") );
    fDlg.selectFile( windowTitle() );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    QFile file( fDlg.selectedFiles().first() );

    if ( !file.open(QIODevice::WriteOnly) )
    { return; }

    QVariant var;
    QAbstractItemModel *pModel = ui->tab->model();
    Q_ASSERT( NULL != pModel );

    QTextStream fileStream( &file );
    for ( int i = 0; i < pModel->rowCount(); i++ )
    {
        //! time
        var = pModel->data( pModel->index(i,0) );
        fileStream<<var.toString()<<",";

        //! value
        var = pModel->data( pModel->index(i,1) );
        fileStream<<var.toString()<<"\n";
    }

    file.close();
}

void queryView::on_cmbViewFmt_currentIndexChanged(int index)
{
    spyItemModel *pRealModel;

    pRealModel = (spyItemModel *)ui->tab->model();
    Q_ASSERT( NULL != pRealModel );
    pRealModel->setViewFmt( spyItem::toViewFormat( index ) );

    ui->tab->update();
}
