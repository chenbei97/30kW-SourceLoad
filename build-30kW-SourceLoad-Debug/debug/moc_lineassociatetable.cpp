/****************************************************************************
** Meta object code from reading C++ file 'lineassociatetable.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../chartview/lineassociatetable.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lineassociatetable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LineAssociateTable_t {
    QByteArrayData data[7];
    char stringdata0[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LineAssociateTable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LineAssociateTable_t qt_meta_stringdata_LineAssociateTable = {
    {
QT_MOC_LITERAL(0, 0, 18), // "LineAssociateTable"
QT_MOC_LITERAL(1, 19, 19), // "associateCompeleted"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 12), // "tableChanged"
QT_MOC_LITERAL(4, 53, 18), // "seriesColorChanged"
QT_MOC_LITERAL(5, 72, 12), // "QLineSeries*"
QT_MOC_LITERAL(6, 85, 13) // "seriesRemoved"

    },
    "LineAssociateTable\0associateCompeleted\0"
    "\0tableChanged\0seriesColorChanged\0"
    "QLineSeries*\0seriesRemoved"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LineAssociateTable[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,
       4,    1,   36,    2, 0x06 /* Public */,
       6,    1,   39,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 5,    2,

       0        // eod
};

void LineAssociateTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LineAssociateTable *_t = static_cast<LineAssociateTable *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->associateCompeleted(); break;
        case 1: _t->tableChanged(); break;
        case 2: _t->seriesColorChanged((*reinterpret_cast< QLineSeries*(*)>(_a[1]))); break;
        case 3: _t->seriesRemoved((*reinterpret_cast< QLineSeries*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLineSeries* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLineSeries* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (LineAssociateTable::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LineAssociateTable::associateCompeleted)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (LineAssociateTable::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LineAssociateTable::tableChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (LineAssociateTable::*_t)(QLineSeries * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LineAssociateTable::seriesColorChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (LineAssociateTable::*_t)(QLineSeries * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LineAssociateTable::seriesRemoved)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject LineAssociateTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_LineAssociateTable.data,
      qt_meta_data_LineAssociateTable,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LineAssociateTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LineAssociateTable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LineAssociateTable.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int LineAssociateTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void LineAssociateTable::associateCompeleted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void LineAssociateTable::tableChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void LineAssociateTable::seriesColorChanged(QLineSeries * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void LineAssociateTable::seriesRemoved(QLineSeries * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE