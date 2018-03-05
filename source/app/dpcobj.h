#ifndef SERVTHREAD_H
#define SERVTHREAD_H

#include <QObject>
#include "appwidget.h"

class dpcObj : public QObject
{
    Q_OBJECT

protected:
    static dpcObj *_pSingleObj;

public:
    static dpcObj* instance();

public:
    dpcObj();

public:
    void tlsProgress( int now, int from, int to );

Q_SIGNALS:
    void sig_begin_do( appWidget::servContext context );
    void sig_end_do( appWidget::servContext context, int ret );
    void sig_prog_do( appWidget::servContext context, int now, int from, int to );

protected Q_SLOTS:
    void slot_post_do( appWidget::servContext context
                     );

protected:
    appWidget::servContext mTlsContext;

};

#endif // SERVTHREAD_H
