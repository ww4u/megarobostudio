#include "robohand.h"
#include "ui_robohand.h"

#include <QDialog>
#include "../../model/handactionmodel.h"

RoboHand::RoboHand(QWidget *parent) :
    modelView(parent),
    ui(new Ui::RoboHand)
{
    ui->setupUi(this);

    SET_INFO_ATTR();

    spyEdited();
}

RoboHand::~RoboHand()
{
    delete ui;
}

void RoboHand::spyEdited()
{

}

void RoboHand::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    //! set hand model
    VRobot *pRobot = (VRobot*)pObj;
    ui->tableView->setModel( pRobot->handActions() );
}

void RoboHand::updateScreen()
{}
void RoboHand::on_btnExport_clicked()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("hand action (*.hand)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    handActionModel *pModel;
    pModel = ((handActionModel*)ui->tableView->model());
    Q_ASSERT( NULL != pModel );
    int ret = pModel->save( fDlg.selectedFiles().first() );
    if ( ret == 0 )
    { }
    else
    { }
}

void RoboHand::on_btnLoad_clicked()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptOpen );
    fDlg.setNameFilter( tr("hand action (*.hand)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    handActionModel *pModel;
    pModel = ((handActionModel*)ui->tableView->model());
    Q_ASSERT( NULL != pModel );
    int ret = pModel->load( fDlg.selectedFiles().first() );
    if ( ret == 0 )
    { /*pModel->endResetModel();*/ }
    else
    {}
}

void RoboHand::on_btnAdd_clicked()
{
    int curRow;

    //! current
    curRow = ui->tableView->currentIndex().row();
    logDbg()<<curRow;

    ui->tableView->model()->insertRow( curRow + 1 );
    ui->tableView->setCurrentIndex( ui->tableView->model()->index( curRow + 1, 0 ) );
}

void RoboHand::on_btnDel_clicked()
{
    ui->tableView->model()->removeRow( ui->tableView->currentIndex().row() );
}
