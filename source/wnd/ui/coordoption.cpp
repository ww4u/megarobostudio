#include "coordoption.h"
#include "ui_coordoption.h"

CoordOption::CoordOption(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoordOption)
{
    ui->setupUi(this);

    _buttons.append( ui->radBody );
    _buttons.append( ui->radTool );
    _buttons.append( ui->radUser );

    foreach( QRadioButton *pBtn, _buttons )
    {
        connect( pBtn, SIGNAL(clicked(bool)),
                 this, SLOT(slot_option_changed()) );
    }
}

CoordOption::~CoordOption()
{
    delete ui;
}

void CoordOption::setOption( int index )
{
    Q_ASSERT( index >=0 && index < _buttons.size() );
    _buttons[index]->setChecked( true );
}
int CoordOption::option()
{
    for ( int i = 0; i < _buttons.size(); i++ )
    {
        if ( _buttons[i]->isChecked() )
        { return i; }
    }

    return 0;
}

void CoordOption::slot_option_changed()
{
    emit signal_option_changed( option() );
}
