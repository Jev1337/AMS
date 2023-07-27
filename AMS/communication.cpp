#include "communication.h"
#include <QComboBox>
#include <QCryptographicHash>
#include <QDebug>
#include <QList>
#include <QRegularExpression>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>

communication::communication() {
    this->id = 0;
    this->nom = "";
    this->email = "";
    this->adresse = "";
    this->remarques = "";
    this->num_tel = "";
}

communication::communication(int id, QString nom, QString adresse,
                             QString numtel, QString email, QString remarques) {
    this->id = id;
    this->nom = nom;
    this->email = email;
    this->adresse = adresse;
    this->remarques = remarques;
    this->num_tel = numtel;
}
int communication::getId() { return id; }
QString communication::getNom() { return nom; }
QString communication::getAdresse() { return adresse; }
QString communication::getNum_tel() { return num_tel; }
QString communication::getEmail() { return email; }
QString communication::getRemarques() { return remarques; }
QString communication::getDatefromId(int id){
    QSqlQuery query;
    query.prepare("SELECT CREATED_AT FROM COMMUNICATIONCENTER WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
    query.next();
    return query.value(0).toString();
}
void communication::setID(int id) { this->id = id; }
void communication::setNom(QString nom) { this->nom = nom; }
void communication::setAdresse(QString adresse) { this->adresse = adresse; }
void communication::setNum_tel(QString num_tel) { this->num_tel = num_tel; }
void communication::setEmail(QString email) { this->email = email; }
void communication::setRemarques(QString remarques) {
    this->remarques = remarques;
}
void communication::afficher() {
    QSqlQuery query;
    query.prepare("SELECT ID, NOM, ADRESSE, NUM_TEL, EMAIL, REMARQUES, "
                  "UPDATED_AT FROM COMMUNICATIONCENTER");
    query.bindValue(":id", id);
    query.bindValue(":email", email);
    query.bindValue(":nom", nom);
    query.bindValue(":num_tel", num_tel);
    query.bindValue(":adresse", adresse);
    query.bindValue(":remarques", remarques);
    query.exec();
}
void communication::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE COMMUNICATIONCENTER SET NOM=:nom, ADRESSE=:adresse, "
                  "NUM_TEL=:num_tel, EMAIL=:email, REMARQUES=:remarques, "
                  "UPDATED_AT=CURRENT_TIMESTAMP WHERE ID=:id");
    query.bindValue(":id", id);
    query.bindValue(":email", email);
    query.bindValue(":nom", nom);
    query.bindValue(":num_tel", num_tel);
    query.bindValue(":adresse", adresse);
    query.bindValue(":remarques", remarques);
    query.exec();
}
void communication::supprimer() {
    QSqlQuery query;
    query.prepare("DELETE FROM COMMUNICATIONCENTER WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
}
void communication::ajouter() {
    QSqlQuery query;
    query.prepare("SELECT * FROM COMMUNICATIONCENTER WHERE ID = :id");
    query.bindValue(":id", id);
    query.exec();
    if (!query.next()) {
        query.prepare("INSERT INTO COMMUNICATIONCENTER (ID, NOM, ADRESSE, "
                      "NUM_TEL, EMAIL, REMARQUES, CREATED_AT, UPDATED_AT) "
                      "VALUES (:id, :nom, :adresse, :num_tel, :email, "
                      ":remarques, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)");
        query.bindValue(":id", id);
        query.bindValue(":nom", nom);
        query.bindValue(":num_tel", num_tel);
        query.bindValue(":adresse", adresse);
        query.bindValue(":remarques", remarques);
        query.bindValue(":email", email);
        if (query.exec())
            QMessageBox::information(nullptr, "Success",
                                     "Communication Center has been added!");
        else
            QMessageBox::warning(
                nullptr, "Error",
                "Error: An error has occured whilst executing your query!");

    } else {
        QMessageBox::warning(nullptr, "Error",
                             "Error: A Center exists with the same ID!");
    }
}

void communication::recherche() {}
QString communication::stat() {
    QSqlQuery query;
    query.prepare("SELECT count(*) as total from communicationcenter");
    query.exec();
    query.next();
    return query.value("TOTAL").toString();
}

QStringList communication::getBest(){
    QSqlQuery query;
    query.prepare("SELECT * FROM (SELECT COMMUNICATION_CENTER_ID, COUNT(*) AS count FROM EMERGENCYCALL GROUP BY COMMUNICATION_CENTER_ID ORDER BY count DESC) WHERE ROWNUM <= 1");
    query.exec();
    if (query.next())
        return QStringList({query.value(0).toString(),query.value(1).toString()});
    else return QStringList({"0","0"});
}

QString communication::getFinished(){
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM EMERGENCYCALL WHERE status = 6");
    query.exec();
    query.next();
    return query.value(0).toString();
}

void communication::getEmergencyCallStat(QtCharts::QChartView *chartView) {

        QDate currentDate = QDate::currentDate();
        int currentYear = currentDate.year();

        QtCharts::QBarSeries *barSeries = new QtCharts::QBarSeries();

        QSqlQuery query;
        query.prepare("SELECT TO_CHAR(created_at, 'MM') as hire_month, COUNT(*) as "
                      "num_new_employees "
                      "FROM EMERGENCYCALL "
                      "WHERE TO_CHAR(created_at, 'YYYY') = :year "
                      "GROUP BY TO_CHAR(created_at, 'MM') "
                      "ORDER BY hire_month");
        query.bindValue(":year", currentYear);
        query.exec();

        int numEmployeesPerMonth[12] = {0};

        while (query.next()) {
            QString hireMonth = query.value("hire_month").toString();
            int numNewEmployees = query.value("num_new_employees").toInt();
            int monthIndex = hireMonth.toInt() - 1;
            numEmployeesPerMonth[monthIndex] = numNewEmployees;
        }

        QtCharts::QBarSet *barSet = new QtCharts::QBarSet("Calls", barSeries);
        for (int i = 0; i < 12; ++i) {
            *barSet << numEmployeesPerMonth[i];
        }
        barSeries->append(barSet);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(barSeries);
        chart->setTitle("Total Emergency Calls emitted this Year " +
                        QString::number(currentYear));
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QtCharts::QBarCategoryAxis *axisX = new QtCharts::QBarCategoryAxis();
        axisX->append({"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
                       "Sep", "Oct", "Nov", "Dec"});
        chart->setAxisX(axisX, barSeries);

        QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
        axisY->setTitleText("Number of Emergency Calls");
        chart->setAxisY(axisY, barSeries);

        chartView->setChart(chart);
        chartView->repaint();
        chartView->setRenderHint(QPainter::Antialiasing);

        chartView->resize(491, 291);

        chartView->show();
    }

void communication::pdf() {}
