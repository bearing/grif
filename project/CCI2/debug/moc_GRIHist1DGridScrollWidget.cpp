/****************************************************************************
** Meta object code from reading C++ file 'GRIHist1DGridScrollWidget.h'
**
** Created: Sat Oct 8 15:32:59 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../application/gui/grihistwidget/trunk/include/GRIHist1DGridScrollWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GRIHist1DGridScrollWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GRIHist1DGridScrollWidget[] = {

 // content:
       5,       // revision
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

const QMetaObject GRIHist1DGridScrollWidget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_GRIHist1DGridScrollWidget,
      qt_meta_data_GRIHist1DGridScrollWidget, 0 }
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
        switch (_id) {
        case 0: paintEvent((*reinterpret_cast< QPaintEvent*(*)>(_a[1]))); break;
        case 1: resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 2: mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: scrollLeft(); break;
        case 4: scrollRight(); break;
        case 5: scrollUp(); break;
        case 6: scrollDown(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
