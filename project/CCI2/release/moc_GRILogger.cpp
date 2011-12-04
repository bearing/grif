/****************************************************************************
** Meta object code from reading C++ file 'GRILogger.h'
**
** Created: Tue Oct 18 15:32:53 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../framework/trunk/include/GRILogger.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GRILogger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GRILogger[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      26,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      56,   52,   47,   10, 0x0a,
      78,   52,   47,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GRILogger[] = {
    "GRILogger\0\0output(string)\0"
    "output(list<string>)\0bool\0msg\0"
    "writeLogFile(QString)\0writeLogFile(GRILogMessage)\0"
};

const QMetaObject GRILogger::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GRILogger,
      qt_meta_data_GRILogger, 0 }
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
        switch (_id) {
        case 0: output((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 1: output((*reinterpret_cast< list<string>(*)>(_a[1]))); break;
        case 2: { bool _r = writeLogFile((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = writeLogFile((*reinterpret_cast< GRILogMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void GRILogger::output(string _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GRILogger::output(list<string> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
