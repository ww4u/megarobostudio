/****************************************************************************
** Meta object code from reading C++ file 'instmgr.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../instmgr.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'instmgr.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MegaDevice__INTRThread_t {
    QByteArrayData data[8];
    char stringdata0[70];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MegaDevice__INTRThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MegaDevice__INTRThread_t qt_meta_stringdata_MegaDevice__INTRThread = {
    {
QT_MOC_LITERAL(0, 0, 22), // "MegaDevice::INTRThread"
QT_MOC_LITERAL(1, 23, 9), // "sig_event"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 7), // "eventId"
QT_MOC_LITERAL(4, 42, 2), // "id"
QT_MOC_LITERAL(5, 45, 9), // "frameData"
QT_MOC_LITERAL(6, 55, 10), // "slot_event"
QT_MOC_LITERAL(7, 66, 3) // "dat"

    },
    "MegaDevice::INTRThread\0sig_event\0\0"
    "eventId\0id\0frameData\0slot_event\0dat"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MegaDevice__INTRThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   29,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    7,

       0        // eod
};

void MegaDevice::INTRThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        INTRThread *_t = static_cast<INTRThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sig_event((*reinterpret_cast< eventId(*)>(_a[1])),(*reinterpret_cast< frameData(*)>(_a[2]))); break;
        case 1: _t->slot_event((*reinterpret_cast< eventId(*)>(_a[1])),(*reinterpret_cast< frameData(*)>(_a[2]))); break;
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
        case 1:
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
            typedef void (INTRThread::*_t)(eventId , frameData );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&INTRThread::sig_event)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MegaDevice::INTRThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_MegaDevice__INTRThread.data,
      qt_meta_data_MegaDevice__INTRThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MegaDevice::INTRThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MegaDevice::INTRThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MegaDevice__INTRThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int MegaDevice::INTRThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void MegaDevice::INTRThread::sig_event(eventId _t1, frameData _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_MegaDevice__InstMgr_t {
    QByteArrayData data[1];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MegaDevice__InstMgr_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MegaDevice__InstMgr_t qt_meta_stringdata_MegaDevice__InstMgr = {
    {
QT_MOC_LITERAL(0, 0, 19) // "MegaDevice::InstMgr"

    },
    "MegaDevice::InstMgr"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MegaDevice__InstMgr[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void MegaDevice::InstMgr::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject MegaDevice::InstMgr::staticMetaObject = {
    { &instServer::staticMetaObject, qt_meta_stringdata_MegaDevice__InstMgr.data,
      qt_meta_data_MegaDevice__InstMgr,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MegaDevice::InstMgr::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MegaDevice::InstMgr::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MegaDevice__InstMgr.stringdata0))
        return static_cast<void*>(this);
    return instServer::qt_metacast(_clname);
}

int MegaDevice::InstMgr::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = instServer::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
