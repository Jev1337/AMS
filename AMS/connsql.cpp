#include "connsql.h"

ConnSQL::ConnSQL() {
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("yassine");
    db.setPassword("password");
    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
    } else {
        qDebug() << "Connected!";
        status = true;
    }
}

ConnSQL::ConnSQL(QString n, QString p) {
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName(n);
    db.setPassword(p);
    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
    } else {
        qDebug() << "Connected!";
        status = true;
    }
}

ConnSQL::~ConnSQL() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Disconnected!";
    }
    status = false;
}

QSqlDatabase *ConnSQL::getDB() { return &db; }
