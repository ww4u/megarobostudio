#ifndef MRXVIEW_H
#define MRXVIEW_H

#include "modelview.h"

class mrxView : public modelView
{
    Q_OBJECT
public:
    mrxView( QWidget *parent = 0 );

public:
    virtual void setAxesId( int id );
    int getAxesId();

    virtual void modelChanged();

protected:
    byte mAxesId;
};

#endif // MRXVIEW_H
