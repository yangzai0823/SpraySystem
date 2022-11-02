/****************************************************************************
** Meta object code from reading C++ file 'mainprocess.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../SpraySystem/Bussiness/mainprocess.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainprocess.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainProcess_t {
    QByteArrayData data[11];
    char stringdata0[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainProcess_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainProcess_t qt_meta_stringdata_MainProcess = {
    {
QT_MOC_LITERAL(0, 0, 11), // "MainProcess"
QT_MOC_LITERAL(1, 12, 16), // "begintraj_Singal"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 4), // "data"
QT_MOC_LITERAL(4, 35, 16), // "recevieData_Slot"
QT_MOC_LITERAL(5, 52, 9), // "traj_Slot"
QT_MOC_LITERAL(6, 62, 5), // "varmc"
QT_MOC_LITERAL(7, 68, 6), // "varrbt"
QT_MOC_LITERAL(8, 75, 12), // "imgData_Slot"
QT_MOC_LITERAL(9, 88, 7), // "imgData"
QT_MOC_LITERAL(10, 96, 17) // "getTrajParam_Slot"

    },
    "MainProcess\0begintraj_Singal\0\0data\0"
    "recevieData_Slot\0traj_Slot\0varmc\0"
    "varrbt\0imgData_Slot\0imgData\0"
    "getTrajParam_Slot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainProcess[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   42,    2, 0x08 /* Private */,
       5,    2,   45,    2, 0x08 /* Private */,
       8,    1,   50,    2, 0x08 /* Private */,
      10,    0,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariant,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant,    3,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant,    6,    7,
    QMetaType::Void, QMetaType::QByteArray,    9,
    QMetaType::Void,

       0        // eod
};

void MainProcess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainProcess *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->begintraj_Singal((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 1: _t->recevieData_Slot((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 2: _t->traj_Slot((*reinterpret_cast< QVariant(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 3: _t->imgData_Slot((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 4: _t->getTrajParam_Slot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainProcess::*)(QVariant );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainProcess::begintraj_Singal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainProcess::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MainProcess.data,
    qt_meta_data_MainProcess,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainProcess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainProcess::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainProcess.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MainProcess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void MainProcess::begintraj_Singal(QVariant _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
