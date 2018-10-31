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

    //! set font
    ui->plainTextEdit->setFont( QFont( m_pmcModel->mSysPref.mFontFamily,
                                       m_pmcModel->mSysPref.mPointSize ) );

    QString fullName = pObj->getPath() + QDir::separator() + pObj->getName();
    fullName = QDir::toNativeSeparators( fullName );
    //! reload the file
    reloadFile( fullName );
}

bool scriptEditor::matchModelObj(mcModelObj *pObj)
{
    Q_ASSERT( NULL != pObj );

    if ( pObj->getType() != m_pModelObj->getType() )
    { return false; }

    //! name match
    if ( !str_is( m_pModelObj->getName(), pObj->getName() ) )
    { return false; }

    //! path match
    if ( !str_is( m_pModelObj->getPath(), pObj->getPath() ) )
    { return false; }

    return true;
}

int scriptEditor::save( QString &outFileName )
{
    outFileName = getModelObj()->getPath() + QDir::separator() + getModelObj()->getName();
    outFileName = QDir::toNativeSeparators( outFileName );

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
