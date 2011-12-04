/****************************************************************************
** Meta object code from reading C++ file 'GRIHist1DWidget.h'
**
** Created: Tue Oct 18 15:32:54 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../application/gui/grihistwidget/trunk/include/GRIHist1DWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GRIHist1DWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GRIHist1DWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,
      36,   30,   16,   16, 0x0a,
      61,   30,   16,   16, 0x0a,
      88,   30,   16,   16, 0x0a,
     113,   30,   16,   16, 0x0a,
     149,   30,   16,   16, 0x0a,
     178,   30,   16,   16, 0x0a,
     208,   30,   16,   16, 0x0a,
     240,   16,   16,   16, 0x09,
     257,   16,   16,   16, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_GRIHist1DWidget[] = {
    "GRIHist1DWidget\0\0updateData()\0event\0"
    "paintEvent(QPaintEvent*)\0"
    "resizeEvent(QResizeEvent*)\0"
    "wheelEvent(QWheelEvent*)\0"
    "mouseDoubleClickEvent(QMouseEvent*)\0"
    "mouseMoveEvent(QMouseEvent*)\0"
    "mousePressEvent(QMouseEvent*)\0"
    "mouseReleaseEvent(QMouseEvent*)\0"
    "toggleLogScale()\0toggleAutoScale()\0"
};

const QMetaObject GRIHist1DWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GRIHist1DWidget,
      qt_meta_data_GRIHist1DWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GRIHist1DWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GRIHist1DWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GRIHist1DWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GRIHist1DWidget))
        return static_cast<void*>(const_cast< GRIHist1DWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int GRIHist1DWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateData(); break;
        case 1: paintEvent((*reinterpret_cast< QPaintEvent*(*)>(_a[1]))); break;
        case 2: resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 3: wheelEvent((*reinterpret_cast< QWheelEvent*(*)>(_a[1]))); break;
        case 4: mouseDoubleClickEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 5: mouseMoveEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 6: mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 7: mouseReleaseEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 8: toggleLogScale(); break;
        case 9: toggleAutoScale(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
