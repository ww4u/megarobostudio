#include "robosketch.h"
#include "ui_robosketch.h"

RoboSketch::RoboSketch(QWidget *parent) :
    modelView(parent),
    ui(new Ui::RoboSketch)
{
    ui->setupUi(this);

    SET_INFO_ATTR();
}

RoboSketch::~RoboSketch()
{
    delete ui;
}

void RoboSketch::changeEvent(QEvent * event)
{
    modelView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

void RoboSketch::setSketch( const QString &rsrc )
{
    ui->label->setPixmap( QPixmap( rsrc ) );
}
