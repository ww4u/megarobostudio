/****************************************************************************
** Meta object code from reading C++ file 'receivecache.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../receivecache.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'receivecache.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_receiveCache_t {
    QByteArrayData data[6];
    char stringdata0[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_receiveCache_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_receiveCache_t qt_meta_stringdata_receiveCache = {
    {
QT_MOC_LITERAL(0, 0, 12), // "receiveCache"
QT_MOC_LITERAL(1, 13, 9), // "sig_event"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 7), // "eventId"
QT_MOC_LITERAL(4, 32, 2), // "id"
QT_MOC_LITERAL(5, 35, 9) // "frameData"

    },
    "receiveCache\0sig_event\0\0eventId\0id\0"
    "frameData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_receiveCache[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    2,

       0        // eod
};

void receiveCache::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        receiveCache *_t = static_cast<receiveCache *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sig_event((*reinterpret_cast< eventId(*)>(_a[1])),(*reinterpret_cast< frameData(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< eventId >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< frameData >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (receiveCache::*_t)(eventId , frameData );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&receiveCache::sig_event)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject receiveCache::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_receiveCache.data,
      qt_meta_data_receiveCache,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *receiveCache::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *receiveCache::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_receiveCache.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int receiveCache::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void receiveCache::sig_event(eventId _t1, frameData _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
