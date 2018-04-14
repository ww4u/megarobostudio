#include "scriptfile.h"

scriptNode::scriptNode()
{
    mNodeType = node_group;
    mSkip = 0;
    m_pParent = NULL;

    //! self is a model obj
    mcModelObj::set( mcModelObj::model_motion_file, this );
}

scriptNode::~scriptNode()
{
    clear();
}

void scriptNode::clear()
{
    delete_all( mNodeList );
}

void scriptNode::setParent( scriptNode *pPar )
{
    Q_ASSERT( NULL != pPar );
    m_pParent = pPar;
}
scriptNode *scriptNode::getParent()
{ return m_pParent; }

void scriptNode::setNodeType( scriptNode::nodeType nodeType )
{ mNodeType = nodeType; }
scriptNode::nodeType scriptNode::getNodeType()
{ return mNodeType; }

void scriptNode::setSkip( int b )
{ mSkip = b; }
int scriptNode::getSkip()
{ return mSkip; }

void scriptNode::appendNode( scriptNode *pNode )
{
    Q_ASSERT( NULL != pNode );
    mNodeList.append( pNode );

    pNode->setParent( this );
}
void scriptNode::insertNode( int row, scriptNode *pNode )
{
    Q_ASSERT( NULL != pNode );

    if ( row < 0 )
    {
        mNodeList.prepend( pNode );
        pNode->setParent( this );
    }
    else if ( row >= mNodeList.size() )
    {
        appendNode( pNode );
    }
    else
    {
        mNodeList.insert( row, pNode );
        pNode->setParent( this );
    }
}
void scriptNode::remove()
{
    if ( m_pParent != NULL )
    {
        m_pParent->mNodeList.removeAll( this );
    }
}

scriptNode *scriptNode::child( int row )
{
    return mNodeList.value( row, NULL );
}

int scriptNode::childNumber()
{
    if ( m_pParent )
    {
        return m_pParent->mNodeList.indexOf( this );
    }

    return 0;
}

int scriptNode::childCount()
{
    return mNodeList.size();
}

//! file
scriptFile::scriptFile( const QString &file )
{
    m_pParent = NULL;

    setName( file );

    setNodeType( scriptNode::node_file );
}

scriptFile::~scriptFile()
{
}

int scriptFile::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement( "file");
    writer.writeTextElement( "name", mName );
    writer.writeTextElement( "path", mPath );
    writer.writeTextElement( "skip", QString::number( mSkip ) );
    writer.writeEndElement();
    return 0;
}
int scriptFile::serialIn( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "name" )
        { mName = reader.readElementText(); }

        //! ref to the project
        else if ( reader.name() == "path" )
        { mPath = reader.readElementText(); }

        else if ( reader.name() == "skip" )
        { mSkip = reader.readElementText().toInt(); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}
