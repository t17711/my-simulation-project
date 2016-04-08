/****************************************************************************
** Meta object code from reading C++ file 'HistogramStretch.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../HistogramStretch.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HistogramStretch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_HistogramStretch_t {
    QByteArrayData data[6];
    char stringdata0[66];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HistogramStretch_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HistogramStretch_t qt_meta_stringdata_HistogramStretch = {
    {
QT_MOC_LITERAL(0, 0, 16), // "HistogramStretch"
QT_MOC_LITERAL(1, 17, 9), // "changeMin"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 9), // "changeMax"
QT_MOC_LITERAL(4, 38, 13), // "changeMinAuto"
QT_MOC_LITERAL(5, 52, 13) // "changeMaxAuto"

    },
    "HistogramStretch\0changeMin\0\0changeMax\0"
    "changeMinAuto\0changeMaxAuto"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HistogramStretch[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x09 /* Protected */,
       3,    1,   37,    2, 0x09 /* Protected */,
       4,    1,   40,    2, 0x09 /* Protected */,
       5,    1,   43,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void HistogramStretch::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HistogramStretch *_t = static_cast<HistogramStretch *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeMin((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->changeMax((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->changeMinAuto((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->changeMaxAuto((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject HistogramStretch::staticMetaObject = {
    { &ImageFilter::staticMetaObject, qt_meta_stringdata_HistogramStretch.data,
      qt_meta_data_HistogramStretch,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *HistogramStretch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HistogramStretch::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_HistogramStretch.stringdata0))
        return static_cast<void*>(const_cast< HistogramStretch*>(this));
    return ImageFilter::qt_metacast(_clname);
}

int HistogramStretch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ImageFilter::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
