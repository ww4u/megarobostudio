#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QWidget>

#include "modelview.h"
#include "../../widget/highlighter.h"

namespace Ui {
class scriptEditor;
}

class scriptEditor : public modelView
{
    Q_OBJECT

public:
    explicit scriptEditor(QWidget *parent = 0);
    ~scriptEditor();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual bool matchModelObj(mcModelObj *pObj);

    virtual int  save( QString &outFileName );
    virtual int  saveAs( QString &outFileName );

protected:
    void reloadFile( const QString &fileName );

private:
    Ui::scriptEditor *ui;
    Highlighter *m_pHighLigter;
};

#endif // SCRIPTEDITOR_H
