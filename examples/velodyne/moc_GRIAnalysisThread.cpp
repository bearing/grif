/****************************************************************************
** Meta object code from reading C++ file 'GRIAnalysisThread.h'
**
** Created: Mon Oct 17 09:48:10 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../framework/trunk/include/GRIAnalysisThread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GRIAnalysisThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GRIAnalysisThread[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      65,   56,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GRIAnalysisThread[] = {
    "GRIAnalysisThread\0\0hist\0"
    "SendHistogram(GRIHistogrammer*)\0"
    "HistName\0GetHistogramSlot(QString)\0"
};

const QMetaObject GRIAnalysisThread::staticMetaObject = {
    { &GRIProcessThread::staticMetaObject, qt_meta_stringdata_GRIAnalysisThread,
      qt_meta_data_GRIAnalysisThread, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GRIAnalysisThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GRIAnalysisThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GRIAnalysisThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GRIAnalysisThread))
        return static_cast<void*>(const_cast< GRIAnalysisThread*>(this));
    return GRIProcessThread::qt_metacast(_clname);
}

int GRIAnalysisThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GRIProcessThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SendHistogram((*reinterpret_cast< GRIHistogrammer*(*)>(_a[1]))); break;
        case 1: GetHistogramSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void GRIAnalysisThread::SendHistogram(GRIHistogrammer * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
