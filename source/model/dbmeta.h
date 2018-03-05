#ifndef DBMETA_H
#define DBMETA_H

#include <QtCore>

class dbMeta
{
public:
    dbMeta();
public:
    bool mbUpload;
    QString mHostName;
    QString mDbName;
    QString mTableName;
    QString mUserName;
    QString mPassword;
};

#endif // DBMETA_H
