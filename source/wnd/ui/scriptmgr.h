#ifndef SCRIPTMGR_H
#define SCRIPTMGR_H

#include <QtWidgets>

//#include "../../model/sinanjumotionitem.h"
#include "../../model/scriptmodel.h"
#include "../../model/roboscenemodel.h"

#include "../../model/megatablemodel.h"

namespace Ui {
class scriptMgr;
}

class scriptMgr : public QWidget
{
    Q_OBJECT

public:
    static MegaTableModel *newMotion( const QString &clsName );

public:
    explicit scriptMgr(QWidget *parent = 0);
    ~scriptMgr();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual QSize sizeHint() const;
public:
    void setExpand( bool b );

    void setName( const QString &name );
    QString getName();

    QString getPath();

    QString getFullName();

    int save( const QString &path, const QString &name );
    int load( const QString &path, const QString &name );
    void reNew( const QString &path, const QString &prjName );

    //! current file
    int saveActiveFile();
    int saveActiveFile( const QString &file );

    int openFile( const QString &path, const QString &file );

    int openMotionGroup( const QString &path, const QString &file );
    int openTpv( const QString &path, const QString &file );
    int openTp( const QString &path, const QString &file );
    int openScene( const QString &path, const QString &file );
    int openSetup( const QString &path, const QString &file );
    int openPy( const QString &path, const QString &file );

    //! save all
    //! prj + motion
    int saveAll();

    //! prompt
    void newFile( const QString &path, const QString &name );
    void newGroup( const QString &name );
    void removeCurrent();

    QString defaultName();

    QStringList sceneList( QStringList &pathList );
    QStringList sceneList( QStringList &pathList,
                           scriptGroup *pGroup );
    
    QStringList fileList( );
    QStringList fileList( QStringList &pathList, scriptGroup *pGroup );

    roboSceneModel *createScene( const QString &path,
                                 const QString &file
                                );

    void iterAllItems();

protected:
    void setupUi();
    void buildConnection();

    bool isExist( const QString &fullName );

Q_SIGNALS:
    void itemXActivated( mcModelObj* );
    void signal_scriptmgr_changed();
    void signal_prj_edited();

private Q_SLOTS:
    void on_scriptView_activated(const QModelIndex &index);

//    void slot_context_newfile();
    void slot_context_newgroup();
    void slot_context_remove();
public Q_SLOTS:
    void slot_context_import();

private:
    Ui::scriptMgr *ui;

    QMenu *m_pContextMenu;
    QMenu *m_pFileContextMenu;

    QAction *m_pNewGroupAction;
    QAction *m_pRemoveAction;
    QAction *m_pImportAction;

    scriptModel *m_pRootModel;   //! root group
    int mUntitleId;

    QFileSystemModel *m_pFileSysModel;
    QString mSelectedNameFilter;
};

#endif // SCRIPTMGR_H
