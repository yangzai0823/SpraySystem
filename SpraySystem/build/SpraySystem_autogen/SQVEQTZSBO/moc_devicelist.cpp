/****************************************************************************
** Meta object code from reading C++ file 'devicelist.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Forms/Devices/devicelist.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'devicelist.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DeviceList_t {
    QByteArrayData data[22];
    char stringdata0[409];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DeviceList_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DeviceList_t qt_meta_stringdata_DeviceList = {
    {
QT_MOC_LITERAL(0, 0, 10), // "DeviceList"
QT_MOC_LITERAL(1, 11, 13), // "client_Singal"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 3), // "msg"
QT_MOC_LITERAL(4, 30, 24), // "on_btn_RobotSend_clicked"
QT_MOC_LITERAL(5, 55, 25), // "on_btn_RobotReset_clicked"
QT_MOC_LITERAL(6, 81, 24), // "on_btn_RobotSave_clicked"
QT_MOC_LITERAL(7, 106, 27), // "on_btn_RobotConnect_clicked"
QT_MOC_LITERAL(8, 134, 25), // "on_btn_CameraSave_clicked"
QT_MOC_LITERAL(9, 160, 33), // "on_lst_Robot_itemSelectionCha..."
QT_MOC_LITERAL(10, 194, 34), // "on_lst_Camera_itemSelectionCh..."
QT_MOC_LITERAL(11, 229, 22), // "on_btn_PLCSave_clicked"
QT_MOC_LITERAL(12, 252, 25), // "on_btn_PLCConnect_clicked"
QT_MOC_LITERAL(13, 278, 22), // "on_btn_PLCSend_clicked"
QT_MOC_LITERAL(14, 301, 27), // "on_tabWidget_currentChanged"
QT_MOC_LITERAL(15, 329, 5), // "index"
QT_MOC_LITERAL(16, 335, 16), // "receiveData_Slot"
QT_MOC_LITERAL(17, 352, 7), // "varData"
QT_MOC_LITERAL(18, 360, 18), // "newConnection_Slot"
QT_MOC_LITERAL(19, 379, 2), // "ip"
QT_MOC_LITERAL(20, 382, 4), // "port"
QT_MOC_LITERAL(21, 387, 21) // "on_btn_MCSave_clicked"

    },
    "DeviceList\0client_Singal\0\0msg\0"
    "on_btn_RobotSend_clicked\0"
    "on_btn_RobotReset_clicked\0"
    "on_btn_RobotSave_clicked\0"
    "on_btn_RobotConnect_clicked\0"
    "on_btn_CameraSave_clicked\0"
    "on_lst_Robot_itemSelectionChanged\0"
    "on_lst_Camera_itemSelectionChanged\0"
    "on_btn_PLCSave_clicked\0on_btn_PLCConnect_clicked\0"
    "on_btn_PLCSend_clicked\0"
    "on_tabWidget_currentChanged\0index\0"
    "receiveData_Slot\0varData\0newConnection_Slot\0"
    "ip\0port\0on_btn_MCSave_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DeviceList[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   92,    2, 0x08 /* Private */,
       5,    0,   93,    2, 0x08 /* Private */,
       6,    0,   94,    2, 0x08 /* Private */,
       7,    0,   95,    2, 0x08 /* Private */,
       8,    0,   96,    2, 0x08 /* Private */,
       9,    0,   97,    2, 0x08 /* Private */,
      10,    0,   98,    2, 0x08 /* Private */,
      11,    0,   99,    2, 0x08 /* Private */,
      12,    0,  100,    2, 0x08 /* Private */,
      13,    0,  101,    2, 0x08 /* Private */,
      14,    1,  102,    2, 0x08 /* Private */,
      16,    1,  105,    2, 0x08 /* Private */,
      18,    2,  108,    2, 0x08 /* Private */,
      21,    0,  113,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::QVariant,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   19,   20,
    QMetaType::Void,

       0        // eod
};

void DeviceList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DeviceList *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->client_Singal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->on_btn_RobotSend_clicked(); break;
        case 2: _t->on_btn_RobotReset_clicked(); break;
        case 3: _t->on_btn_RobotSave_clicked(); break;
        case 4: _t->on_btn_RobotConnect_clicked(); break;
        case 5: _t->on_btn_CameraSave_clicked(); break;
        case 6: _t->on_lst_Robot_itemSelectionChanged(); break;
        case 7: _t->on_lst_Camera_itemSelectionChanged(); break;
        case 8: _t->on_btn_PLCSave_clicked(); break;
        case 9: _t->on_btn_PLCConnect_clicked(); break;
        case 10: _t->on_btn_PLCSend_clicked(); break;
        case 11: _t->on_tabWidget_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->receiveData_Slot((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 13: _t->newConnection_Slot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->on_btn_MCSave_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DeviceList::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DeviceList::client_Singal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DeviceList::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_DeviceList.data,
    qt_meta_data_DeviceList,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DeviceList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DeviceList::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DeviceList.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int DeviceList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void DeviceList::client_Singal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
