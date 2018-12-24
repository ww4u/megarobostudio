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
    void setLabel( const QString &lab );
    QString label();

private:
    Ui::RectEntity *ui;


};

#endif // RECTENTITY_H
