#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <QDateTime>
#include <QSqlQueryModel>
#include <QString>
#include <QStandardItemModel>
class Hospital {
  private:
    QString Nom_dep;
    QString Adresse;
    QString Numero;
    QString Email;
    QString Service;
    QString Equipements;
    QString Remarques;
    int Capacite, ID;
    QDateTime Created_at;
    QDateTime Updated_at;

  public:
    Hospital();
    Hospital(QString, QString, QString, QString, QString, QString, QString,
             int);
    bool Create();
    QSqlQueryModel *Show();
    bool Update(int);
    bool Delete(int);
    int totalyHospitals();
    float averageHospitalsCapacity();
    bool verifiylatLong(QString);

    QSqlQueryModel *FetchAvailableAmbulance();
    QSqlQueryModel *FetchAllAmbulance();
    QSqlQueryModel *FetchEmergencyCalls();
    QStandardItemModel *getAssignedEmergencyCalls();
    QStandardItemModel *getNonAssignedEmergencyCalls();

    bool ChangeAmbulanceAvailability(QString);
    bool AsignAmbuanceToEmergencyCall(int, QString);
    bool AsignHospitalToEmergencyCall(int, int);

    QSqlQueryModel *FetchAmbulanceByMatricule(QString);
    QSqlQueryModel *FetchHospitalByID(int);
    QSqlQueryModel *FetchOccupiedEmergencyCalls();
    bool UpdateVitesse(int);
    int ShowVitesse();
};

#endif // HOSPITAL_H
