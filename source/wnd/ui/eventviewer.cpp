#include "eventviewer.h"

#include "ui_eventviewer.h"

#include "../../device/mrq/deviceMRQ.h"

struct struExceptionDesc
{
    QString str;
    int id;
    int mCode;
    int sCode;
    int len;
    int mPattern[6];       //! -1 not care

};

static struExceptionDesc _exceptions_[]=
{
    { QStringLiteral("Lose Step"),
      event_over_step,
      mc_SYSTEM, sc_SYSTEM_EVENTCODE_Q,
      6,
      { -1, 4, 0, -1, -1, -1,}
    },

    { QStringLiteral("Over Distance"),
      event_over_distance,
      mc_SYSTEM, sc_SYSTEM_EVENTCODE_Q,
      6,
      { -1, 2, 0, -1, -1, -1,}
    },
    { QStringLiteral("Over Angle"),
      event_over_angle,
      mc_SYSTEM, sc_SYSTEM_EVENTCODE_Q,
      6,
      { -1, 2, 1, -1, -1, -1,}
    },

};

struct struExceptionActionDesc
{
    QString str;
    int id;
};
static struExceptionActionDesc _exception_actions_[] =
{
    { QStringLiteral("Stop"), exception_action_stop },
    { QStringLiteral("Prompt"), exception_action_prompt },
    { QStringLiteral("Prompt+Stop"), exception_action_prompt_stop },
};

int eventViewer::exceptionCode( const QString &str )
{
    for( int i = 0; i < sizeof_array(_exceptions_); i++ )
    {
        if ( _exceptions_[i].str == str )
        { return _exceptions_[i].id; }
    }

    return _exceptions_[0].id;
}
int eventViewer::actionCode( const QString &str )
{
    for( int i = 0; i < sizeof_array(_exception_actions_); i++ )
    {
        if ( _exception_actions_[i].str == str )
        { return _exception_actions_[i].id; }
    }

    return _exception_actions_[0].id;
}

//int eventViewer::exceptionMCode( int exceptionId )
//{
//    for ( int i = 0; i < sizeof_array(_exceptions_); i++ )
//    {
//        if ( _exceptions_[i].id == exceptionId )
//        { return _exceptions_[i].mCode; }
//    }

//    Q_ASSERT( false );
//    return 0;
//}
//int eventViewer::exceptionSCode( int exceptionId )
//{
//    for ( int i = 0; i < sizeof_array(_exceptions_); i++ )
//    {
//        if ( _exceptions_[i].id == exceptionId )
//        { return _exceptions_[i].sCode; }
//    }

//    Q_ASSERT( false );
//    return 0;
//}

int eventViewer::exceptionIndex( const QString &str )
{
    for( int i = 0; i < sizeof_array(_exceptions_); i++ )
    {
        if ( _exceptions_[i].str == str )
        { return i; }
    }

//    Q_ASSERT( false );
    return 0;
}

eventViewer::eventViewer( receiveCache *pIntSrc,
                          EventActionModel *pModel,
                          QWidget *parent) :
    QDialog(parent),
    ui(new Ui::eventViewer)
{
    Q_ASSERT( NULL != pIntSrc );
    Q_ASSERT( NULL != pModel );


    ui->setupUi(this);

//    ui->listWidget->addItem("a");
//    ui->listWidget->addItem("b");
//    ui->listWidget->addItem("c");

    //! set interrupt src
    m_pInterruptSrc = pIntSrc;

    //! set model
    ui->eventView->setModel( pModel );
    connect( pModel, SIGNAL(signal_data_changed()),
             this, SLOT(slot_exception_changed()) );

    //! set delegate
    QList<int> vals;
    for ( int i = 0; i < sizeof_array(_exceptions_); i++ )
    {
        mEventStringList<<_exceptions_[i].str;
        vals<<_exceptions_[i].id;
    }
    mEventDelegate.setItems( mEventStringList, vals );

    vals.clear();
    for ( int i = 0; i < sizeof_array(_exception_actions_); i++ )
    {
        mActionStringList<<_exception_actions_[i].str;
        vals<<_exception_actions_[i].id;
    }
    mActionDelegate.setItems( mActionStringList, vals );

    ui->eventView->setItemDelegateForColumn( 1, &mEventDelegate );
    ui->eventView->setItemDelegateForColumn( 2, &mActionDelegate );
}

eventViewer::~eventViewer()
{
    delete ui;
}

void eventViewer::slot_event( eventId id, frameData data )
{
    if ( ui->chkView->isChecked() )
    {
        //! remove the first item
        while( ui->listWidget->count() >= ui->spinBox->value() )
        {
            ui->listWidget->removeItemWidget( ui->listWidget->takeItem( 0 ) );
        }

        QString str;

        str = QString("ID:%1 Time:%2 FrameId:%3 Data:").arg( id ).arg( QDateTime::currentDateTime().toString("hh:mm:ss.zzz") ).arg( data.getFrameId(), 0, 16 );
        str.append( data.toHex(' ') );

        ui->listWidget->addItem(str);
        ui->listWidget->setCurrentRow( ui->listWidget->count() - 1 );
    }
}

//! disable all event then istall now
void eventViewer::slot_exception_changed()
{
    Q_ASSERT( NULL != m_pInterruptSrc );

    //! disable all
    frameEvent event;logDbg();
    for ( int i = 0; i < sizeof_array(_exceptions_); i++ )
    {
        event.setId( (eventId)_exceptions_[i].id );logDbg();
        event.setMainSubCode( _exceptions_[i].mCode,
                              _exceptions_[i].sCode,
                              _exceptions_[i].mPattern,
                              _exceptions_[i].len );
        m_pInterruptSrc->setFrameEventEnable( event, false );logDbg();

    }

    //! enable the current
    Q_ASSERT( NULL != ui->eventView->model() );
    EventActionModel *pEventActionModel;
    pEventActionModel = (EventActionModel*)ui->eventView->model();
    Q_ASSERT( NULL != pEventActionModel );

    Q_ASSERT( NULL != pEventActionModel->items() );
logDbg();

    int eIndex;
    foreach( EventAction *pAction, *pEventActionModel->items() )
    {
        Q_ASSERT( NULL != pAction );

        eIndex = exceptionIndex( pAction->event() );

        event.setId( (eventId)_exceptions_[eIndex].id );

        event.setMainSubCode( _exceptions_[eIndex].mCode,
                              _exceptions_[eIndex].sCode,
                              _exceptions_[eIndex].mPattern,
                              _exceptions_[eIndex].len );

        m_pInterruptSrc->setFrameEventEnable( event, pAction->enable() );
    }logDbg();
}

void eventViewer::on_btnExport_clicked()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("CSV (*.csv )") );
//    fDlg.selectFile( windowTitle() );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    QFile file( fDlg.selectedFiles().first() );

    if ( !file.open(QIODevice::WriteOnly) )
    { return; }

    QVariant var;
    QTextStream fileStream( &file );
    for ( int i = 0; i < ui->listWidget->count(); i++ )
    {
        var = ui->listWidget->item(i)->data( Qt::DisplayRole );

        fileStream<<var.toString()<<"\n";
    }

    file.close();
}

void eventViewer::on_btnClear_clicked()
{
    ui->listWidget->clear();
}

void eventViewer::on_spinBox_valueChanged(int arg1)
{
    //! remove the over items
    while( ui->listWidget->count() > arg1 )
    {
        ui->listWidget->removeItemWidget( ui->listWidget->takeItem( 0 ) );
    }
}

void eventViewer::on_btnEventExport_clicked()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("event action (*.evt)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    EventActionModel *pModel;
    pModel = ((EventActionModel*)ui->eventView->model());
    Q_ASSERT( NULL != pModel );
    int ret = pModel->save( fDlg.selectedFiles().first() );
    if ( ret == 0 )
    { }
    else
    { }
}

void eventViewer::on_btnEventLoad_clicked()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptOpen );
    fDlg.setNameFilter( tr("event action (*.evt)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    EventActionModel *pModel;
    pModel = ((EventActionModel*)ui->eventView->model());
    Q_ASSERT( NULL != pModel );
    int ret = pModel->load( fDlg.selectedFiles().first() );
    if ( ret == 0 )
    { /*pModel->endResetModel();*/ }
    else
    {}
}

void eventViewer::on_btnEventAdd_clicked()
{
    int curRow;

    //! current
    curRow = ui->eventView->currentIndex().row();
    logDbg()<<curRow;
    Q_ASSERT( NULL != ui->eventView->model() );

    ui->eventView->model()->insertRow( curRow + 1 );
    ui->eventView->setCurrentIndex( ui->eventView->model()->index( curRow + 1, 0 ) );
}

void eventViewer::on_btnEventDel_clicked()
{
    ui->eventView->model()->removeRow( ui->eventView->currentIndex().row() );
}

