#include "progressgroup.h"
#include "ui_progressgroup.h"
#include "../../include/mydebug.h"
ProgressGroup::ProgressGroup(QWidget *parent) :
    QWidget( parent ),
    ui(new Ui::ProgressGroup)
{
    ui->setupUi(this);

    m_pProgList = new QList<ProgressElement*>;
    Q_ASSERT( NULL != m_pProgList );

    setWindowFlag( Qt::WindowMaximizeButtonHint, false );
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
    { /*show();*/ }
    else
    { hide(); }

    progress( name, id )->setVisible(b);

    foreach( ProgressElement *pg, *m_pProgList )
    {
        Q_ASSERT( NULL != pg );
        if ( pg->isVisible() )
        { return; }
    }

    //! hide all
    hide();
}

bool ProgressGroup::progressSnap( int &min,
               int &max,
               int &now )
{
    //! init
    min = INT_MAX;
    max = INT_MIN;
    now = INT_MAX;

    int iterMin, iterMax, iterNow;

    foreach ( ProgressElement *pg, *m_pProgList )
    {
        Q_ASSERT( NULL != pg );

        if ( pg->progress( iterMax, iterMin, iterNow) )
        {
            min = qMin( min, iterMin );
            max = qMax( max, iterMax );
            now = qMin( now, iterNow );
        }
    }

    //! visible?
    if ( min != INT_MAX )
    { return true; }
    else
    { return false; }
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
