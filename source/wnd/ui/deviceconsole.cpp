#include "deviceconsole.h"
#include "ui_deviceconsole.h"

#define LUT_COUNT   10

deviceConsole::deviceConsole(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deviceConsole)
{
    ui->setupUi(this);

    setAttribute( Qt::WA_DeleteOnClose );

    connect( this, SIGNAL(signal_classname_changed()),
             this, SLOT(slot_classname_changed()) );

    m_pScpiShell = NULL;
}

deviceConsole::~deviceConsole()
{
    delete ui;
}

void deviceConsole::setShell( scpiShell *pShell )
{
    Q_ASSERT( NULL != pShell );
    m_pScpiShell = pShell;
}
scpiShell *deviceConsole::getShell()
{
    return m_pScpiShell;
}

void deviceConsole::setMrq( bool b )
{

}

void deviceConsole::setClassName( const QString &cls )
{
    mClassName = cls;

    emit signal_classname_changed();

}
QString deviceConsole::className()
{ return mClassName; }

void deviceConsole::on_btnWrite_clicked()
{   
    QString str = ui->comboBox->currentText();

    doWrite( str );

    //! remove the item
    for( int i = 0; i < ui->comboBox->count(); i++ )
    {
        //! equal
        if ( ui->comboBox->itemText(i) == str )
        {
            ui->comboBox->removeItem(i);
            break;
        }
    }

    //! insert again
    ui->comboBox->insertItem( 0, str );
    //! select current
    ui->comboBox->setCurrentIndex( 0 );

    //! limit the lut
    while( ui->comboBox->count() > LUT_COUNT )
    {
        ui->comboBox->removeItem( ui->comboBox->count() - 1 );
    }
}

void deviceConsole::on_btnRead_clicked()
{
    Q_ASSERT( NULL != m_pScpiShell );

    int retSize = m_pScpiShell->size();
    if ( retSize > 0 )
    {
        char *pOut = new char[ retSize ];
        Q_ASSERT( NULL != pOut );
        if ( retSize == m_pScpiShell->read( pOut, retSize ) )
        {
            ui->listWidget->addItem( QByteArray( pOut, retSize ).trimmed() );
            ui->listWidget->setCurrentRow( ui->listWidget->count() - 1 );
        }
        else
        {}

        delete []pOut;
    }
}

void deviceConsole::slot_device_changed()
{
    close();    //! as the mgr changed
}

void deviceConsole::slot_classname_changed()
{

    mCommandsetFullName = QCoreApplication::applicationDirPath()
            + QDir::separator()
            + QStringLiteral("doc")
            + QDir::separator()
            + mClassName
            + QStringLiteral("_commandset.md");
    mCommandsetFullName.replace("/","\\");

    ui->btnCmdSet->setEnabled( QFile::exists( mCommandsetFullName ) );
}

void deviceConsole::on_btnRst_clicked()
{
    doWrite( "ROTATE 0,0,0.1,360" );
}

void deviceConsole::on_btnCls_clicked()
{
    doWrite( "ROTATE 0,0,0.1,-360" );
}

void deviceConsole::on_comboBox_editTextChanged(const QString &arg1)
{
    QString str = arg1;
    str.trimmed();
    ui->btnWrite->setEnabled( str.length() > 0 );
}

void deviceConsole::doWrite( const QString &str )
{
    //! add to list
    ui->listWidget->addItem( str  );
    ui->listWidget->setCurrentRow( ui->listWidget->count()-1 );

    Q_ASSERT( NULL != m_pScpiShell );

    //! enc str
    QString sendStr;
    sendStr = str + "\r\n";

    //! send
    m_pScpiShell->write( sendStr.toLatin1().data(),
                         sendStr.length() );
}

void deviceConsole::on_comboBox_currentTextChanged(const QString &arg1)
{
    ui->btnWrite->setEnabled( arg1.trimmed().length() > 0 );
}

void deviceConsole::on_btnCmdSet_clicked()
{
    QStringList args;
    args<<mCommandsetFullName;
    QProcess::execute( "explorer.exe", args );
}
