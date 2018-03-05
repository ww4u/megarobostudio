
#include "mainwindow.h"
#include "../../bus/filebus.h"

void MainWindow::slot_scene_changed()
{
    updateSceneRobot();
}

DeviceTree MainWindow::sceneRobotTree(
                            const QStringList &pathList,
                            const QStringList &fileList )
{
    //! size must be equal
    Q_ASSERT( pathList.size() == fileList.size() );

    DeviceTree devTree;
    for( int i = 0; i < pathList.size(); i++ )
    {logDbg()<<pathList[i]<<fileList[i];
        VRoboList roboList = sceneRobotList( pathList[i],
                                             fileList[i] );
        logDbg()<<roboList.size();
        //! has robot
        if ( roboList.size() > 0 )
        {
            VRoboList *pList = new VRoboList();
            Q_ASSERT( NULL != pList );
logDbg();
            *pList = roboList;
logDbg();
            MegaDevice::FILEBus *pBus = new MegaDevice::FILEBus();
            Q_ASSERT( NULL != pBus );logDbg();
            pBus->setName( fileList[i] );
logDbg();
            pList->attachBus( pBus );
logDbg();
            devTree.append( pList );logDbg();
        }
        else
        {
        }
    }

    return devTree;
}

//! get robo from scene list
VRoboList MainWindow::sceneRobotList(
                          const QString &path,
                          const QString &name )
{
    QString refPath;
    refPath = path;

    //! load scene
    roboScene scene;
    QString fullPath;
    fullPath = path + "/" + name;
    logDbg()<<fullPath;

    VRoboList complexRoboList;
    //! load the file
    int ret = scene.load( fullPath, refPath );
    if ( ret != 0 )
    { return complexRoboList; }
logDbg();
    //! get robo list
    QList<VRobot *> roboList = scene.complexRobotList();
    foreach( VRobot *pRobo, roboList )
    {logDbg();
        Q_ASSERT( NULL != pRobo);

        //! detach from the widget
        pRobo->setGc( false );
        complexRoboList.append( pRobo );
    }
logDbg();
    return complexRoboList;
}

void MainWindow::updateSceneRobot()
{
    QStringList fileList;
    QStringList pathList;

    Q_ASSERT( NULL != m_pScriptMgr );
    fileList = m_pScriptMgr->sceneList( pathList );

    DeviceTree devTree = sceneRobotTree( pathList, fileList );

    Q_ASSERT( NULL != mMcModel.m_pInstMgr );
    mMcModel.m_pInstMgr->clearFileDeviceTree();
    mMcModel.m_pInstMgr->appendFileDeviceTree( devTree );
logDbg()<<devTree.size();
    m_pDeviceMgr->updateUi();
}


