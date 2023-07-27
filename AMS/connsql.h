#ifndef CONNSQL_H
#define CONNSQL_H

#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class ConnSQL {
  public:
    ConnSQL();
    ConnSQL(QString, QString);
    ~ConnSQL();
    void close();
    QSqlDatabase *getDB();

  private:
    bool status;
    QSqlDatabase db;
};

#endif // CONNSQL_H
