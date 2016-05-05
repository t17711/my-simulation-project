/****************************************************************************
** Meta object code from reading C++ file 'Median.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Median.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Median.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Median_t {
    QByteArrayData data[7];
    char stringdata0[62];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Median_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Median_t qt_meta_stringdata_Median = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Median"
QT_MOC_LITERAL(1, 7, 13), // "changeSliderX"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 13), // "changeSliderY"
QT_MOC_LITERAL(4, 36, 9), // "changeAvg"
QT_MOC_LITERAL(5, 46, 7), // "combine"
QT_MOC_LITERAL(6, 54, 7) // "useHist"

    },
    "Median\0changeSliderX\0\0changeSliderY\0"
    "changeAvg\0combine\0useHist"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Median[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x09 /* Protected */,
       3,    1,   42,    2, 0x09 /* Protected */,
       4,    1,   45,    2, 0x09 /* Protected */,
       5,    1,   48,    2, 0x09 /* Protected */,
       6,    1,   51,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void Median::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Median *_t = static_cast<Median *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeSliderX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->changeSliderY((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->changeAvg((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->combine((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->useHist((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Median::staticMetaObject = {
    { &ImageFilter::staticMetaObject, qt_meta_stringdata_Median.data,
      qt_meta_data_Median,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Median::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Median::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Median.stringdata0))
        return static_cast<void*>(const_cast< Median*>(this));
    return ImageFilter::qt_metacast(_clname);
}

int Median::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ImageFilter::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
