/****************************************************************************
** Meta object code from reading C++ file 'LinkGame.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../LinkGame.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LinkGame.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSLinkGameENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSLinkGameENDCLASS = QtMocHelpers::stringData(
    "LinkGame",
    "updateRemainTime",
    "",
    "updateHintTime",
    "updateFlashTime",
    "updateFreezeTime",
    "updateDizzyTime",
    "startNewGame",
    "loadGame"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSLinkGameENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[9];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[15];
    char stringdata4[16];
    char stringdata5[17];
    char stringdata6[16];
    char stringdata7[13];
    char stringdata8[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSLinkGameENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSLinkGameENDCLASS_t qt_meta_stringdata_CLASSLinkGameENDCLASS = {
    {
        QT_MOC_LITERAL(0, 8),  // "LinkGame"
        QT_MOC_LITERAL(9, 16),  // "updateRemainTime"
        QT_MOC_LITERAL(26, 0),  // ""
        QT_MOC_LITERAL(27, 14),  // "updateHintTime"
        QT_MOC_LITERAL(42, 15),  // "updateFlashTime"
        QT_MOC_LITERAL(58, 16),  // "updateFreezeTime"
        QT_MOC_LITERAL(75, 15),  // "updateDizzyTime"
        QT_MOC_LITERAL(91, 12),  // "startNewGame"
        QT_MOC_LITERAL(104, 8)   // "loadGame"
    },
    "LinkGame",
    "updateRemainTime",
    "",
    "updateHintTime",
    "updateFlashTime",
    "updateFreezeTime",
    "updateDizzyTime",
    "startNewGame",
    "loadGame"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSLinkGameENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x0a,    1 /* Public */,
       3,    0,   57,    2, 0x0a,    2 /* Public */,
       4,    0,   58,    2, 0x0a,    3 /* Public */,
       5,    0,   59,    2, 0x0a,    4 /* Public */,
       6,    0,   60,    2, 0x0a,    5 /* Public */,
       7,    0,   61,    2, 0x0a,    6 /* Public */,
       8,    0,   62,    2, 0x0a,    7 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject LinkGame::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSLinkGameENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSLinkGameENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSLinkGameENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LinkGame, std::true_type>,
        // method 'updateRemainTime'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateHintTime'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateFlashTime'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateFreezeTime'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateDizzyTime'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'startNewGame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadGame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void LinkGame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LinkGame *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateRemainTime(); break;
        case 1: _t->updateHintTime(); break;
        case 2: _t->updateFlashTime(); break;
        case 3: _t->updateFreezeTime(); break;
        case 4: _t->updateDizzyTime(); break;
        case 5: _t->startNewGame(); break;
        case 6: _t->loadGame(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *LinkGame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LinkGame::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSLinkGameENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LinkGame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
