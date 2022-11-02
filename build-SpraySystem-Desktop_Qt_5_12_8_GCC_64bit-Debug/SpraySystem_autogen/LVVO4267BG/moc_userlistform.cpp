/****************************************************************************
** Meta object code from reading C++ file 'userlistform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../SpraySystem/Forms/Users/List/userlistform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userlistform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UserListForm_t {
    QByteArrayData data[9];
    char stringdata0[131];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UserListForm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UserListForm_t qt_meta_stringdata_UserListForm = {
    {
QT_MOC_LITERAL(0, 0, 12), // "UserListForm"
QT_MOC_LITERAL(1, 13, 24), // "on_act_AddUser_triggered"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 21), // "on_act_edit_triggered"
QT_MOC_LITERAL(4, 61, 23), // "on_act_delete_triggered"
QT_MOC_LITERAL(5, 85, 9), // "save_slot"
QT_MOC_LITERAL(6, 95, 21), // "std::shared_ptr<User>"
QT_MOC_LITERAL(7, 117, 4), // "user"
QT_MOC_LITERAL(8, 122, 8) // "iscreate"

    },
    "UserListForm\0on_act_AddUser_triggered\0"
    "\0on_act_edit_triggered\0on_act_delete_triggered\0"
    "save_slot\0std::shared_ptr<User>\0user\0"
    "iscreate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UserListForm[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    2,   37,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Bool,    7,    8,

       0        // eod
};

void UserListForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UserListForm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_act_AddUser_triggered(); break;
        case 1: _t->on_act_edit_triggered(); break;
        case 2: _t->on_act_delete_triggered(); break;
        case 3: _t->save_slot((*reinterpret_cast< std::shared_ptr<User>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UserListForm::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_UserListForm.data,
    qt_meta_data_UserListForm,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UserListForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserListForm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UserListForm.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int UserListForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
