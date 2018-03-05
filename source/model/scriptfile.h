#ifndef SCRIPTFILE_H
#define SCRIPTFILE_H

#include <QtCore>

#include "motionitem.h"
#include "mcmodelobj.h"
#include "../intf/iserial.h"

class scriptNode : public mcModelObj, public ISerial
{
public:
    enum nodeType
    {
        node_group,
        node_file,
    };

public:
    scriptNode();
    virtual ~scriptNode();
    void clear();

public:
    void setParent( scriptNode *pPar );
    scriptNode *getParent();

    void setNodeType( scriptNode::nodeType node );
    nodeType getNodeType();

    void setSkip( int skip );
    int getSkip();

    void appendNode( scriptNode *pNode );
    void insertNode( int row, scriptNode *pNode );
    void remove();

    int childCount();
    scriptNode *child( int row );   //! group then file

    int childNumber();
protected:
    scriptNode *m_pParent;
    nodeType mNodeType;

    int mSkip;
                                    //! many nodes in group
    QList< scriptNode* > mNodeList;
};

Q_DECLARE_METATYPE(scriptNode)
Q_DECLARE_METATYPE(scriptNode*)

class scriptFile : public scriptNode
{
public:
    scriptFile( const QString &file="" );
    ~scriptFile();

public:
    virtual int serialOut( QXmlStreamWriter &writer );
    virtual int serialIn( QXmlStreamReader &reader );

};

Q_DECLARE_METATYPE(scriptFile)
Q_DECLARE_METATYPE(scriptFile*)

#endif // SCRIPTFILE_H
