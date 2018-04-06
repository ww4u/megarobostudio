#include "scripteditor.h"
#include "ui_scripteditor.h"

scriptEditor::scriptEditor(QWidget *parent) :
    modelView(parent),
    ui(new Ui::scriptEditor)
{
    ui->setupUi(this);

    mFilePattern<<script_desc<<script_ext;

    m_pHighLigter = NULL;

    m_pHighLigter = new Highlighter( ui->plainTextEdit->document() );
}

scriptEditor::~scriptEditor()
{
    delete ui;
}

void scriptEditor::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    logDbg()<<pObj->getPath()<<pObj->getName();

    //! reload the file
    reloadFile( pObj->getPath() + QDir::separator() + pObj->getName() );
}

bool scriptEditor::matchModelObj(mcModelObj *pObj)
{
    Q_ASSERT( NULL != pObj );

    if ( pObj->getType() != m_pModelObj->getType() )
    { return false; }

    //! name match
    if ( QString::compare( m_pModelObj->getName(), pObj->getName(),Qt::CaseInsensitive ) != 0 )
    { return false; }

    //! path match
    if ( QString::compare( m_pModelObj->getPath(), pObj->getPath(),Qt::CaseInsensitive ) != 0 )
    { return false; }

    return true;
}

int scriptEditor::save( QString &outFileName )
{
    outFileName = getModelObj()->getPath() + QDir::separator() + getModelObj()->getName();

    QFile file( outFileName );
    if ( !file.open( QIODevice::WriteOnly ) )
    { return ERR_FILE_OPEN_FAIL; }

    file.write( ui->plainTextEdit->toPlainText().toLatin1() );

    file.close();

    return ERR_NONE;
}
int scriptEditor::saveAs( QString &outFileName )
{
    QFile file( outFileName );
    if ( !file.open( QIODevice::WriteOnly ) )
    { return ERR_FILE_OPEN_FAIL; }

    file.write( ui->plainTextEdit->toPlainText().toLatin1() );

    file.close();

    return ERR_NONE;
}

void scriptEditor::reloadFile( const QString &fileName )
{
    QFile file( fileName );
    if ( !file.open( QIODevice::ReadOnly ) )
    { return; }

    QByteArray ary = file.readAll();

    ui->plainTextEdit->setPlainText( QString(ary) );

    file.close();
}
