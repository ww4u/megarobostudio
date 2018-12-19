#include "cfgtab.h"
#include "ui_cfgtab.h"

cfgTab::cfgTab(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::cfgTab)
{
    ui->setupUi(this);
}

cfgTab::~cfgTab()
{
    delete ui;
}

void cfgTab::changeEvent( QEvent *event )
{
    QTabWidget::changeEvent( event );

    if ( event->type() == QEvent::LanguageChange )
    { ui->retranslateUi( this ); }

//    for( int i = 0; i < count(); i++ )
//    {
//        widget( i )->changeEvent( event );
//    }
}

void cfgTab::on_cfgTab_currentChanged(int index)
{

}
