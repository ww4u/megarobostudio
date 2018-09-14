#include "aboutdlg.h"
#include "ui_aboutdlg.h"


aboutDlg::aboutDlg(SysPara *pPara,
                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDlg)
{
    m_pPara = pPara;

    ui->setupUi(this);
    ui->label_3->setText( qApp->applicationVersion() );
    ui->label_5->setText( tr("Build:") + __DATE__ + " " +  __TIME__ );

    //! admin
    if ( m_pPara->mSysMode == sys_admin )
    { ui->label_5->setVisible( true ); }
    else
    { ui->label_5->setVisible( false ); }
}

aboutDlg::~aboutDlg()
{
    delete ui;
}

//! ctrl + f1
void aboutDlg::keyReleaseEvent(QKeyEvent *event )
{
    if ( event->key() == Qt::Key_F1 && event->modifiers() == Qt::ControlModifier )
    { ui->label_5->setVisible( !ui->label_5->isVisible() ); }

    //! admin
    if ( ui->label_5->isVisible() )
    { m_pPara->mSysMode = sys_admin; }
    else
    { m_pPara->mSysMode = sys_normal; }
}
