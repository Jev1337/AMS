#include "employee.h"

employee::employee() {
}

employee::employee(QString mail, QString password, int mode) {

    QSqlQuery query;
    if (mode == 1) {
        query.prepare("SELECT CIN, TYPE, MAIL, NOM, PRENOM, DATE_NAISSANCE, "
                      "REMARQUES, PASSWORD, ISADMIN, PHONE FROM EMPLOYEE WHERE MAIL = :mail");
        query.bindValue(":mail", mail);
        query.exec();

        if (query.exec() && query.next()) {
            QByteArray hash = QCryptographicHash::hash(
                                  password.toUtf8(), QCryptographicHash::Sha256)
                                  .toHex();
            QString hashedPassword(hash);
            QString storedPassword = query.value(7).toString();
            if (storedPassword == hashedPassword) {
                CIN = query.value("CIN").toString();
                type = query.value("TYPE").toInt();
                this->mail = query.value("MAIL").toString();
                nom = query.value("NOM").toString();
                prenom = query.value("PRENOM").toString();
                datenaiss = query.value("DATE_NAISSANCE").toString();
                remarks = query.value("REMARQUES").toString();
                phone = query.value("PHONE").toString();
                admin = query.value("ISADMIN").toInt();
            } else
                type = -1;
        } else {
            qDebug() << "Query returned no rows";
            type = -2;
            return;
        }
    } else if (mode == 2) {
        query.prepare("SELECT CIN, TYPE, MAIL, NOM, PRENOM, DATE_NAISSANCE, "
                      "REMARQUES, PASSWORD, ISADMIN, PHONE FROM EMPLOYEE WHERE MAIL = :mail");
        query.bindValue(":mail", mail);
        if (query.exec() && query.next()) {
            CIN = query.value("CIN").toString();
            type = query.value("TYPE").toInt();
            this->mail = query.value("MAIL").toString();
            nom = query.value("NOM").toString();
            prenom = query.value("PRENOM").toString();
            datenaiss = query.value("DATE_NAISSANCE").toString();
            remarks = query.value("REMARQUES").toString();
            admin = query.value("ISADMIN").toInt();
            phone = query.value("PHONE").toString();
        }
        query.prepare("UPDATE EMPLOYEE SET Face_ID = :faceid , Face_TS "
                      "=CURRENT_TIMESTAMP WHERE MAIL = :mail");
        query.bindValue(":mail", mail);
        query.bindValue(":faceid", password);
        query.exec();

    } else if (mode == 3) {
        query.prepare(
            "SELECT CIN, TYPE, MAIL, NOM, PRENOM, DATE_NAISSANCE, REMARQUES, "
            "PASSWORD, ISADMIN, PHONE FROM EMPLOYEE WHERE FINGERPRINT_ID = :id");
        query.bindValue(":id", password);
        if (query.exec() && query.next()) {
            CIN = query.value("CIN").toString();
            type = query.value("TYPE").toInt();
            this->mail = query.value("MAIL").toString();
            nom = query.value("NOM").toString();
            prenom = query.value("PRENOM").toString();
            datenaiss = query.value("DATE_NAISSANCE").toString();
            remarks = query.value("REMARQUES").toString();
            admin = query.value("ISADMIN").toInt();
            phone = query.value("PHONE").toString();
        } else
            type = -2;
    }
}

QString employee::GetCurrentFaceID(QString email) {

    QSqlQuery query;
    query.prepare("SELECT FACE_ID FROM EMPLOYEE WHERE MAIL = :mail AND FACE_TS "
                  "> SYSDATE - INTERVAL '28' DAY");
    query.bindValue(":mail", email);
    query.exec();
    if (query.next())
        return query.value(0).toString();
    return "";
}
QString employee::getEmpAvgAge() {
    QSqlQuery query;
    query.prepare("SELECT AVG(TRUNC(MONTHS_BETWEEN(SYSDATE, "
                  "date_naissance)/12)) as avg_age FROM Employee");
    query.exec();
    if (query.next()) {
        double avgAge = query.value("avg_age").toDouble();
        return QString::number(avgAge, 'f', 1) + " years";
    }
    return "N/A";
}

QString employee::getEmpMonth() {
    QDate currentDate = QDate::currentDate();
    QString currentMonth = currentDate.toString("MM");

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) as num_employees FROM Employee WHERE "
                  "TO_CHAR(created_at, 'MM') = :month");
    query.bindValue(":month", currentMonth);

    if (!query.exec()) {
        qDebug() << "Unable to execute query:" << query.lastError().text();
        return QString();
    }

    if (query.next()) {
        int numEmployees = query.value("num_employees").toInt();
        return QString::number(numEmployees);
    }

    return "N/A";
}

QString employee::getTotalEmp() {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) as num_employees FROM Employee");
    query.exec();
    if (query.next()) {
        int numEmployees = query.value("num_employees").toInt();
        return QString::number(numEmployees);
    }
    return "N/A";
}

double employee::getEmpIncreasePercent() {
    // Get the current month and year
    QDate currentDate = QDate::currentDate();
    int currentYear = currentDate.year();
    int currentMonth = currentDate.month();

    // Get the new employee count for the current month
    int currentMonthEmpCount = getEmpMonth().toInt();

    // Get the new employee count for the previous month
    int prevMonthEmpCount = 0;
    QDate prevMonthDate(currentYear, currentMonth - 1, 1);
    if (prevMonthDate.isValid()) {
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) as num_new_employees "
                      "FROM Employee "
                      "WHERE TO_CHAR(created_at, 'YYYY-MM') = :prevMonth");
        query.bindValue(":prevMonth", prevMonthDate.toString("yyyy-MM"));
        if (query.exec() && query.next()) {
            prevMonthEmpCount = query.value("num_new_employees").toInt();
        }
    }

    // Calculate the percentage increase
    double percentIncrease = 0;
    if (prevMonthEmpCount > 0) {
        percentIncrease = (currentMonthEmpCount - prevMonthEmpCount) * 100.0 /
                          prevMonthEmpCount;
    }

    return percentIncrease;
}

QString employee::getCINfromEMAIL(QString email){
    QSqlQuery query;
    query.prepare("SELECT CIN FROM EMPLOYEE WHERE MAIL= :email");
    query.bindValue(":email", email);
    query.exec();
    qDebug() << query.executedQuery();
    query.next();
    return query.value(0).toString();
}
void employee::chartRefresh(QtCharts::QChartView *chartView) {
    QDate currentDate = QDate::currentDate();
    int currentYear = currentDate.year();

    QtCharts::QBarSeries *barSeries = new QtCharts::QBarSeries();

    QSqlQuery query;
    query.prepare("SELECT TO_CHAR(created_at, 'MM') as hire_month, COUNT(*) as "
                  "num_new_employees "
                  "FROM Employee "
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

    QtCharts::QBarSet *barSet = new QtCharts::QBarSet("Employees", barSeries);
    for (int i = 0; i < 12; ++i) {
        *barSet << numEmployeesPerMonth[i];
    }
    barSeries->append(barSet);

    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(barSeries);
    chart->setTitle("New Employees Hired Per Month on " +
                    QString::number(currentYear));
    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

    QtCharts::QBarCategoryAxis *axisX = new QtCharts::QBarCategoryAxis();
    axisX->append({"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
                   "Sep", "Oct", "Nov", "Dec"});
    chart->setAxisX(axisX, barSeries);

    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setTitleText("Number of New Employees");
    chart->setAxisY(axisY, barSeries);

    chartView->setChart(chart);
    chartView->repaint();
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->resize(491, 291);

    chartView->show();
}

employee::employee(QString CIN, int type, QString mail, QString nom,
                   QString prenom, QString datenaiss, QString remarks, QString phone,
                   int admin) {
    this->CIN = CIN;
    this->type = type;
    this->mail = mail;
    this->nom = nom;
    this->prenom = prenom;
    this->datenaiss = datenaiss;
    this->remarks = remarks;
    this->phone = phone;
    this->admin = admin;
}
QString employee::getCIN() { return CIN; }
int employee::getType() { return type; }
QString employee::getMail() { return mail; }
QString employee::getNom() { return nom; }
QString employee::getPrenom() { return prenom; }
QString employee::getDateNaiss() { return datenaiss; }
QString employee::getRemarks() { return remarks; }
void employee::setCIN(QString CIN) { this->CIN = CIN; }
void employee::setType(int type) { this->type = type; }
void employee::setMail(QString mail) { this->mail = mail; }
void employee::setNom(QString nom) { this->nom = nom; }
void employee::setPrenom(QString prenom) { this->prenom = prenom; }
void employee::setDateNaiss(QString datenaiss) { this->datenaiss = datenaiss; }
void employee::setRemarks(QString remarks) { this->remarks = remarks; }
bool employee::getAccountTypes(QComboBox *comboBox) {
    QSqlQuery query;
    bool test = query.exec("SELECT DESCRIPTION FROM ACCOUNTTYPE ORDER BY TYPE");
    while (query.next()) {
        QString description = query.value(0).toString();
        comboBox->addItem(description);
    }
    return test;
}

bool employee::getDeps(int id, QComboBox *comboBox) {
    QSqlQuery query;
    bool test = false;
    if (id == 2)
        test = query.exec("SELECT ID FROM COMMUNICATIONCENTER");
    if (id == 3)
        test = query.exec("SELECT ID FROM HOSPITAL");
    if (id == 5)
        test = query.exec("SELECT MATRICULE FROM AMBULANCE");
    while (query.next()) {
        QString description = query.value(0).toString();
        comboBox->addItem(description);
    }
    return test;
}
QSqlRelationalTableModel *employee::getAccountList() {
    auto *model = new QSqlRelationalTableModel();
    model->setTable("employee");
    model->setRelation(
        1, QSqlRelation("accounttype", "type",
                        "description")); // assuming the foreign key column is
                                         // at index 1 and the description
                                         // column is named "description"
    model->removeColumn(model->fieldIndex("password"));
    model->removeColumn(model->fieldIndex("created_at"));
    model->removeColumn(model->fieldIndex("updated_at"));
    model->removeColumn(model->fieldIndex("face_id"));
    model->removeColumn(model->fieldIndex("face_ts"));
    model->removeColumn(model->fieldIndex("fingerprint_date"));
    model->removeColumn(model->fieldIndex("fingerprint_id"));
    model->removeColumn(model->fieldIndex("isadmin"));
    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Type");
    model->setHeaderData(2, Qt::Horizontal, "E-Mail");
    model->setHeaderData(3, Qt::Horizontal, "Name");
    model->setHeaderData(4, Qt::Horizontal, "Last Name");
    model->setHeaderData(5, Qt::Horizontal, "Date of Birth");
    model->setHeaderData(6, Qt::Horizontal, "Phone Number");
    model->setHeaderData(7, Qt::Horizontal, "Remarks");
    model->select();
    return model;
}

QStandardItemModel * employee::getMessages(){
    auto* model = new QStandardItemModel;

    QStringList lines = remarks.split("\n");

    for (int i = 0; i < lines.size() - 1; ++i) {
            QStandardItem* item = new QStandardItem(lines.at(i));
            model->appendRow(item);
    }

    QStandardItem* finalItem = new QStandardItem(lines.last());
       model->appendRow(finalItem);


    /*auto* model = new QSqlTableModel();
    model->setTable("employee");
    model->setFilter("CIN = "+this->CIN);
    model->setSort(model->fieldIndex("UPDATED_AT"), Qt::DescendingOrder);
    model->removeColumn(model->fieldIndex("password"));
    model->removeColumn(model->fieldIndex("created_at"));
    model->removeColumn(model->fieldIndex("updated_at"));
    model->removeColumn(model->fieldIndex("face_id"));
    model->removeColumn(model->fieldIndex("face_ts"));
    model->removeColumn(model->fieldIndex("fingerprint_date"));
    model->removeColumn(model->fieldIndex("fingerprint_id"));
    model->removeColumn(model->fieldIndex("isadmin"));
    model->removeColumn(model->fieldIndex("CIN"));
    model->removeColumn(model->fieldIndex("Type"));
    model->removeColumn(model->fieldIndex("mail"));
    model->removeColumn(model->fieldIndex("nom"));
    model->removeColumn(model->fieldIndex("prenom"));
    model->removeColumn(model->fieldIndex("date_naissance"));
    model->removeColumn(model->fieldIndex("phone"));
    model->setHeaderData(0, Qt::Horizontal, this->nom);
    model->select();
    */
    return model;
}

bool employee::addSelf(QString password) {
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYEE (CIN, TYPE, MAIL, PASSWORD, NOM, "
                  "PRENOM, DATE_NAISSANCE, REMARQUES, ISADMIN, PHONE, CREATED_AT, UPDATED_AT) "
                  "VALUES (:cin, :type, :mail, :password, :nom, :prenom, "
                  "TO_DATE(:dateNaiss, 'YYYY-MM-DD'), :remarks, "
                  ":admin, :phone, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)");
    query.bindValue(":cin", CIN);
    query.bindValue(":type", type + 1);
    query.bindValue(":mail", mail);
    QByteArray hash =
        QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256)
            .toHex();
    QString hashedPassword(hash);
    query.bindValue(":password", hashedPassword);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateNaiss", datenaiss);
    query.bindValue(":remarks", remarks);
    query.bindValue(":admin", admin);
    query.bindValue(":phone", phone);
    return query.exec();
}
bool employee::removeSelf() {
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYEE WHERE CIN = :cin");
    query.bindValue(":cin", CIN);
    return query.exec();
}
bool employee::updateSelf() {
    QSqlQuery query;
    query.prepare(
        "UPDATE EMPLOYEE SET TYPE=:type, MAIL=:mail, NOM=:nom, PRENOM=:prenom, "
        "DATE_NAISSANCE=TO_DATE(:dateNaiss, 'YYYY-MM-DD'), REMARQUES=:remarks, phone=:phone, "
        "UPDATED_AT=CURRENT_TIMESTAMP, ISADMIN = :admin WHERE CIN=:cin");
    query.bindValue(":type", type + 1);
    query.bindValue(":mail", mail);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateNaiss", datenaiss);
    query.bindValue(":remarks", remarks);
    query.bindValue(":admin", admin);
    query.bindValue(":cin", CIN);
    query.bindValue(":phone", phone);
    return query.exec();
    ;
}
bool employee::changePw(QString newPW) {
    QByteArray hash =
        QCryptographicHash::hash(newPW.toUtf8(), QCryptographicHash::Sha256)
            .toHex();
    QString hashedPassword(hash);
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET PASSWORD = :password , "
                  "UPDATED_AT=CURRENT_TIMESTAMP WHERE CIN = :CIN");
    query.bindValue(":password", hashedPassword);
    query.bindValue(":CIN", CIN);
    return query.exec();
}

bool employee::setFaceID(QString FaceID) {
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET FACE_ID = :faceid , "
                  "FACE_TS=CURRENT_TIMESTAMP WHERE CIN = :CIN");
    query.bindValue(":faceid", FaceID);
    query.bindValue(":CIN", CIN);
    return query.exec();
}

bool employee::setFingerprint(int fingerprint) {
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET FINGERPRINT_ID = :fp , "
                  "FINGERPRINT_DATE=CURRENT_TIMESTAMP WHERE CIN = :CIN");
    query.bindValue(":fp", fingerprint);
    query.bindValue(":CIN", CIN);
    return query.exec();
}

int employee::GetLastFingerprintID() {
    QSqlQuery query;
    query.prepare("SELECT MAX(FINGERPRINT_ID) FROM EMPLOYEE");
    query.exec();
    if (query.next())
        return query.value(0).toInt();
    else
        return -1;
}

bool employee::cinExistance(QString CIN) {
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYEE WHERE cin = :CIN");
    query.bindValue(":CIN", CIN);
    query.exec();
    return query.next();
}
bool employee::emailExistance(QString email) {
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYEE WHERE mail = :email");
    query.bindValue(":email", email);
    query.exec();
    return query.next();
}

bool EmailValidator(QString email) {
    QRegularExpression re(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    QRegularExpressionMatch match = re.match(email);
    return match.hasMatch();
}

bool PasswordValidator(QString password) {
    QRegularExpression re("^(?=.*\\d)(?=.*[a-zA-Z]).{8,}$");
    QRegularExpressionMatch match = re.match(password);
    return match.hasMatch();
}
bool CINValidator(QString cin) {
    QRegularExpression re("^[0-9]{8,}$");
    QRegularExpressionMatch match = re.match(cin);
    return match.hasMatch();
}

bool NameValidator(QString name) {
    QRegularExpression re("^[a-zA-Z]{3,20}$");
    QRegularExpressionMatch match = re.match(name);
    return match.hasMatch();
}

int employee::getAdmin(){
    return admin;
}

int employee::getCurrentFingerprint(){
    QSqlQuery query;
    query.prepare("SELECT FINGERPRINT_ID FROM EMPLOYEE WHERE cin = :CIN");
    query.bindValue(":CIN", CIN);
    query.exec();
    if(query.next())
        return query.value(0).toInt();
    return 0;

}
void employee::setAdmin(bool isAdmin){
    admin = isAdmin;
}

void employee::setPhone(QString phone){
    this->phone = phone;
}

QString employee::getPhone(){
    return phone;
}

QString employee::getPHONEfromEMAIL(QString email){
    QSqlQuery query;
    query.prepare("SELECT PHONE FROM EMPLOYEE WHERE MAIL= :email");
    query.bindValue(":email", email);
    query.exec();
    qDebug() << query.executedQuery();
    query.next();
    return query.value(0).toString();
}


QStringList employee::getArds(){
    return ards;
}

