#include "scriptgroup.h"
#include "motiongroup.h"

scriptGroup::scriptGroup( const QString &name )
{
    m_pParent = NULL;

    setName( name );

    setNodeType( scriptNode::node_group );

    mcModelObj::set( mcModelObj::model_motion_group, this );
}

scriptGroup::~scriptGroup()
{
}

int scriptGroup::save( const QString &str )
{
    QFile file(str);

    if ( !file.open( QIODevice::WriteOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamWriter writer( &file );

    writer.writeStartDocument();

    serialOut( writer );

//    writer.writeStartElement("prj");

//    writer.writeTextElement("name", mName );

//    foreach( scriptNode *pNode, mNodeList )
//    {
//        pNode->serialOut( writer );
//    }

//    writer.writeEndElement();

    writer.writeEndDocument();

    return 0;
}
int scriptGroup::load( const QString &str )
{
    QFile file(str);

    if ( !file.open( QIODevice::ReadOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamReader reader( &file );

    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "group" )
        {
            serialIn( reader );
        }

//        if ( reader.name() == "prj" )                   //! top level
//        {
//            while( reader.readNextStartElement() )
//            {
//                if ( reader.name() == "name" )          //! project name
//                { mName = reader.readElementText(); }

//                //! group
//                if ( reader.name() == "group" )         //! sub group
//                {
//                    scriptGroup *pGroup;
//                    pGroup = new scriptGroup();
//                    Q_ASSERT( NULL != pGroup );
//                    pGroup->serialIn( reader );

//                    appendNode( pGroup );
//                }

//                //! a new file
//                if ( reader.name() == "file" )          //! sub file
//                {
//                    scriptFile *pFile;
//                    pFile = new scriptFile();
//                    Q_ASSERT( NULL != pFile );
//                    pFile->serialIn( reader );

//                    appendNode( pFile );
//                }
//            }
//        }
    }

    dbgPrint();

    return 0;
}

int scriptGroup::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement("group");
    writer.writeTextElement( "name", mName );
    writer.writeTextElement( "skip", QString::number( mSkip ) );

    //! child
    foreach( scriptNode *pNode, mNodeList )
    {
        Q_ASSERT( NULL != pNode );
        pNode->serialOut( writer );
    }

    writer.writeEndElement();

    return 0;
}
int scriptGroup::serialIn( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "name" )
        { mName = reader.readElementText(); logDbg()<<mName; }
        if ( reader.name() == "skip" )
        { mSkip = reader.readElementText().toInt(); }

        if ( reader.name() == "file" )          //! sub file
        {
            scriptFile *pFile;

            pFile = new scriptFile();
            Q_ASSERT( NULL != pFile );
            pFile->serialIn( reader );logDbg()<<pFile->getName();

            appendNode( pFile );
        }
        if( reader.name() == "group" )          //! sub group
        {
            scriptGroup *pGroup;
            pGroup = new scriptGroup();
            Q_ASSERT( NULL != pGroup );
            pGroup->serialIn( reader );

            appendNode( pGroup );
            logDbg()<<pGroup->getName();
            logDbg()<<pGroup->childCount();
        }
    }

    return 0;
}

int scriptGroup::saveAll( const QString &dir )
{
    foreach( scriptNode *pNode, mNodeList )
    {
        if ( pNode->getNodeType() == scriptNode::node_group )
        {
            dynamic_cast<scriptGroup*>( pNode )->saveAll( dir );
        }
        else if ( pNode->getNodeType() == scriptNode::node_file )
        {
            dynamic_cast<scriptFile*>( pNode )->save( dir + QDir::separator() + pNode->getPath() + QDir::separator() + pNode->getName() );
        }
        else
        {}

    }

    return 0;
}

void scriptGroup::dbgPrint()
{
    iterGroup( this );
}

void scriptGroup::iterGroup( scriptGroup *pGp )
{
    Q_ASSERT( NULL != pGp );
    if ( pGp->getParent() != NULL )
    { logDbg()<<"group:"<<pGp->name()<<pGp->getParent()->getName(); }
    else
    { logDbg()<<"group:"<<pGp->name(); }
    foreach( scriptNode* pNode, pGp->mNodeList )
    {
        Q_ASSERT( NULL != pNode );

        if ( pNode->getNodeType() == node_group )
        {
            ((scriptGroup*)pNode)->iterGroup( (scriptGroup *)pNode );
        }
        else
        {
            logDbg()<<"file:"<<pNode->getName()<<pNode->getParent()->getName();
        }
    }

}


