#ifndef SCPISHELL_H
#define SCPISHELL_H

#include <QtCore>
#include "../model/mcmodelobj.h"

#define DEF_IN_SIZE     1024
#define DEF_OUT_SIZE    1024
#define DEF_ERR_SIZE    16

#if defined(EXPORT_LIBRARY)
#  define SCPI_SHELL_EXPORT Q_DECL_EXPORT
#else
#  define SCPI_SHELL_EXPORT Q_DECL_IMPORT
#endif

#undef SCPI_SHELL_EXPORT
#define SCPI_SHELL_EXPORT

SCPI_SHELL_EXPORT int setCnt( int cnt );
SCPI_SHELL_EXPORT int getCnt( );

struct outNode
{
    int mDataSize;      //! in bytes for data
    char mDataBuf[4];
};

//! get full size
#define node_size( node )   ( node->mDataSize + sizeof(outNode) - 4 )

//! expect the node size from len
#define node_cap( len )     ( sizeof(outNode) + (len) - ( (len) > 4 ? 4 : (len) ) )

#define attach_node( pNode, data, len ) pNode->mDataSize = len;\
                                        memcpy( pNode->mDataBuf, data, len );

class SCPI_SHELL_EXPORT scpiShell : public mcModelObj
{
public:
    scpiShell();
    virtual ~scpiShell();

public:
    virtual void setObjPara( const QString &name, void *pPara );

public:
    int open( int inSize = DEF_IN_SIZE,
                        int outSize = DEF_OUT_SIZE,
                        int errSize = DEF_ERR_SIZE );
    int close();

    int write( const char *p, int len );
    int write( const char *p );
    int size();
    int read( char *pOut, int len );
    int error();

protected:
    virtual const void* loadScpiCmd();
    virtual const char* getIdn1();
    virtual const char* getIdn2();
    virtual const char* getIdn3();
    virtual const char* getIdn4();

public:
    //! common api
    int SCPI_Write(void * context, const char * data, size_t len);
    int SCPI_Flush(void * context);

    int SCPI_Error(void * context, int_fast16_t err);
    int SCPI_Control(void * context, int ctrl, unsigned short val);
    int SCPI_Reset( void *context );

    void gc();

public:
//    void setName( const QString &name );
//    QString getName();

public:
//    scpi_interface_t scpi_interface;
//    char *p_scpi_input_buffer;
//    scpi_error_t *p_scpi_error_queue_data;
//    scpi_t scpi_context;

    void *p_scpi_interface;
    char *p_scpi_input_buffer;
    void *p_scpi_error_queue_data;
    void *p_scpi_context;
    char *p_scpi_output_buffer;

protected:
    int mIntputBufferSize;
    int mOutputBufferSize;
    int mErrQueueSize;

    int mOutWritePos, mOutReadPos;

    void *m_pUserPara;

//    QString mName;      //! device name

};

#endif // SCPISHELL_H
