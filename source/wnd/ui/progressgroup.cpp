#include "progressgroup.h"
#include "ui_progressgroup.h"

ProgressGroup::ProgressGroup(QWidget *parent) :
//    QDialog(parent),
    QWidget( parent ),
    ui(new Ui::ProgressGroup)
{
    ui->setupUi(this);

    m_pProgList = new QList<ProgressElement*>;
    Q_ASSERT( NULL != m_pProgList );
}

ProgressGroup::~ProgressGroup()
{
    delete ui;

    delete m_pProgList;
}

void ProgressGroup::progressInfo( const QString &name, int id, const QString &str )
{ progress( name, id )->progressInfo( str ); }
void ProgressGroup::progressProg( const QString &name, int id, int n )
{ progress( name, id )->progressProg( n ); }
void ProgressGroup::progressRange( const QString &name, int id, int a, int b )
{ progress( name, id )->progressRange( a, b); }
void ProgressGroup::progressShow( const QString &name, int id, bool b )
{
    //! show
    if ( b )
    { show(); }

    progress( name, id )->setVisible(b);

    foreach( ProgressElement *pg, *m_pProgList )
    {
        Q_ASSERT( NULL != pg );
        if ( pg->isVisible() )
        { return; }
    }

    hide();
}

ProgressElement * ProgressGroup::progress( const QString &name, int id )
{
    Q_ASSERT( NULL != m_pProgList );
    //! find
    foreach( ProgressElement *pg, *m_pProgList )
    {
        Q_ASSERT( NULL != pg );
        if ( pg->match( name, id ) )
        { return pg; }
    }

    //! create one
    ProgressElement *pEle;
    pEle = new ProgressElement();
    Q_ASSERT( NULL != pEle );

    pEle->set( name, id );
    //! connect
    connect( pEle, SIGNAL(sigCancel( const QString&, int)),
             this, SIGNAL(sigCancel( const QString&, int)) );

    ui->verticalLayout->addWidget( pEle );

    m_pProgList->append( pEle );
    return pEle;
}
