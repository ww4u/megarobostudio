#ifndef IMAGEENTITY_H
#define IMAGEENTITY_H

#include <QWidget>
#include "entitywidget.h"
namespace Ui {
class ImageEntity;
}

class ImageEntity : public EntityWidget
{
    Q_OBJECT

public:
    explicit ImageEntity(QWidget *parent = 0);
    ~ImageEntity();

public:
    virtual int serialOut( QXmlStreamWriter &writer, QList<EntityWidget *> &refWidgets );
    virtual int serialIn( QXmlStreamReader &reader );

    virtual QString description();
public:
    void setImage( const QString &img );
    void setDataClass( const QString &dataCls );

    void setLabel( const QString &lab );
    QString label();

private:
    Ui::ImageEntity *ui;

    QString mIconName;
    QString mDataClass;
};

#endif // IMAGEENTITY_H
