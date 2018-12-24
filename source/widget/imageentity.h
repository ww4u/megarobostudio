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
    void setImage( const QImage &img );

    void setLabel( const QString &lab );
    QString label();

private:
    Ui::ImageEntity *ui;
};

#endif // IMAGEENTITY_H
