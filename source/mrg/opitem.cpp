#include "opitem.h"

OpItem::OpItem()
{

}

OpItem::~OpItem()
{

}

OpGroup::OpGroup()
{}


OpGroup::~OpGroup()
{
    qDeleteAll( mItems );
    mItems.clear();
}

OpLib::OpLib()
{}
OpLib::~OpLib()
{
    qDeleteAll( mGroups );
    mGroups.clear();
}

int OpLib::load( const QString &fileName )
{
    QFile file( fileName );

    if ( file.open( QIODevice::ReadOnly ) )
    {}
    else
    { return -1; }

    QXmlStreamReader reader( &file );

    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "groups" )
        {
            while( reader.readNextStartElement() )
            {

                OpGroup *pGroup;
                if ( reader.name() == "group" )
                {
                    pGroup = new OpGroup();
                    if ( NULL == pGroup )
                    { return -1; }

                    mGroups.append( pGroup );

                    while( reader.readNextStartElement() )
                    {
                        OpItem   *pItem;
                        if ( reader.name() == "name" )
                        {
                            pGroup->mName = reader.readElementText();
                        }
                        else if ( reader.name() == "item" )
                        {
                            pItem = new OpItem();
                            if ( NULL == pItem )
                            { return -1; }

                            pGroup->mItems.append( pItem );
                            while( reader.readNextStartElement() )
                            {
                                if ( reader.name() == "name" )
                                {
                                    pItem->mName = reader.readElementText();
                                }
                                else if ( reader.name() == "res" )
                                {
                                    pItem->mRes = reader.readElementText();
                                }
                                else
                                { reader.skipCurrentElement(); }
                            }
                        }
                        else
                        {
                            reader.skipCurrentElement();
                        }
                    }
                }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else
        { reader.skipCurrentElement(); }
    }
}
