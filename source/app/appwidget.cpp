#include "appwidget.h"

appWidget::appWidget(QWidget *parent) : QWidget(parent)
{
    mbLink = false;
    mAgentAxes = -1;
}

void appWidget::slot_begin_do( appWidget::servContext context )
{
    Q_ASSERT( NULL != context.m_pBase );

    //! pointer
    if ( context.m_pBase != this )
    { return; }

    //! api null
    if ( context.mGroup.mBeginDo == NULL )
    { return; }

    (context.m_pBase->*(context.mGroup.mBeginDo))( context.m_pPara );
}
void appWidget::slot_end_do( appWidget::servContext context,
                  int ret )
{
    Q_ASSERT( NULL != context.m_pBase );

    //! pointer
    if ( context.m_pBase != this )
    { return; }

    //! api null
    if ( context.mGroup.mEndDo == NULL )
    { return; }

    (context.m_pBase->*(context.mGroup.mEndDo))( ret, context.m_pPara );
}

void appWidget::slot_prog_do( appWidget::servContext context,
                            int now,
                            int from,
                            int to )
{
    Q_ASSERT( NULL != context.m_pBase );

    //! pointer
    if ( context.m_pBase != this )
    { return; }

    //! api null
    if ( context.mGroup.mProgDo == NULL )
    { return; }

    (context.m_pBase->*(context.mGroup.mProgDo))( now, from, to, context.m_pPara );
}

void appWidget::slot_net_event( const QString &name,
                     int axes,
                     RoboMsg msg )
{
    if ( !mbLink )
    { return; }

    do
    {
        //! child
        if ( mChildAgentNames.contains( name ) )
        { break; }

        //! self
        if ( name != mAgentName )
        { return; }

        //! valid axes
        if ( mAgentAxes != -1 )
        {
            if (axes == mAgentAxes )
            {}
            else
            { return; }
        }

    }while( 0 );

    //! on event
    onNetEvent( name, axes, msg );
}

void appWidget::onNetEvent( const QString &name,
                         int axes,
                         RoboMsg &msg )
{}

void appWidget::setAgentName( const QString &name )
{ mAgentName = name; }
QString appWidget::getAgentName()
{ return mAgentName; }

void appWidget::setAgentAxes( int axes )
{ mAgentAxes = axes; }
int appWidget::getAgentAxes()
{ return mAgentAxes; }

void appWidget::setAgent( const QString &name, int axes )
{
    mAgentName = name;
    mAgentAxes = axes;
}

void appWidget::setChildAgents( const QStringList &childAgents )
{ mChildAgentNames = childAgents; }
QStringList appWidget::childAgents()
{ return mChildAgentNames; }

void appWidget::setLink( bool b )
{ mbLink = b; }
bool appWidget::getLink()
{ return mbLink; }

