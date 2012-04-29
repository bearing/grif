/****************************************************************************
** Meta object code from reading C++ file 'GRIHist1DGridScrollWidget.h'
**
** Created: Sat Apr 28 20:32:24 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../framework/include/hist/GRIHist1DGridScrollWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GRIHist1DGridScrollWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GRIHist1DGridScrollWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      33,   27,   26,   26, 0x0a,
      58,   27,   26,   26, 0x0a,
      85,   27,   26,   26, 0x0a,
     115,   26,   26,   26, 0x09,
     128,   26,   26,   26, 0x09,
     142,   26,   26,   26, 0x09,
     153,   26,   26,   26, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_GRIHist1DGridScrollWidget[] = {
    "GRIHist1DGridScrollWidget\0\0event\0"
    "paintEvent(QPaintEvent*)\0"
    "resizeEvent(QResizeEvent*)\0"
    "mousePressEvent(QMouseEvent*)\0"
    "scrollLeft()\0scrollRight()\0scrollUp()\0"
    "scrollDown()\0"
};

void GRIHist1DGridScrollWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GRIHist1DGridScrollWidget *_t = static_cast<GRIHist1DGridScrollWidget *>(_o);
        switch (_id) {
        case 0: _t->paintEvent((*reinterpret_cast< QPaintEvent*(*)>(_a[1]))); break;
        case 1: _t->resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 2: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: _t->scrollLeft(); break;
        case 4: _t->scrollRight(); break;
        case 5: _t->scrollUp(); break;
        case 6: _t->scrollDown(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GRIHist1DGridScrollWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GRIHist1DGridScrollWidget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_GRIHist1DGridScrollWidget,
      qt_meta_data_GRIHist1DGridScrollWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GRIHist1DGridScrollWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GRIHist1DGridScrollWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GRIHist1DGridScrollWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GRIHist1DGridScrollWidget))
        return static_cast<void*>(const_cast< GRIHist1DGridScrollWidget*>(this));
    return QFrame::qt_metacast(_clname);
}

int GRIHist1DGridScrollWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
