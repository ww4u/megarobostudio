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
      MRQ_mc_SYSTEM, MRQ_sc_SYSTEM_EVENTCODE_Q,
      6,
      { -1, 4, 0, -1, -1, -1,}
    },

    { QStringLiteral("Over Distance"),
      event_over_distance,
      MRQ_mc_SYSTEM, MRQ_sc_SYSTEM_EVENTCODE_Q,
      6,
      { -1, 2, 0, -1, -1, -1,}
    },
    { QStringLiteral("Over Angle"),
      event_over_angle,
      MRQ_mc_SYSTEM, MRQ_sc_SYSTEM_EVENTCODE_Q,
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
    { QStringLiteral("None"), e_device_action_none },
    { QStringLiteral("Stop"), e_device_action_stop },
    { QStringLiteral("Prompt"), e_device_action_prompt },
    { QStringLiteral("Prompt+Stop"), e_device_action_prompt_stop },
};

int eventViewer::exceptionCode( const QString &str )
{
    for( int i = 0; i < sizeof_array(_exceptions_); i++ )
    {
        if ( str_is( _exceptions_[i].str,str) )
        { return _exceptions_[i].id; }
    }

    return _exceptions_[0].id;
}
int eventViewer::actionCode( const QString &str )
{
    for( int i = 0; i < sizeof_array(_exception_actions_); i++ )
    {
        if ( str_is( _exception_actions_[i].str,str) )
        { return _exception_actions_[i].id; }
    }

    return _exception_actions_[0].id;
}

int eventViewer::exceptionIndex( const QString &str )
{
    for( int i = 0; i < sizeof_array(_exceptions_); i++ )
    {
        if ( str_is( _exceptions_[i].str, str ) )
        { return i; }
    }

    return 0;
}

eventViewer::eventViewer(
                          EventActionModel *pModel,
                          QWidget *parent) :
    QDialog(parent),
    ui(new Ui::eventViewer)
{
    Q_ASSERT( NULL != pModel );

    ui->setupUi(this);

    m_pMcModel = NULL;

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

    ui->eventView->setItemDelegateForColumn( 0, &mBoolDelegate );
    ui->eventView->setItemDelegateForColumn( 1, &mEventDelegate );
    ui->eventView->setItemDelegateForColumn( 2, &mActionDelegate );

    //! changed
    slot_exception_changed();
}

eventViewer::~eventViewer()
{
    delete ui;
}

void eventViewer::setMcModel( mcModel *pObj )
{
    Q_ASSERT( NULL != pObj );
    m_pMcModel = pObj;
}

void eventViewer::slot_event( eventId id, frameData data )
{
    //! bypass status event
    if ( id == event_status )
    { return; }

    if ( ui->chkView->isChecked() )
    {
        //! remove the first item
        while( ui->listWidget->count() >= ui->spinBox->value() )
        {
            ui->listWidget->removeItemWidget( ui->listWidget->takeItem( 0 ) );
        }

        QString str;
        str = QString("ID:%1,Time:%2,Bus:%3,FrameId:%4,Data:").arg( id )
                                                              .arg( QDateTime::currentDateTime().toString("hh:mm:ss.zzz") )
                                                              .arg( data.devId() )
                                                              .arg( data.frameId(), 0, 16 );
        str.append( data.toHex(' ') );

        ui->listWidget->addItem(str);
        ui->listWidget->setCurrentRow( ui->listWidget->count() - 1 );
    }

    //! log the file
    if ( ui->chkLog->isChecked() )
    {
        do
        {
            Q_ASSERT( NULL != m_pMcModel );
            QFile file( m_pMcModel->mSysPref.mEventLogFile );
            if ( !file.open( QIODevice::Append) )
            {
                sysError( tr("Fail on file"), m_pMcModel->mSysPref.mEventLogFile );
                break;
            }

            //! log the file
            QString str;
            str = QString("ID:%1,Time:%2,FrameId:%3,Data:").arg( id ).arg( QDateTime::currentDateTime().toString("hh:mm:ss.zzz") ).arg( data.frameId(), 0, 16 );
            str.append( data.toHex(' ') );

            if ( str.length() != file.write( str.toUtf8().data(), str.length() ) )
            {
                sysError( tr("Fail on file"), m_pMcModel->mSysPref.mEventLogFile );
            }

        }while( 0 );

    }
}

//! disable all event then istall now
void eventViewer::slot_exception_changed()
{
    //! disable all
    frameEvent event;
    for ( int i = 0; i < sizeof_array(_exceptions_); i++ )
    {
        event.setId( (eventId)_exceptions_[i].id );logDbg();
        event.setMainSubCode( _exceptions_[i].mCode,
                              _exceptions_[i].sCode,
                              _exceptions_[i].mPattern,
                              _exceptions_[i].len );
        receiveCache::setFrameEventEnable( event, false );
    }

    //! enable the current
    Q_ASSERT( NULL != ui->eventView->model() );
    EventActionModel *pEventActionModel;
    pEventActionModel = (EventActionModel*)ui->eventView->model();
    Q_ASSERT( NULL != pEventActionModel );

    Q_ASSERT( NULL != pEventActionModel->items() );

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

        receiveCache::setFrameEventEnable( event, pAction->enable() );
    }

    emit signal_model_changed();
}

void eventViewer::on_btnExport_clicked()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("CSV (*.csv )") );
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

        fileStream<<var.toString()<<ROW_SEP;
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

