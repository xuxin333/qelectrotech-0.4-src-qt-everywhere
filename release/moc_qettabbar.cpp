/****************************************************************************
** Meta object code from reading C++ file 'qettabbar.h'
**
** Created: Sat Feb 27 15:10:06 2016
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../sources/qettabbar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qettabbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QETTabBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      28,   10,   10,   10, 0x05,
      47,   10,   10,   10, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QETTabBar[] = {
    "QETTabBar\0\0lastTabRemoved()\0"
    "firstTabInserted()\0tabDoubleClicked(int)\0"
};

void QETTabBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QETTabBar *_t = static_cast<QETTabBar *>(_o);
        switch (_id) {
        case 0: _t->lastTabRemoved(); break;
        case 1: _t->firstTabInserted(); break;
        case 2: _t->tabDoubleClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QETTabBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QETTabBar::staticMetaObject = {
    { &QTabBar::staticMetaObject, qt_meta_stringdata_QETTabBar,
      qt_meta_data_QETTabBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QETTabBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QETTabBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QETTabBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QETTabBar))
        return static_cast<void*>(const_cast< QETTabBar*>(this));
    return QTabBar::qt_metacast(_clname);
}

int QETTabBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabBar::qt_metacall(_c, _id, _a);
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
void QETTabBar::lastTabRemoved()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QETTabBar::firstTabInserted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QETTabBar::tabDoubleClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
