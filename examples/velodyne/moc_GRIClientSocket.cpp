/****************************************************************************
** Meta object code from reading C++ file 'GRIClientSocket.h'
**
** Created: Mon Oct 17 09:48:11 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../framework/trunk/include/GRIClientSocket.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GRIClientSocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GRIClientSocket[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   17,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   16,   16,   16, 0x08,
      63,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GRIClientSocket[] = {
    "GRIClientSocket\0\0command\0"
    "incomingCommand(QString)\0readClient()\0"
    "disconnectionMessage()\0"
};

const QMetaObject GRIClientSocket::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_GRIClientSocket,
      qt_meta_data_GRIClientSocket, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GRIClientSocket::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GRIClientSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GRIClientSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GRIClientSocket))
        return static_cast<void*>(const_cast< GRIClientSocket*>(this));
    return QTcpSocket::qt_metacast(_clname);
}

int GRIClientSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: incomingCommand((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: readClient(); break;
        case 2: disconnectionMessage(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void GRIClientSocket::incomingCommand(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
