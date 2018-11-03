#ifndef MDATAROW_H
#define MDATAROW_H

#include <QtCore>

#define line_seperator    "\n"

class MDataRow : public QStringList
{
public:
    MDataRow();

    MDataRow( const QStringList & );
    MDataRow( const QString & );

    MDataRow &operator=( const QStringList & );
    MDataRow &operator=( const QString & );

public:
    void dbgShow();
};

class MDataSection
{
public:
    static QMap<QString, QVariant::Type > _keyw_types;

public:
    MDataSection( int sec = 0 );
    ~MDataSection();

public:
    void setModel( const QString &model );
    QString model();

    void setHeaders( const QStringList &header );
    QStringList headers();
    QString columnName( int c );

    void setSection( int sec );
    int section();

    bool lineIn( QString &ary );
    bool lineOut( QTextStream &stream );

    bool addRow( const QString &str1, QChar sep='/' );
    int rows();

    QVariant cell( int r, int c );
    QString rawCell( int r, int c );

    bool cellValue( int r, int c,
                    bool &v, bool def,
                    bool bOv=true );
    bool cellValue( int r, int c,
                    float &v, float def,
                    bool bOv=true );
    bool cellValue( int r, int c,
                    double &v, double def,
                    bool bOv=true );
    bool cellValue( int r, int c,
                    int &v, int def,
                    bool bOv=true );
    bool cellValue( int r, int c,
                    QString &v, const QString &def,
                    bool bOv=true );

public:
    void dbgShow();

public:
    QString mModel;
    QStringList mHeaders;

    int mSection;
    QList< MDataRow *> mRows;
};

#endif // MDATAROW_H
