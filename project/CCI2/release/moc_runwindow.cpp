/****************************************************************************
** Meta object code from reading C++ file 'runwindow.h'
**
** Created: Fri Nov 11 11:40:18 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/runwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'runwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RunWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      19,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RunWindow[] = {
    "RunWindow\0\0start()\0stop()\0"
};

const QMetaObject RunWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_RunWindow,
      qt_meta_data_RunWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RunWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RunWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RunWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RunWindow))
        return static_cast<void*>(const_cast< RunWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int RunWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: start(); break;
        case 1: stop(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
