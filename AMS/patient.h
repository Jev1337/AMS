#ifndef PATIENT_H
#define PATIENT_H
#include <QString>
#include <QDate>
#include <QSqlQueryModel>


class Patient
{
private:
    QString Cin;
    QString Nom;
    QString Prenom;
    QDate Date;
    QDateTime Created_at;
    QDateTime Updated_at;
public:
    Patient();
    Patient(QString,QString,QString,QDate);
    bool Create();
    QSqlQueryModel* Show();
    bool Update();
    bool Delete(QString);
    bool IDexist(QString);
    int count();
    bool UpdatePulsation(int);
    int ShowPulsation();

};

#endif // PATIENT_H
