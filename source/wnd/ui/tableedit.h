#ifndef TABLEEDIT_H
#define TABLEEDIT_H

#include <QtWidgets>
#include "modelView.h"

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

protected Q_SLOTS:
    void slot_context_remove();
    void slot_context_add_before();
    void slot_context_add_below();

protected:
    void setupUi();

protected:
    QMenu *m_pEditMenu;
};

#endif // TABLEEDIT_H
