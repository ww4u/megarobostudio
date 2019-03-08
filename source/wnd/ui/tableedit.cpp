#include "tableedit.h"

tableEdit::tableEdit( QWidget *parent ) : modelView( parent )
{
    m_pEditMenu = NULL;

    setupUi();
}

tableEdit::~tableEdit()
{}

void tableEdit::contextMenuEvent(QContextMenuEvent *event)
{
    Q_ASSERT( NULL != m_pEditMenu );

    m_pContextActionToHere->setVisible( context_toHere_able() );

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
void tableEdit::context_toHere()
{}

bool tableEdit::context_toHere_able()
{ return false; }

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

void tableEdit::slot_context_tohere()
{
    context_toHere();
}

void tableEdit::setupUi()
{
    m_pEditMenu = new QMenu( this );

    m_pContextActionInsertBefore = m_pEditMenu->addAction( QIcon(":/res/image/icon/addition.png"), tr("Insert before"), this, SLOT(slot_context_add_before()) );
    m_pContextActionInsertAfter = m_pEditMenu->addAction( QIcon(":/res/image/icon/addition.png"), tr("Insert below"), this, SLOT(slot_context_add_below()) );
    m_pEditMenu->addSeparator();
    m_pContextActionDelete = m_pEditMenu->addAction( QIcon(":/res/image/icon/delete.png"), tr("Delete line"), this, SLOT(slot_context_remove()) );
    m_pEditMenu->addSeparator();
    m_pContextActionClear = m_pEditMenu->addAction( QIcon(":/res/image/icon/trash.png"), tr("Clear"), this, SLOT(slot_context_clear()) );
//    m_pEditMenu->addSeparator();
    m_pContextActionToHere = m_pEditMenu->addAction( QIcon(":/res/image/icon/skip.png"), tr("To here"), this, SLOT(slot_context_tohere()) );
}
void tableEdit::retranslateContextMenu()
{
    m_pContextActionInsertBefore->setText( tr("Insert before"));
    m_pContextActionInsertAfter->setText( tr("Insert below"));
    m_pContextActionDelete->setText( tr("Delete line"));
    m_pContextActionClear->setText( tr("Clear"));
}
