#include "ambulance.h"
#include <QComboBox>
#include <QCryptographicHash>
#include <QDebug>
#include <QLabel>
#include <QList>
#include <QPixmap>
#include <QRegularExpression>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>

ambulance::ambulance() {
    this->matricule = "";
    this->etat = "";
    this->capacite = 0;
    this->emplacement = "";
    this->equipements = "";
    this->remarques = "";
}
ambulance::ambulance(QString matricule, QString etat, int capacite,
                     QString emplacement, QString equipements,
                     QString remarques) {
    this->matricule = matricule;
    this->etat = etat;
    this->capacite = capacite;
    this->emplacement = emplacement;
    this->equipements = equipements;
    this->remarques = remarques;
}

QString ambulance::get_matricule() { return matricule; }

QString ambulance::get_etat() { return etat; }

int ambulance::get_capacite() { return capacite; }

QString ambulance::get_emplacement() { return emplacement; }

QString ambulance::get_equipements() { return equipements; }

QString ambulance::get_remarques() { return remarques; }

void ambulance::set_matricule(QString matricule) {
    this->matricule = matricule;
}
void ambulance::set_etat(QString etat) { this->etat = etat; }
void ambulance::set_capacite(int capacite) { this->capacite = capacite; }
void ambulance::set_emplacement(QString emplacement) {
    this->emplacement = emplacement;
}
void ambulance::set_equipements(QString equipements) {
    this->equipements = equipements;
}
void ambulance::set_remarques(QString remarques) {
    this->remarques = remarques;
}

void ambulance::afficher() { // QString ambu;
    // QLabel label;
    // QPixmap pixmap;
    QSqlQuery query;
    query.prepare("SELECT MATRICULE, ETAT, CAPACITE, EMPLACEMENT, EQUIPEMENTS, "
                  "REMARQUES, UPDATED_AT FROM AMBULANCE");
    // if(etat=="0")
    //  { QPixmap
    //  pixmap("C:/Users/adamb/OneDrive/Desktop/AMSv/AMSv/assets/images/ambu_noir.png");
    //  }
    // label.setPixmap(pixmap);
    query.bindValue(": Registration Number ", matricule);
    query.bindValue(": State ", etat);
    query.bindValue(": Number od Employees ", capacite);
    query.bindValue(": Location ", emplacement);
    query.bindValue(": Equipment ", equipements);
    query.bindValue(": Remarks ", remarques);
    query.exec();
}
void ambulance::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE AMBULANCE SET ETAT=:etat, CAPACITE=:capacite, "
                  "EMPLACEMENT=:emplacement, EQUIPEMENTS=:equipements, "
                  "REMARQUES=:remarques, UPDATED_AT=CURRENT_TIMESTAMP WHERE "
                  "MATRICULE=:matricule");
    query.bindValue(":matricule", matricule);
    query.bindValue(":etat", etat);
    query.bindValue(":capacite", capacite);
    query.bindValue(":emplacement", emplacement);
    query.bindValue(":equipements", equipements);
    query.bindValue(":remarques", remarques);
    query.exec();
}
void ambulance::supprimer() {
    QSqlQuery query;
    query.prepare("DELETE FROM AMBULANCE WHERE MATRICULE = :matricule");
    query.bindValue(":matricule", matricule);
    query.exec();
}
void ambulance::ajouter() {
    QSqlQuery query;
    query.prepare("SELECT * FROM AMBULANCE WHERE MATRICULE = :matricule");
    query.bindValue(":matricule", matricule);
    query.exec();
    if (!query.next()) {
        query.prepare(
            "INSERT INTO AMBULANCE (MATRICULE, ETAT, CAPACITE, EMPLACEMENT, "
            "EQUIPEMENTS, REMARQUES, CREATED_AT, UPDATED_AT) "
            "VALUES (:matricule, :etat, :capacite, :emplacement, :equipements, "
            ":remarques, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)");
        query.bindValue(":matricule", matricule);
        query.bindValue(":etat", etat);
        query.bindValue(":capacite", capacite);
        query.bindValue(":emplacement", emplacement);
        query.bindValue(":equipements", equipements);
        query.bindValue(":remarques", remarques);
        if (query.exec())
            QMessageBox::information(nullptr, "Success",
                                     "An Ambulance has been added!");
        else
            QMessageBox::warning(
                nullptr, "Error",
                "Error: An error has occured whilst executing your query!");

    } else {
        QMessageBox::warning(
            nullptr, "Error",
            "Error: An Ambulance exists with the same MATRICULE!");
    }
}

QStandardItemModel* ambulance::getPendingConf(){
    auto* model = new QStandardItemModel;

    QSqlQuery query;
    QStandardItem* idHeader = new QStandardItem("ID");
    QStandardItem* problemHeader = new QStandardItem("PROBLEM");
    QStandardItem* MatHeader = new QStandardItem("MATRICULE");

    model->setHorizontalHeaderItem(0, idHeader);
    model->setHorizontalHeaderItem(1, problemHeader);
    model->setHorizontalHeaderItem(2, MatHeader);

    query.prepare("SELECT ID, PROBLEM, MATRICULE_AMBULANCE FROM EMERGENCYCALL WHERE STATUS = 3 or STATUS = 5");
    query.exec();
    while (query.next()) {
        QString id = query.value(0).toString();
        QString problem = query.value(1).toString();
        QString mat = query.value(2).toString();

        QStandardItem* idItem = new QStandardItem(id);
        QStandardItem* problemItem = new QStandardItem(problem);
        QStandardItem* matItem = new QStandardItem(mat);

        model->appendRow({ idItem, problemItem, matItem });
    }

    return model;

}
