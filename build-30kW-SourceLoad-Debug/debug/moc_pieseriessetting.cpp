/****************************************************************************
** Meta object code from reading C++ file 'pieseriessetting.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../seriessetting/pieseriessetting.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pieseriessetting.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PieSeriesSetting_t {
    QByteArrayData data[5];
    char stringdata0[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PieSeriesSetting_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PieSeriesSetting_t qt_meta_stringdata_PieSeriesSetting = {
    {
QT_MOC_LITERAL(0, 0, 16), // "PieSeriesSetting"
QT_MOC_LITERAL(1, 17, 19), // "associateCompeleted"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 11), // "modeChanged"
QT_MOC_LITERAL(4, 50, 18) // "seriesColorChanged"

    },
    "PieSeriesSetting\0associateCompeleted\0"
    "\0modeChanged\0seriesColorChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PieSeriesSetting[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    2,   30,    2, 0x06 /* Public */,
       4,    2,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QColor, QMetaType::Int,    2,    2,

       0        // eod
};

void PieSeriesSetting::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PieSeriesSetting *_t = static_cast<PieSeriesSetting *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->associateCompeleted(); break;
        case 1: _t->modeChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->seriesColorChanged((*reinterpret_cast< QColor(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (PieSeriesSetting::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PieSeriesSetting::associateCompeleted)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (PieSeriesSetting::*_t)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PieSeriesSetting::modeChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (PieSeriesSetting::*_t)(QColor , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PieSeriesSetting::seriesColorChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject PieSeriesSetting::staticMetaObject = {
    { &QVBoxLayout::staticMetaObject, qt_meta_stringdata_PieSeriesSetting.data,
      qt_meta_data_PieSeriesSetting,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PieSeriesSetting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PieSeriesSetting::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PieSeriesSetting.stringdata0))
        return static_cast<void*>(this);
    return QVBoxLayout::qt_metacast(_clname);
}

int PieSeriesSetting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QVBoxLayout::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void PieSeriesSetting::associateCompeleted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void PieSeriesSetting::modeChanged(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PieSeriesSetting::seriesColorChanged(QColor _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
