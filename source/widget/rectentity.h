#ifndef RECTENTITY_H
#define RECTENTITY_H

#include <QWidget>
#include "entitywidget.h"

namespace Ui {
class RectEntity;
}

class RectEntity : public EntityWidget
{
    Q_OBJECT

public:
    explicit RectEntity(QWidget *parent = 0);
    ~RectEntity();

public:
    virtual int serialOut( QXmlStreamWriter &writer, QList<EntityWidget *> &refWidgets );
    virtual int serialIn( QXmlStreamReader &reader );
    virtual QString description();

public:
    void setLabel( const QString &lab );
    QString label();

protected:
    Ui::RectEntity *ui;


};

#endif // RECTENTITY_H
