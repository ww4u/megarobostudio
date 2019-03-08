#ifndef TABLEEDIT_H
#define TABLEEDIT_H

#include <QtWidgets>
#include "modelview.h"

class tableEdit : public modelView
{
    Q_OBJECT
public:
    tableEdit( QWidget *parent = 0 );
    virtual ~tableEdit();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void context_remove();
    virtual void context_add_before();
    virtual void context_add_below();
    virtual void context_clear();

    virtual void context_toHere();
    virtual bool context_toHere_able();
protected Q_SLOTS:
    void slot_context_remove();
    void slot_context_add_before();
    void slot_context_add_below();
    void slot_context_clear();
    void slot_context_tohere();
protected:
    void setupUi();
    void retranslateContextMenu();

protected:
    QMenu *m_pEditMenu;

    QAction *m_pContextActionInsertBefore;
    QAction *m_pContextActionInsertAfter;

    QAction *m_pContextActionClear;
    QAction *m_pContextActionDelete;

    QAction *m_pContextActionToHere;
};

#endif // TABLEEDIT_H
