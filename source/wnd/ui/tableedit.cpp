#include "tableedit.h"

tableEdit::tableEdit( QWidget *parent ) : modelView( parent )
{
    m_pEditMenu = NULL;

    setupUi();

//    scroll();
//    setHorizontalScrollBarPolicy
}

tableEdit::~tableEdit()
{}

void tableEdit::contextMenuEvent(QContextMenuEvent *event)
{
    Q_ASSERT( NULL != m_pEditMenu );
    m_pEditMenu->popup( mapToGlobal( event->pos() ) );
    event->accept();
}

void tableEdit::context_remove()
{}
void tableEdit::context_add_before()
{}
void tableEdit::context_add_below()
{}
void tableEdit::context_clear()
{}

void tableEdit::slot_context_remove()
{
    context_remove();
}
void tableEdit::slot_context_add_before()
{
    context_add_before();
}
void tableEdit::slot_context_add_below()
{
    context_add_below();
}
void tableEdit::slot_context_clear()
{
    context_clear();
}

void tableEdit::setupUi()
{
    m_pEditMenu = new QMenu( this );

    m_pEditMenu->addAction( QIcon(":/res/image/icon/addition.png"), tr("Insert before"), this, SLOT(slot_context_add_before()) );
    m_pEditMenu->addAction( QIcon(":/res/image/icon/addition.png"), tr("Insert below"), this, SLOT(slot_context_add_below()) );
    m_pEditMenu->addSeparator();
    m_pContextActionDelete = m_pEditMenu->addAction( QIcon(":/res/image/icon/delete.png"), tr("Delete Line"), this, SLOT(slot_context_remove()) );
    m_pEditMenu->addSeparator();
    m_pContextActionClear = m_pEditMenu->addAction( QIcon(":/res/image/icon/trash.png"), tr("Clear"), this, SLOT(slot_context_clear()) );
}
