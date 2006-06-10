/****************************************************************************
** Meta object code from reading C++ file 'qsql_psql.h'
**
** Created: vie jun 9 23:51:03 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qsql_psql.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qsql_psql.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QPSQLDriver[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_QPSQLDriver[] = {
    "QPSQLDriver\0"
};

const QMetaObject QPSQLDriver::staticMetaObject = {
    { &QSqlDriver::staticMetaObject, qt_meta_stringdata_QPSQLDriver,
      qt_meta_data_QPSQLDriver, 0 }
};

const QMetaObject *QPSQLDriver::metaObject() const
{
    return &staticMetaObject;
}

void *QPSQLDriver::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPSQLDriver))
	return static_cast<void*>(const_cast<QPSQLDriver*>(this));
    return QSqlDriver::qt_metacast(_clname);
}

int QPSQLDriver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSqlDriver::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
