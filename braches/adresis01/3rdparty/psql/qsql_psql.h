/****************************************************************************
**
** Copyright (C) 1992-2006 Trolltech AS. All rights reserved.
**
** This file is part of the QtSql module of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QSQL_PSQL_H
#define QSQL_PSQL_H

#include <QtSql/qsqlresult.h>
#include <QtSql/qsqldriver.h>

#ifdef QT_PLUGIN
#define Q_EXPORT_SQLDRIVER_PSQL
#else
#define Q_EXPORT_SQLDRIVER_PSQL Q_SQL_EXPORT
#endif

typedef struct pg_conn PGconn;
typedef struct pg_result PGresult;

class QPSQLResultPrivate;
class QPSQLDriverPrivate;
class QPSQLDriver;
class QSqlRecordInfo;

class QPSQLResult : public QSqlResult
{
    friend class QPSQLResultPrivate;
public:
    QPSQLResult(const QPSQLDriver* db, const QPSQLDriverPrivate* p);
    ~QPSQLResult();

    QVariant handle() const;

protected:
    void cleanup();
    bool fetch(int i);
    bool fetchFirst();
    bool fetchLast();
    QVariant data(int i);
    bool isNull(int field);
    bool reset (const QString& query);
    int size();
    int numRowsAffected();
    QSqlRecord record() const;
    QVariant lastInsertId() const;

private:
    QPSQLResultPrivate *d;
};

class Q_EXPORT_SQLDRIVER_PSQL QPSQLDriver : public QSqlDriver
{
    Q_OBJECT
public:
    enum Protocol {
        Version6 = 6,
        Version7 = 7,
        Version71 = 8,
        Version73 = 9
    };

    explicit QPSQLDriver(QObject *parent=0);
    explicit QPSQLDriver(PGconn *conn, QObject *parent=0);
    ~QPSQLDriver();
    bool hasFeature(DriverFeature f) const;
    bool open(const QString & db,
              const QString & user,
              const QString & password,
              const QString & host,
              int port,
              const QString& connOpts);
    bool isOpen() const;
    void close();
    QSqlResult *createResult() const;
    QStringList tables(QSql::TableType) const;
    QSqlIndex primaryIndex(const QString& tablename) const;
    QSqlRecord record(const QString& tablename) const;

    Protocol protocol() const;
    QVariant handle() const;

    QString escapeIdentifier(const QString &identifier, IdentifierType type) const;
    QString formatValue(const QSqlField &field,
                                     bool trimStrings) const;

protected:
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
private:
    void init();
    QPSQLDriverPrivate *d;
};

#endif // QSQL_PSQL_H
