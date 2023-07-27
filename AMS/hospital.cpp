#include "hospital.h"
#include "connsql.h"
#include <QDateTime>
#include <QTableView>
Hospital::Hospital() {}

Hospital::Hospital(QString Nom_dep, QString Adresse, QString Numero,
                   QString Email, QString Service, QString Equipements,
                   QString Remarques, int Capacite) {

    srand((unsigned)time(NULL));

    this->ID = rand();
    this->Nom_dep = Nom_dep;
    this->Adresse = Adresse;
    this->Numero = Numero;
    this->Email = Email;
    this->Service = Service;
    this->Equipements = Equipements;
    this->Remarques = Remarques;
    this->Capacite = Capacite;
    this->Created_at = QDateTime::currentDateTime();
    this->Updated_at = QDateTime::currentDateTime();
}

bool Hospital::Create() {
    QSqlQuery query;

    QString SqlQuery =
        "insert into Hospital "
        "(ID,NOM_DEP,ADRESSE,NUMERO,EMAIL,CAPACITE,SERVICE,EQUIPEMENTS,"
        "REMARQUES,CREATED_AT,UPDATED_AT) "
        "VALUES "
        "(:ID,:Nom_dep,:Adresse,:Numero,:Email,:Capacite,:Service,:Equipements,"
        ":Remarques,:Created_at,:Updated_at)";
    query.prepare(SqlQuery);

    query.bindValue(":ID", ID);
    query.bindValue(":Nom_dep", Nom_dep);
    query.bindValue(":Adresse", Adresse);
    query.bindValue(":Numero", Numero);
    query.bindValue(":Email", Email);
    query.bindValue(":Capacite", Capacite);
    query.bindValue(":Service", Service);
    query.bindValue(":Equipements", Equipements);
    query.bindValue(":Remarques", Remarques);
    query.bindValue(":Created_at", Created_at);
    query.bindValue(":Updated_at", Updated_at);

    return query.exec();
}

QSqlQueryModel *Hospital::Show() {
    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("Select * from Hospital");
    return model;
}

bool Hospital::Delete(int ID) {
    QSqlQuery query;

    QString SqlQuery = "Delete FROM Hospital WHERE ID = :ID;";
    query.prepare(SqlQuery);

    query.bindValue(":ID", ID);

    return query.exec();
}

bool Hospital::Update(int ID) {
    QSqlQuery query;

    QString SqlQuery =
        "UPDATE Hospital "
        "set "
        "NOM_DEP=:NOM_DEP,ADRESSE=:ADRESSE,NUMERO=:NUMERO,EMAIL=:EMAIL,"
        "CAPACITE=:CAPACITE,SERVICE=:SERVICE,EQUIPEMENTS=:EQUIPEMENTS,"
        "REMARQUES=:REMARQUES,UPDATED_AT=:UPDATED_AT where ID=:ID;";
    query.prepare(SqlQuery);

    query.bindValue(":ID", ID);
    query.bindValue(":NOM_DEP", Nom_dep);
    query.bindValue(":ADRESSE", Adresse);
    query.bindValue(":NUMERO", Numero);
    query.bindValue(":EMAIL", Email);
    query.bindValue(":CAPACITE", Capacite);
    query.bindValue(":SERVICE", Service);
    query.bindValue(":EQUIPEMENTS", Equipements);
    query.bindValue(":REMARQUES", Remarques);
    query.bindValue(":UPDATED_AT", Updated_at);

    return query.exec();
}

int Hospital::totalyHospitals() {
    QSqlQuery query;
    QString SqlQuery = "SELECT COUNT(*) as FROM Hospital;";

    query.prepare(SqlQuery);
    query.exec();
    query.next();
    int count = query.value(0).toInt();
    return count;
}
float Hospital::averageHospitalsCapacity() {
    QSqlQuery query;
    QString SqlQuery = "SELECT AVG(CAPACITE) as avg_capacite FROM Hospital;";

    query.prepare(SqlQuery);
    query.exec();
    query.next();
    float avg = query.value(0).toFloat();
    return avg;
}
bool Hospital::verifiylatLong(QString latLongString) {
    QStringList latLongList = latLongString.split(",");
    if (latLongList.count() != 2) {
        return false;
    }

    bool isValidLat = false;
    bool isValidLong = false;
    double latitude = latLongList[0].toDouble(&isValidLat);
    double longitude = latLongList[1].toDouble(&isValidLong);

    if (!isValidLat || !isValidLong) {
        return false;
    }

    if (latitude < -90 || latitude > 90) {
        return false;
    }

    if (longitude < -180 || longitude > 180) {
        return false;
    }

    return true;
}

QSqlQueryModel *Hospital::FetchAvailableAmbulance() {
    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery(QString("Select * from AMBULANCE Where ETAT = %1").arg(0));
    return model;
}

QSqlQueryModel *Hospital::FetchEmergencyCalls() {
    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery(
        QString("Select * from EMERGENCYCALL Where Status != 6"));
    return model;
}

QStandardItemModel *Hospital::getAssignedEmergencyCalls(){
    auto* model = new QStandardItemModel;



    QSqlQuery query;
    QStandardItem* idHeader = new QStandardItem("ID");
    QStandardItem* problemHeader = new QStandardItem("PROBLEM");
    QStandardItem* statusHeader = new QStandardItem("STATUS");

    model->setHorizontalHeaderItem(0, idHeader);
    model->setHorizontalHeaderItem(1, problemHeader);
    model->setHorizontalHeaderItem(2, statusHeader);

    query.prepare("SELECT ID, PROBLEM, STATUS FROM EMERGENCYCALL WHERE STATUS = 3 OR STATUS =5");
    query.exec();
    while (query.next()) {
        QString id = query.value(0).toString();
        QString problem = query.value(1).toString();
        QString status = query.value(2).toString();

        QStandardItem* idItem = new QStandardItem(id);
        QStandardItem* problemItem = new QStandardItem(problem);
        QStandardItem* statusItem = new QStandardItem(status);

        model->appendRow({ idItem, problemItem, statusItem });
    }

    return model;
}

QStandardItemModel *Hospital::getNonAssignedEmergencyCalls(){
    auto* model = new QStandardItemModel;



    QSqlQuery query;
    QStandardItem* idHeader = new QStandardItem("ID");
    QStandardItem* problemHeader = new QStandardItem("PROBLEM");
    QStandardItem* statusHeader = new QStandardItem("STATUS");

    model->setHorizontalHeaderItem(0, idHeader);
    model->setHorizontalHeaderItem(1, problemHeader);
    model->setHorizontalHeaderItem(2, statusHeader);

    query.prepare("SELECT ID, PROBLEM, STATUS FROM EMERGENCYCALL WHERE STATUS = 2 or STATUS = 4");
    query.exec();
    while (query.next()) {
        QString id = query.value(0).toString();
        QString problem = query.value(1).toString();
        QString status = query.value(2).toString();

        QStandardItem* idItem = new QStandardItem(id);
        QStandardItem* problemItem = new QStandardItem(problem);
        QStandardItem* statusItem = new QStandardItem(status);

        model->appendRow({ idItem, problemItem, statusItem });
    }

    return model;
}

bool Hospital::ChangeAmbulanceAvailability(QString MATRICULE) {
    QSqlQuery query;

    QString SqlQuery = "UPDATE AMBULANCE "
                       "set ETAT = 1  where MATRICULE=:MATRICULE;";
    query.prepare(SqlQuery);

    query.bindValue(":MATRICULE", MATRICULE);

    return query.exec();
}

QSqlQueryModel *Hospital::FetchAllAmbulance() {
    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("Select * from AMBULANCE");
    return model;
}

bool Hospital::AsignAmbuanceToEmergencyCall(int ID, QString MATRICULE) {
    QSqlQuery query;

    QString SqlQuery = "UPDATE EMERGENCYCALL "
                       "set MATRICULE_AMBULANCE = :MATRICULE  where ID=:ID;";
    query.prepare(SqlQuery);

    query.bindValue(":ID", ID);
    query.bindValue(":MATRICULE", MATRICULE);

    return query.exec();
}

bool Hospital::AsignHospitalToEmergencyCall(int ID, int hospitalID) {
    QSqlQuery query;

    QString SqlQuery = "UPDATE EMERGENCYCALL "
                       "set HOSPITAL_ID = :HOSPITAL_ID  where ID=:ID;";
    query.prepare(SqlQuery);

    query.bindValue(":ID", ID);
    query.bindValue(":HOSPITAL_ID", hospitalID);

    return query.exec();
}

QSqlQueryModel *Hospital::FetchAmbulanceByMatricule(QString matricule) {
    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("Select * from AMBULANCE Where MATRICULE = " + matricule);
    return model;
}

QSqlQueryModel *Hospital::FetchHospitalByID(int hospitalID) {
    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery(
        QString("Select * from HOSPITAL Where ID = %1").arg(hospitalID));
    return model;
}
QSqlQueryModel *Hospital::FetchOccupiedEmergencyCalls() {
    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery(
        QString("Select * from EMERGENCYCALL Where status > 2 AND status < 6"));
    return model;
}

bool Hospital::UpdateVitesse(int Vitesse)
{
    QSqlQuery query;

    QString SqlQuery = QString("UPDATE VEHICLE set VITESSE=%1").arg(Vitesse);
    query.prepare(SqlQuery);



    return query.exec();
}

int Hospital::ShowVitesse()
{
    QSqlQuery query;
    query.prepare("Select VITESSE from VEHICLE");
    query.exec();
    if (query.next()) {
            return query.value(0).toInt();
    } else {
        // Handle the case where no pulsation value is found
        return -1;
    }
}


