#ifndef EVENTVIEWER_H
#define EVENTVIEWER_H

#include <QtWidgets>

#include "../../../bus/receivecache.h"

#include "../../../model/eventactionmodel.h"
#include "../../../model/comboxdelegate.h"

namespace Ui {
class eventViewer;
}

class eventViewer : public QDialog
{
    Q_OBJECT

public:
    static int exceptionCode( const QString &str );
    static int actionCode( const QString &str );

//    static int exceptionMCode( int exceptionId );
//    static int exceptionSCode( int exceptionId );

    static int exceptionIndex( const QString &str );

public:
    explicit eventViewer(
                          EventActionModel *pModel,
                          QWidget *parent = 0);
    ~eventViewer();

protected Q_SLOTS:
    void slot_event( eventId id, frameData );
public Q_SLOTS:
    void slot_exception_changed();

    void on_btnExport_clicked();

    void on_btnClear_clicked();

    void on_spinBox_valueChanged(int arg1);

private slots:
    void on_btnEventExport_clicked();

    void on_btnEventLoad_clicked();

    void on_btnEventAdd_clicked();

    void on_btnEventDel_clicked();


private:
    Ui::eventViewer *ui;

    QStringList mEventStringList;
    comboxDelegate mEventDelegate;

    QStringList mActionStringList;
    comboxDelegate mActionDelegate;

//    receiveCache * m_pInterruptSrc;
};

#endif // EVENTVIEWER_H
