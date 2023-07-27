#include "patient.h"
#include <QSqlQuery>
#include <QDebug>
Patient::Patient()
{

}
Patient::Patient(QString Cin,QString Nom,QString Prenom,QDate Date)
{


    this->Cin=Cin;
    this->Nom=Nom;
    this->Prenom=Prenom;
    this->Date=Date;
    this->Created_at= QDateTime::currentDateTime();
    this->Updated_at= QDateTime::currentDateTime();

}
bool Patient::Create()
{
    QSqlQuery query;

    QString SqlQuery = "insert into Patient (Cin,Nom,Prenom,DATE_NAISSANCE,Created_at,Updated_at) "
                     "VALUES (:Cin,:Nom,:Prenom,:Date,:Created_at,:Updated_at)";
    query.prepare(SqlQuery);

    query.bindValue(":Cin",Cin);
    query.bindValue(":Nom",Nom);
    query.bindValue(":Prenom",Prenom);
    query.bindValue(":Date",Date);
    query.bindValue(":Created_at",Created_at);
    query.bindValue(":Updated_at",Updated_at);

    return query.exec();
}

QSqlQueryModel* Patient::Show()
{
    QSqlQuery query;
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("Select * from Patient");
    return  model;
}

bool Patient::Delete(QString Cin)
{
    QSqlQuery query;

    QString SqlQuery = "Delete FROM Patient WHERE Cin = :Cin;";
    query.prepare(SqlQuery);

    query.bindValue(":Cin",Cin);

    return query.exec();

}

bool Patient::Update()
{
    QSqlQuery query;

    QString SqlQuery = "UPDATE Patient "
                       "set Nom=:Nom,Prenom=:Prenom,DATE_NAISSANCE  =:Date,UPDATED_AT=:UPDATED_AT where Cin=:Cin;";
    query.prepare(SqlQuery);


    query.bindValue(":Cin",Cin);
    query.bindValue(":Nom",Nom);
    query.bindValue(":Prenom",Prenom);
    query.bindValue(":Date",Date);
    query.bindValue(":UPDATED_AT",Updated_at);

    return query.exec();
}
bool Patient::IDexist(QString Cin)
{

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Patient WHERE Cin = ?");
        query.addBindValue(Cin);
    query.exec();
    query.first();
    int count = query.value(0).toInt();
    qDebug()<<count;
    return (count > 0);

}
int Patient::count(){
    QSqlQuery query;
    QString SqlQuery = "SELECT COUNT(*) as FROM Patient";

        query.prepare(SqlQuery);
        query.exec();
        query.next();
        int count = query.value(0).toInt();
        return count;

}

bool Patient::UpdatePulsation(int pulsation)
{
    QSqlQuery query;

    QString SqlQuery = QString("UPDATE HEARTRATE set PULSATION=%1").arg(pulsation);
    query.prepare(SqlQuery);



    return query.exec();
}


int Patient::ShowPulsation()
{
    QSqlQuery query;
    query.prepare("Select PULSATION from HEARTRATE");
    query.exec();
    if (query.next()) {
            return query.value(0).toInt();
    } else {
        // Handle the case where no pulsation value is found
        return -1;
    }
}
