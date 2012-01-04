/****************************************************************************
** Meta object code from reading C++ file 'GRILogger.h'
**
** Created: Tue Jan 3 22:25:23 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../framework/include/GRILogger.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GRILogger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GRILogger[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      31,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      67,   63,   58,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GRILogger[] = {
    "GRILogger\0\0output(std::string)\0"
    "output(QList<std::string>)\0bool\0msg\0"
    "writeLogFile(GRILogMessage)\0"
};

void GRILogger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GRILogger *_t = static_cast<GRILogger *>(_o);
        switch (_id) {
        case 0: _t->output((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 1: _t->output((*reinterpret_cast< QList<std::string>(*)>(_a[1]))); break;
        case 2: { bool _r = _t->writeLogFile((*reinterpret_cast< GRILogMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GRILogger::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GRILogger::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GRILogger,
      qt_meta_data_GRILogger, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GRILogger::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GRILogger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GRILogger::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GRILogger))
        return static_cast<void*>(const_cast< GRILogger*>(this));
    return QObject::qt_metacast(_clname);
}

int GRILogger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void GRILogger::output(std::string _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GRILogger::output(QList<std::string> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
