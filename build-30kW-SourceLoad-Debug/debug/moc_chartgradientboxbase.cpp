/****************************************************************************
** Meta object code from reading C++ file 'chartgradientboxbase.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../chartsetting/chartgradientboxbase.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chartgradientboxbase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChartGradientBoxBase_t {
    QByteArrayData data[4];
    char stringdata0[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChartGradientBoxBase_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChartGradientBoxBase_t qt_meta_stringdata_ChartGradientBoxBase = {
    {
QT_MOC_LITERAL(0, 0, 20), // "ChartGradientBoxBase"
QT_MOC_LITERAL(1, 21, 20), // "linearGradientParams"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 29) // "QVector<QPair<qreal,QColor> >"

    },
    "ChartGradientBoxBase\0linearGradientParams\0"
    "\0QVector<QPair<qreal,QColor> >"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChartGradientBoxBase[] = {

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
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

void ChartGradientBoxBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ChartGradientBoxBase *_t = static_cast<ChartGradientBoxBase *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->linearGradientParams((*reinterpret_cast< QVector<QPair<qreal,QColor> >(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (ChartGradientBoxBase::*_t)(QVector<QPair<qreal,QColor>> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChartGradientBoxBase::linearGradientParams)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ChartGradientBoxBase::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ChartGradientBoxBase.data,
      qt_meta_data_ChartGradientBoxBase,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ChartGradientBoxBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChartGradientBoxBase::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChartGradientBoxBase.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ChartGradientBoxBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ChartGradientBoxBase::linearGradientParams(QVector<QPair<qreal,QColor>> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE