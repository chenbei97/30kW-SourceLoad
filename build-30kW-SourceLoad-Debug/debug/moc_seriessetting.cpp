/****************************************************************************
** Meta object code from reading C++ file 'seriessetting.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../seriessetting/seriessetting.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'seriessetting.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SeriesSetting_t {
    QByteArrayData data[6];
    char stringdata0[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SeriesSetting_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SeriesSetting_t qt_meta_stringdata_SeriesSetting = {
    {
QT_MOC_LITERAL(0, 0, 13), // "SeriesSetting"
QT_MOC_LITERAL(1, 14, 13), // "seriesToggled"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 13), // "seriesRemoved"
QT_MOC_LITERAL(4, 43, 16), // "QAbstractSeries*"
QT_MOC_LITERAL(5, 60, 13) // "onNameChanged"

    },
    "SeriesSetting\0seriesToggled\0\0seriesRemoved\0"
    "QAbstractSeries*\0onNameChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SeriesSetting[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       3,    1,   37,    2, 0x06 /* Public */,
       3,    1,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   43,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 4,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,

       0        // eod
};

void SeriesSetting::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SeriesSetting *_t = static_cast<SeriesSetting *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->seriesToggled((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->seriesRemoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->seriesRemoved((*reinterpret_cast< QAbstractSeries*(*)>(_a[1]))); break;
        case 3: _t->onNameChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSeries* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SeriesSetting::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SeriesSetting::seriesToggled)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SeriesSetting::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SeriesSetting::seriesRemoved)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SeriesSetting::*_t)(QAbstractSeries * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SeriesSetting::seriesRemoved)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject SeriesSetting::staticMetaObject = {
    { &QVBoxLayout::staticMetaObject, qt_meta_stringdata_SeriesSetting.data,
      qt_meta_data_SeriesSetting,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SeriesSetting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SeriesSetting::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SeriesSetting.stringdata0))
        return static_cast<void*>(this);
    return QVBoxLayout::qt_metacast(_clname);
}

int SeriesSetting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QVBoxLayout::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SeriesSetting::seriesToggled(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SeriesSetting::seriesRemoved(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SeriesSetting::seriesRemoved(QAbstractSeries * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
