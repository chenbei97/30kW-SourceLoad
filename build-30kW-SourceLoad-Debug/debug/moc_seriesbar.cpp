/****************************************************************************
** Meta object code from reading C++ file 'seriesbar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../seriessetting/seriesbar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'seriesbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SeriesBar_t {
    QByteArrayData data[8];
    char stringdata0[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SeriesBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SeriesBar_t qt_meta_stringdata_SeriesBar = {
    {
QT_MOC_LITERAL(0, 0, 9), // "SeriesBar"
QT_MOC_LITERAL(1, 10, 11), // "changeWidth"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 13), // "changeVisible"
QT_MOC_LITERAL(4, 37, 11), // "changeAngle"
QT_MOC_LITERAL(5, 49, 14), // "changePosition"
QT_MOC_LITERAL(6, 64, 10), // "changeFont"
QT_MOC_LITERAL(7, 75, 17) // "changeBorderColor"

    },
    "SeriesBar\0changeWidth\0\0changeVisible\0"
    "changeAngle\0changePosition\0changeFont\0"
    "changeBorderColor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SeriesBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x08 /* Private */,
       3,    1,   47,    2, 0x08 /* Private */,
       4,    1,   50,    2, 0x08 /* Private */,
       5,    1,   53,    2, 0x08 /* Private */,
       6,    0,   56,    2, 0x08 /* Private */,
       7,    0,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SeriesBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SeriesBar *_t = static_cast<SeriesBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeWidth((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->changeVisible((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->changeAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->changePosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->changeFont(); break;
        case 5: _t->changeBorderColor(); break;
        default: ;
        }
    }
}

const QMetaObject SeriesBar::staticMetaObject = {
    { &QGroupBox::staticMetaObject, qt_meta_stringdata_SeriesBar.data,
      qt_meta_data_SeriesBar,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SeriesBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SeriesBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SeriesBar.stringdata0))
        return static_cast<void*>(this);
    return QGroupBox::qt_metacast(_clname);
}

int SeriesBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE