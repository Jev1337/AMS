#include "dashboard_hospital.h"
#include "ui_dashboard_hospital.h"
#include <QDesktopServices>
#include <QPainter>
#include <QPdfWriter>
#include <QUrl>
#include <QtDebug>
#include <QFileDialog>

#include "hospital.h"
#include "selectmap.h"

dashboard_hospital::dashboard_hospital(employee &e, QWidget *parent) :
    CFramelessWindow(parent),
    ui(new Ui::dashboard_hospital)
{
    ui->setupUi(this);
    this->e = e;
    admin = e.getAdmin();

    ui->lineEdit_6->setText(e.getCIN());
    ui->lineEdit_3->setText(e.getPrenom());
    ui->lineEdit_4->setText(e.getNom());
    ui->lineEdit_8->setText(e.getMail());
    ui->greetingIns->setText(this->e.getNom() + " " + this->e.getPrenom());
    ui->Greeting->setText(ui->Greeting->text().append(this->e.getNom() + "!"));
    setTitleBar(ui->widget_2);
    ui->dropdownWidget->installEventFilter(this);
    ui->widget->installEventFilter(this);
    ui->centralwidget->installEventFilter(this);
    ui->notificationWidget->installEventFilter(this);

    connect(ui->tableView_4, SIGNAL(clicked(const QModelIndex &)), this,
            SLOT(onTableHospitalClicked(const QModelIndex &)));

    auto obj = ui->quickWidget->rootObject();

    connect(this, SIGNAL(setRoute(QVariant, QVariant, QVariant, QVariant)), obj, SLOT(setRoute(QVariant, QVariant, QVariant, QVariant)));
    connect(this, SIGNAL(setCenter(QVariant, QVariant)), obj, SLOT(setCenter(QVariant, QVariant)));
    connect(this, SIGNAL(addMarker(QVariant, QVariant)), obj, SLOT(addMarker(QVariant, QVariant)));
    connect(this,SIGNAL(addHospitalMarker(QVariant, QVariant)),obj,SLOT(addHospitalMarker(QVariant, QVariant)));
    connect(this,SIGNAL(addAmbulanceMarker(QVariant, QVariant)),obj,SLOT(addAmbulanceMarker(QVariant, QVariant)));
    connect(this,SIGNAL(addPatientMarker(QVariant, QVariant,QVariant)),obj,SLOT(addPatientMarker(QVariant, QVariant,QVariant)));

    connect(this, SIGNAL(addAmbulanceOccupiedMarker(QVariant, QVariant)), obj,
            SLOT(addAmbulanceOccupiedMarker(QVariant, QVariant)));
    connect(this, SIGNAL(addPatientOccupiedMarker(QVariant, QVariant)), obj,
            SLOT(addPatientOccupiedMarker(QVariant, QVariant)));

    emit setCenter(36.900068737969, 10.189164289212641);

    refresh();

    // voice of welcoming
    QTextToSpeech *tts = new QTextToSpeech(this);
    tts->setLocale(QLocale(QLocale::English));
    tts->setVoice(tts->availableVoices().at(1));
    tts->setPitch(20);
    QString employee;
    employee =
        "Hello" + e.getNom() + ", Welcome to Ambulance Management System!";
    if (ui->ttsenable->isChecked())
    tts->say(employee);

   QProcess pingProcess;
       QStringList args;
       args << "-n" << "1" << "-w" << "300" << "1.1.1.1";
       pingProcess.start("ping", args);

       if (pingProcess.waitForFinished()) {
           QByteArray output = pingProcess.readAll();
           QString outputStr(output);

           QRegularExpression re("Minimum = (\\d+)ms");
           QRegularExpressionMatch match = re.match(outputStr);
           if (match.hasMatch()) {
               QString minimum = match.captured(1);
               ui->stat1_4->setText(minimum + " ms");
           } else {
               qDebug() << "Failed to extract minimum round trip time.";
               QMessageBox::warning(nullptr, "Error", "Failed to extract latency: Please make sure you are connected to the internet!");
               return;
           }
       }

   QElapsedTimer timerCalcul;
   timerCalcul.start();
   QSqlQuery query;
   query.prepare("SELECT * FROM EMPLOYEE");
   query.exec();
   ui->stat1_2->setText(QString::number(timerCalcul.elapsed()) + "."+ QString::number(timerCalcul.nsecsElapsed())[0] + "ms" );
   timerCalcul.restart();
   QNetworkAccessManager manager;
   QUrl url("https://api.openai.com");
   QNetworkReply *reply = manager.get(QNetworkRequest(url));
   QEventLoop loop;
   QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
   loop.exec();
   qint64 ms = timerCalcul.elapsed();
   ui->stat1_5->setText(QString::number(ms -118) + " ms");
   reply->deleteLater();

   timerCalcul.restart();
   QUrl url1("https://dynamodb.us-west-1.amazonaws.com");
   QNetworkReply *reply1 = manager.get(QNetworkRequest(url1));
   QEventLoop loop1;
   QObject::connect(reply1, &QNetworkReply::finished, &loop1, &QEventLoop::quit);
   loop1.exec();
   qint64 ms1 = timerCalcul.elapsed();
   ui->stat1_6->setText(QString::number(ms1- 118) + " ms");
   reply1->deleteLater();

   timerCalcul.restart();
   QUrl url2("https://westeurope.stt.speech.microsoft.com");
   QNetworkReply *reply2 = manager.get(QNetworkRequest(url2));
   QEventLoop loop2;
   QObject::connect(reply2, &QNetworkReply::finished, &loop2, &QEventLoop::quit);
   loop2.exec();
   qint64 ms2 = timerCalcul.elapsed();
   ui->stat1_3->setText(QString::number(ms2-118) + " ms");
   reply2->deleteLater();

}

void dashboard_hospital::showArdStats(QString message){

    bool first = A.isOpen();
    int ret = 0;

    if (!first){
        ret = A.connect_arduino(FINGERPRINT_PORT); // lancer la connexion à arduino
        switch (ret) {
        case (0):
            qDebug() << "arduino is available and connected to : "
                     << A.getarduino_port_name();
            break;
        case (1):
            qDebug() << "given arduino is not available";
            return;
            break;

        }
     } else
        if(A.getarduino_port_name() != FINGERPRINT_PORT){
            first = false;
            A.close_arduino();
            ret = A.connect_arduino(FINGERPRINT_PORT); // lancer la connexion à arduino
            switch (ret) {
            case (0):
                qDebug() << "arduino is available and connected to : "
                         << A.getarduino_port_name();
                break;
            case (1):
                qDebug() << "given arduino is not available";
                return;
                break;
            }
        }
        if (ret == 0){
            if (!first)
            QTimer::singleShot(3000, [this, message]() {

                        A.write_to_arduino("l" + message.toUtf8());
                    });
                    else
                        A.write_to_arduino(message.toUtf8());

        }
}


bool dashboard_hospital::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->centralwidget) {
        if (event->type() == QEvent::MouseButtonPress) {
            if (ui->widget->geometry().width() != 51 &&
                ui->widget->geometry().width() == 211) {
                QPropertyAnimation *animation =
                    new QPropertyAnimation(ui->widget, "geometry");
                animation->setStartValue(QRect(0, 20, 211, 561));
                animation->setEndValue(QRect(0, 20, 51, 561));
                animation->setDuration(500);
                animation->setEasingCurve(QEasingCurve::OutCubic);
                animation->start();
            }
        }
    }
    if (watched == ui->dropdownWidget) {
        if (event->type() == QEvent::Leave) {
            qDebug() << "Left dropdownWidget";
            toggleDropdownWidget();
        }
    } else if (watched == ui->widget) {
        if (event->type() == QEvent::Leave) {
            if (ui->widget->geometry().width() != 51 &&
                ui->widget->geometry().width() == 211) {
                QPropertyAnimation *animation =
                    new QPropertyAnimation(ui->widget, "geometry");
                animation->setStartValue(QRect(0, 20, 211, 561));
                animation->setEndValue(QRect(0, 20, 51, 561));
                animation->setDuration(500);
                animation->setEasingCurve(QEasingCurve::OutCubic);
                animation->start();
            }
        }
    } else if (watched == ui->notificationWidget){
        if (event->type() == QEvent::Leave) {
            toggleNotificationWidget();
        }
    }
    return false;
}

dashboard_hospital::~dashboard_hospital() { delete ui; }

void dashboard_hospital::assignAmbulanceToEmergency(int id) {
    QSqlQuery query1;
    query1.prepare("SELECT Emplacement, Matricule FROM Ambulance WHERE ETAT = 0");
    query1.exec();
    if (!query1.next()){
        QMessageBox::warning(
            nullptr, "Error",
            "Sorry! There are no ambulances available!");
        return;
    }
    QSqlQuery query2;
    query2.prepare("SELECT Adresse FROM EmergencyCall WHERE id = :id");
    query2.bindValue(":id", id);
    query2.exec();
    query2.next();
    QString nearestMatricule;
    double nearestDistance = std::numeric_limits<double>::max();
    QStringList locationPartsEme = query2.value(0).toString().split(',');
    while (query1.next()){

        QStringList locationPartsAmb = query1.value(0).toString().split(',');
        if (locationPartsAmb.size() == 2 && locationPartsEme.size() == 2) {
                    double latitude1 = locationPartsAmb[0].toDouble();
                    double longitude1 = locationPartsAmb[1].toDouble();
                    double latitude2 = locationPartsEme[0].toDouble();
                    double longitude2 = locationPartsEme[1].toDouble();

                    // Calculate the distance between current ambulance location and target location
                    QGeoCoordinate coord1(latitude1, longitude1);
                    QGeoCoordinate coord2(latitude2, longitude2);

                    double distance = coord1.distanceTo(coord2);

                    // Update the nearestMatricule if the current distance is smaller
                    if (distance < nearestDistance) {
                        nearestMatricule = query1.value(1).toString();
                        nearestDistance = distance;
                    }

                }
    }

    QSqlQuery query3;
    query3.prepare("UPDATE EmergencyCall SET MATRICULE_AMBULANCE = :mat , STATUS = 3 WHERE id = :id");
    query3.bindValue(":id", id);
    query3.bindValue(":mat", nearestMatricule);
    query3.exec();

    QSqlQuery query4;
    query4.prepare("UPDATE Ambulance SET ETAT = 1 WHERE Matricule = :mat");
    query4.bindValue(":mat", nearestMatricule);
    query4.exec();

    refresh();

}

void dashboard_hospital::assignAmbulanceToHospital(int id) {
    QSqlQuery query1;
    query1.prepare("SELECT Adresse, id FROM Hospital");
    query1.exec();
    if (!query1.next())
        QMessageBox::warning(
            nullptr, "Error",
            "Sorry! There are no hospitals available!");
        return;
    QSqlQuery query2;
    query2.prepare("SELECT Adresse FROM EmergencyCall WHERE id = :id");
    query2.bindValue(":id", id);
    query2.exec();
    query2.next();
    QString nearestMatricule;
    double nearestDistance = std::numeric_limits<double>::max();
    QStringList locationPartsEme = query2.value(0).toString().split(',');
    while (query1.next()){

        QStringList locationPartsAmb = query1.value(0).toString().split(',');
        if (locationPartsAmb.size() == 2 && locationPartsEme.size() == 2) {
                    double latitude1 = locationPartsAmb[0].toDouble();
                    double longitude1 = locationPartsAmb[1].toDouble();
                    double latitude2 = locationPartsEme[0].toDouble();
                    double longitude2 = locationPartsEme[1].toDouble();

                    // Calculate the distance between current ambulance location and target location
                    QGeoCoordinate coord1(latitude1, longitude1);
                    QGeoCoordinate coord2(latitude2, longitude2);

                    double distance = coord1.distanceTo(coord2);

                    // Update the nearestMatricule if the current distance is smaller
                    if (distance < nearestDistance) {
                        nearestMatricule = query1.value(1).toString();
                        nearestDistance = distance;
                    }
                }
    }

    QSqlQuery query3;
    query3.prepare("UPDATE EmergencyCall SET HOSPITAL_ID = :mat , STATUS = 5 WHERE id = :id");
    query3.bindValue(":id", id);
    query3.bindValue(":mat", nearestMatricule);
    query3.exec();

    refresh();

}


void dashboard_hospital::refresh() {
    ui->label_total_hospitals->setText(
        QString("%1").arg(Htmp.totalyHospitals()));
    ui->label_average_hospitalcapacity->setText(
        QString("%1").arg(Htmp.averageHospitalsCapacity()));

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel;
    proxyModel->setSourceModel(Htmp.Show());
    ui->tableView_4->setModel(proxyModel);




    // load all the emergency calls in live map
    QSqlQueryModel *EmergencyCalls = Htmp.FetchEmergencyCalls();

    for (int i = 0; i < EmergencyCalls->rowCount(); i++) {
        QString address = EmergencyCalls->index(i, 6).data().toString();
        QStringList latLongList = address.split(",");

        if (latLongList.size() >=
            2) { // make sure the list has at least 2 elements
            addPatientMarker(latLongList[0], latLongList[1],
                             EmergencyCalls->index(i, 0).data().toInt());
        }
    }

    // load all the hospitals in live map
    for (int i = 0; i < ui->tableView_4->model()->rowCount(); i++) {
        QString address =
            ui->tableView_4->model()->index(i, 2).data().toString();
        QStringList latLongList = address.split(",");
        if (latLongList.size() >=
            2) { // make sure the list has at least 2 elements
            addHospitalMarker(latLongList[0], latLongList[1]);
        }
    }


    // load all the available ambulance in live map
    QSqlQueryModel *availableAmbulance = Htmp.FetchAvailableAmbulance();
    for (int i = 0; i < availableAmbulance->rowCount(); i++) {
        QString address = availableAmbulance->index(i, 3).data().toString();
        QStringList latLongList = address.split(",");
        if (latLongList.size() >=
            2) { // make sure the list has at least 2 elements
            addAmbulanceMarker(QString::number(latLongList[0].toDouble() + 0.00045), latLongList[1]);
        }
    }



    // load all the occupied calls in live map
    QSqlQueryModel *EmergencyOccupiedCalls = Htmp.FetchOccupiedEmergencyCalls();
    for (int i = 0; i < EmergencyOccupiedCalls->rowCount(); i++) {
        QString addressOfEmergency =
            EmergencyOccupiedCalls->index(i, 6).data().toString();
        QStringList latLongListOfEmergency = addressOfEmergency.split(",");

        QString addressOfAmbulance =
            Htmp.FetchAmbulanceByMatricule(
                    EmergencyOccupiedCalls->index(i, 3).data().toString())
                ->index(0, 3)
                .data()
                .toString();
        QStringList latLongListOfAmbulance = addressOfAmbulance.split(",");
        qDebug()<<latLongListOfAmbulance.size();



        if (latLongListOfEmergency.size() >= 2) { // make sure the list has at least 2 elements
            addPatientOccupiedMarker(latLongListOfEmergency[0],
                                     latLongListOfEmergency[1]);
        }

        if (latLongListOfAmbulance.size() >= 2) { // make sure the list has at least 2 elements
            addAmbulanceOccupiedMarker(latLongListOfAmbulance[0],
                                       latLongListOfAmbulance[1]);

        }

    }
    // load chart

    QStringList categories;
    categories << "Hospitals"
               << "Average Hospital Capacity";
    QtCharts::QBarSet *set = new QtCharts::QBarSet("Numbre");
    *set << Htmp.totalyHospitals() << Htmp.averageHospitalsCapacity();

    QtCharts::QBarSeries *series = new QtCharts::QBarSeries(this);
    series->append(set);

    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Hospital Average Stats");
    chart->createDefaultAxes();

    QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
    axis->append(categories);
    chart->setAxisX(axis, series);

    QtCharts::QChartView *m_chartView = new QtCharts::QChartView(chart, this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_chartView);

    qDeleteAll(ui->chart->children());
    ui->chart->setLayout(layout);

    QStandardItemModel *model1 = Htmp.getNonAssignedEmergencyCalls();
    ui->tableView_5->setModel(model1);
    ui->gridLayout_10->addWidget(ui->tableView_5, 0, 0);
    ui->pushButton_35->setText(QString::number(model1->rowCount()));


    QStandardItemModel *model2 = Htmp.getAssignedEmergencyCalls();
    ui->tableView_6->setModel(model2);
    ui->gridLayout_12->addWidget(ui->tableView_6, 0, 0);


    showArdStats("[HTMP] Total = " + QString::number(Htmp.totalyHospitals()) + " Avg Cap = " + QString::number(Htmp.averageHospitalsCapacity()));

}

void dashboard_hospital::FollowEmergency(QGeoCoordinate, QGeoCoordinate) {

}

void dashboard_hospital::onTableHospitalClicked(const QModelIndex &index) {
    selected_hospital = index.row();
}

void dashboard_hospital::on_pushButton_released() { QApplication::quit(); }

void dashboard_hospital::on_pushButton_27_released() {
    // do nothing
}

void dashboard_hospital::changeToMain(int index){
    ui->stackedWidget_4->setCurrentIndex(4);
    QTimer *timer = new QTimer();
    QMovie *movie = new QMovie(":/assets/images/preloader.gif");
    ui->label_3->setMovie(movie);
    movie->start();
    QObject::connect(timer, &QTimer::timeout, [=]() {
        ui->stackedWidget_4->setCurrentIndex(index);
        timer->stop();
        timer->deleteLater();
        movie->stop();
        movie->deleteLater();
    });
    timer->start(300);
}

void dashboard_hospital::on_pushButton_28_released() {
    changeToMain(1);
}

void dashboard_hospital::on_pushButton_29_released() {
    changeToMain(2);
}

void dashboard_hospital::on_pushButton_30_released() {
    changeToMain(3);
}

void dashboard_hospital::on_pushButton_36_released() {
    changeToMain(0);
}

void dashboard_hospital::on_pushButton_38_released() {
    changeToMain(2);
}

void dashboard_hospital::on_pushButton_39_released() {
    changeToMain(3);
}

void dashboard_hospital::on_pushButton_120_released() {
    changeToMain(0);
}

void dashboard_hospital::on_pushButton_121_released() {
    changeToMain(1);
}

void dashboard_hospital::on_pushButton_123_released() {
    changeToMain(3);
}

void dashboard_hospital::on_pushButton_124_released() {
    changeToMain(0);
}

void dashboard_hospital::on_pushButton_125_released() {
    changeToMain(1);
}

void dashboard_hospital::on_pushButton_126_released() {
    changeToMain(2);
}

void dashboard_hospital::on_pushButton_31_released() {
    bool exec = false;
    QPropertyAnimation *animation =
        new QPropertyAnimation(ui->widget, "geometry");
    if (ui->widget->geometry().width() != 211 &&
        ui->widget->geometry().width() == 51) {
        animation->setStartValue(QRect(0, 20, 51, 561));
        animation->setEndValue(QRect(0, 20, 211, 561));
        exec = true;
    } else if (ui->widget->geometry().width() != 51 &&
               ui->widget->geometry().width() == 211) {
        animation->setStartValue(QRect(0, 20, 211, 561));
        animation->setEndValue(QRect(0, 20, 51, 561));
        exec = true;
    }
    if (exec) {
        animation->setDuration(500);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->start();
    }
    /*QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=](){
            if (ui->widget->geometry().width() != 51 &&
    ui->widget->geometry().width() == 211){ animation->setStartValue(QRect(0,
    20, 211, 561)); animation->setEndValue(QRect(0, 20, 51, 561));
                animation->start();
            }
            timer->stop();
    });
    timer->start(5000); // 5000ms = 5s
    */
}

void dashboard_hospital::on_logOut_released() {
    if (ui->pushButton_16->isChecked())
        on_pushButton_16_toggled(false);
    A.close_arduino();
    QTimer::singleShot(0, this, SLOT(close()));
    auto w = new MainWindow();
    w->show();
}

void dashboard_hospital::on_dashboardBtn_released() {
    if (ui->stackedWidget->currentIndex() != 0) {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void dashboard_hospital::on_CUDBtn_released() {
    if (ui->stackedWidget->currentIndex() != 1) {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void dashboard_hospital::on_ChatBtn_released() {
    if (ui->stackedWidget->currentIndex() != 2) {
        ui->stackedWidget->setCurrentIndex(2);
    }
}

void dashboard_hospital::sendOpenAIRequest() {
    QString userInput = ui->lineEdit_7->text();
    if (!userInput.isEmpty()) {
        // Append user input to conversation
        QString userMessage = "<div style=' border-radius: 10px;"
                              "padding: 10px;'><strong>User:</strong> " +
                              userInput + "</div>";
        ui->textEdit->append(userMessage);
        QJsonArray conversationJsonArray;
        QJsonObject messageJsonObject{
            {"role", "system"},
            {"content",
             "You are an Assistant in an Ambulance Management System"}};
        conversationJsonArray.append(messageJsonObject);
        // Iterate over each line in the QTextEdit
        for (QString line : ui->textEdit->toPlainText().split("\n")) {
            line = line.trimmed();

            if (line.startsWith("User: ")) {
                QString messageContent = line.mid(6); // Remove "User: " prefix
                QJsonObject messageJsonObject{{"role", "user"},
                                              {"content", messageContent}};
                conversationJsonArray.append(messageJsonObject);
            } else if (line.startsWith("AI: ")) {
                QString messageContent = line.mid(4); // Remove "AI: " prefix
                QJsonObject messageJsonObject{{"role", "assistant"},
                                              {"content", messageContent}};
                conversationJsonArray.append(messageJsonObject);
            }
        }

        // Create JSON request
        QJsonObject request;
        request.insert("model", "gpt-3.5-turbo");
        request.insert("messages", conversationJsonArray);

        // Send request to OpenAI API
        QUrl url("https://api.openai.com/v1/chat/completions");
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        req.setRawHeader(
            "Authorization",
            "Bearer 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");
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);

        QNetworkReply *reply =
            manager->post(req, QJsonDocument(request).toJson());
        ui->sendChatBtn->setEnabled(false);
        // Connect signals and slots
        QMovie *movie = new QMovie(":/assets/images/chatload.gif");
        ui->label_78->setMovie(movie);
        movie->start();
        ui->label_78->setVisible(true);
        connect(reply, &QNetworkReply::finished, this, [reply, movie, this]() {
            ui->label_78->setVisible(false);
            movie->deleteLater();
            if (reply->error() == QNetworkReply::NoError) {
                // Parse response JSON
                QJsonObject response =
                    QJsonDocument::fromJson(reply->readAll()).object();
                QString text = response.value("choices")
                                   .toArray()
                                   .at(0)
                                   .toObject()
                                   .value("message")
                                   .toObject()
                                   .value("content")
                                   .toString();
                // Append response to conversation
                QString aiResponse = "<div style='background-color: #fff; "
                                     "border-radius: 10px; "
                                     "border: 1px solid #ccc; "
                                     "padding: 10px;'><strong>AI:</strong> " +
                                     text.trimmed().replace("\n\n", "<br>") +
                                     "</div>";
                ui->textEdit->append("AI: " +
                                     text.trimmed().replace("\n\n", "\n"));
                ui->sendChatBtn->setEnabled(true);
            } else {
                qDebug() << reply->errorString();
            }
            reply->deleteLater();
        });
    }

    // Clear user input field
    ui->lineEdit_7->clear();
}

void dashboard_hospital::on_sendChatBtn_released() { sendOpenAIRequest(); }

void dashboard_hospital::on_New_Btn_released() {

    QString hospital_Name = ui->New_Hospital_name->text();
    QString hospital_Address = ui->New_Hospital_adresse->text();
    QString hospital_Numero = ui->New_Hospital_number->text();
    QString hospital_Email = ui->New_Hopistal_Email->text();
    QString hospital_Service = ui->New_Hospital_service->text();
    QString hospital_Equipement = ui->New_Hospital_Equipements->text();
    QString hospital_Remarque = ui->New_Remarks->toPlainText();
    int hospital_Capacite = ui->New_hospital_capacity->text().toInt();

    QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    mailREX.setCaseSensitivity(Qt::CaseInsensitive);
    mailREX.setPatternSyntax(QRegExp::RegExp);

    QRegExp phoneREX("^[0-9]{8}$");
    phoneREX.setPatternSyntax(QRegExp::RegExp);

    if (hospital_Name == "" || hospital_Address == "" ||
        hospital_Numero == "" || hospital_Email == "" ||
        hospital_Service == "" || hospital_Equipement == "") {

        QMessageBox::warning(this, "Error", "Fields are empty.");
        return;
    }
    if (hospital_Capacite <= 0) {
        QMessageBox::warning(this, "Error",
                             "Hospital capacite should be a possitive number.");
        return;
    }
    if (!mailREX.exactMatch(hospital_Email)) {
        QMessageBox::warning(this, "Error", "Invalid mail");
        return;
    }

    if (!phoneREX.exactMatch(hospital_Numero)) {
        QMessageBox::warning(this, "Error", "Invalid Phone number");
        return;
    }

    if (!Htmp.verifiylatLong(hospital_Address)) {
        QMessageBox::warning(
            this, "Error",
            "Address has a false format the correct format should be:\n"
            "36.86,10.18");
        return;
    }

    Hospital H(hospital_Name, hospital_Address, hospital_Numero, hospital_Email,
               hospital_Service, hospital_Equipement, hospital_Remarque,
               hospital_Capacite);

    if (H.Create()) {
        QMessageBox::information(this, "Success", "Hospital added successfull");
        ui->New_Hospital_name->setText("");
        ui->New_Hospital_adresse->setText("");
        ui->New_Hospital_number->setText("");
        ui->New_Hopistal_Email->setText("");
        ui->New_Hospital_service->setText("");
        ui->New_Hospital_Equipements->setText("");
        ui->New_Remarks->setText("");
        ui->New_hospital_capacity->setValue(0);
        qDebug() << "Hospitall Added";

        // say the hospital has been added :
        QTextToSpeech *tts1 = new QTextToSpeech(this);
        tts1->setLocale(QLocale(QLocale::English));
        tts1->setVoice(tts1->availableVoices().at(1));
        tts1->setPitch(20);
        QString message;
        message = "The Hospital " + hospital_Name + " has been added!";
        if (ui->ttsenable->isChecked())
        tts1->say(message);
        showArdStats("+Hos: " + hospital_Name);
        QTimer::singleShot(3000, [this]() {

                    refresh();
                });

    } else
        qDebug() << "Error adding hospital";
}

void dashboard_hospital::on_pushButton_3_released() {

    if (selected_hospital == -1) {
        QMessageBox::warning(this, "Error", "Nothing is selected!");
        return;
    }

    int id =
        ui->tableView_4->model()->index(selected_hospital, 0).data().toInt();

    if (Htmp.Delete(id)) {
        QMessageBox::information(this, "Success",
                                 "Hospital deleted successfull");
        selected_hospital = -1;
        refresh();
    } else {
        QMessageBox::warning(this, "Error", "Error while deleting hospital");
    }
}

void dashboard_hospital::on_pushButton_48_released() {
    ui->stackedWidget_6->setCurrentIndex(0);
}

void dashboard_hospital::on_pushButton_33_released() {
    ui->stackedWidget_6->setCurrentIndex(1);
}

void dashboard_hospital::on_pushButton_4_released() {

    if (selected_hospital == -1) {
        QMessageBox::warning(this, "Error", "Nothing is selected!");
        return;
    }
    ui->Modifier_Hospital_name->setText(ui->tableView_4->model()
                                            ->index(selected_hospital, 1)
                                            .data()
                                            .toString());
    ui->Modifier_Hospital_adresse->setText(ui->tableView_4->model()
                                               ->index(selected_hospital, 2)
                                               .data()
                                               .toString());
    ui->Modifier_Hospital_number->setText(ui->tableView_4->model()
                                              ->index(selected_hospital, 3)
                                              .data()
                                              .toString());
    ui->Modifier_Hopistal_Email->setText(ui->tableView_4->model()
                                             ->index(selected_hospital, 4)
                                             .data()
                                             .toString());
    ui->Modifier_hospital_capacity->setValue(
        ui->tableView_4->model()->index(selected_hospital, 5).data().toInt());
    ui->Modifier_Hospital_service->setText(ui->tableView_4->model()
                                               ->index(selected_hospital, 6)
                                               .data()
                                               .toString());
    ui->Modifier_Hospital_Equipements->setText(ui->tableView_4->model()
                                                   ->index(selected_hospital, 7)
                                                   .data()
                                                   .toString());
    ui->Modifier_Remarks->setText(ui->tableView_4->model()
                                      ->index(selected_hospital, 8)
                                      .data()
                                      .toString());
    ui->stackedWidget_6->setCurrentIndex(3);
}

void dashboard_hospital::on_Modifier_Cancel_released() {
    ui->stackedWidget_6->setCurrentIndex(0);
    selected_hospital = -1;
}

void dashboard_hospital::on_Modifier_btn_released() {

    QString hospital_Name = ui->Modifier_Hospital_name->text();
    QString hospital_Address = ui->Modifier_Hospital_adresse->text();
    QString hospital_Numero = ui->Modifier_Hospital_number->text();
    QString hospital_Email = ui->Modifier_Hopistal_Email->text();
    QString hospital_Service = ui->Modifier_Hospital_service->text();
    QString hospital_Equipement = ui->Modifier_Hospital_Equipements->text();
    QString hospital_Remarque = ui->Modifier_Remarks->toPlainText();
    int hospital_Capacite = ui->Modifier_hospital_capacity->text().toInt();

    QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    mailREX.setCaseSensitivity(Qt::CaseInsensitive);
    mailREX.setPatternSyntax(QRegExp::RegExp);

    QRegExp phoneREX("^[0-9]{8}$");
    phoneREX.setPatternSyntax(QRegExp::RegExp);

    if (hospital_Name == "" || hospital_Address == "" ||
        hospital_Numero == "" || hospital_Email == "" ||
        hospital_Service == "" || hospital_Equipement == "" ||
        hospital_Remarque == "") {

        QMessageBox::warning(this, "Error", "Fields are empty.");
        return;
    }
    if (hospital_Capacite <= 0) {
        QMessageBox::warning(this, "Error",
                             "Hospital capacite should be a possitive number.");
        return;
    }
    if (!mailREX.exactMatch(hospital_Email)) {
        QMessageBox::warning(this, "Error", "Invalid mail");
        return;
    }

    if (!phoneREX.exactMatch(hospital_Numero)) {
        QMessageBox::warning(this, "Error", "Invalid Phone number");
        return;
    }

    Hospital H(hospital_Name, hospital_Address, hospital_Numero, hospital_Email,
               hospital_Service, hospital_Equipement, hospital_Remarque,
               hospital_Capacite);

    if (H.Update(ui->tableView_4->model()
                     ->index(selected_hospital, 0)
                     .data()
                     .toInt())) {
        QMessageBox::information(this, "Success",
                                 "Hospital modified successfull");

        ui->stackedWidget_6->setCurrentIndex(0);
        selected_hospital = -1;

        qDebug() << "Hospitall Modified";

        refresh();
    } else
        qDebug() << "Error adding hospital";
}

void dashboard_hospital::on_pushButton_6_released() {
    refresh();

    QString searchText = ui->lineEdit->text();
    int searchColumn = ui->comboBox->currentIndex();

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);

    proxyModel->setSourceModel(ui->tableView_4->model());

    if (searchColumn == 0) {
        proxyModel->setFilterRegExp(
            QRegExp(searchText, Qt::CaseInsensitive, QRegExp::FixedString));
        proxyModel->setFilterKeyColumn(-1);
    } else {
        proxyModel->setFilterRegExp(
            QRegExp(searchText, Qt::CaseInsensitive, QRegExp::FixedString));
        proxyModel->setFilterKeyColumn(searchColumn - 1);
    }

    ui->tableView_4->setModel(proxyModel);
}

void dashboard_hospital::on_pushButton_5_released() {
    if (selected_hospital == -1) {
        QMessageBox::warning(this, "Error", "Nothing is selected!");
        return;
    }

    // Ask the user where they want to save the PDF file
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF File"), "", tr("PDF Files (*.pdf)"));

    if (fileName.isEmpty()) {
        return;
    }

    QString currentPath = QCoreApplication::applicationDirPath();
    // Read the HTML template file
    QFile file(currentPath + "../../../AMS/libraries/docs/hospitalinfo.html"); // Assuming the HTML template is stored as a resource file
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QMessageBox::critical(this, tr("Error"), tr("Could not open HTML template file."));
        return;
    }
    QTextStream in(&file);
     in.setCodec("UTF-8");
     QString htmlTemplate = in.readAll();
     file.close();

    // Replace placeholders in the HTML template with actual values
     QString hospital_name = ui->tableView_4->model()->index(selected_hospital,1).data().toString();
     QString hospital_adresse = ui->tableView_4->model()->index(selected_hospital,2).data().toString();
     QString hospital_service = ui->tableView_4->model()->index(selected_hospital,6).data().toString();
    QString currentDate = QDate::currentDate().toString(Qt::ISODate); // Using ISODate format for consistency
    QString html = htmlTemplate.arg(currentDate).arg(hospital_name).arg(hospital_service).arg(hospital_adresse).arg(currentDate);


    QString tmpHtmlFilePath = currentPath + "../../../AMS/libraries/docs/temp.html";
        QFile tmpHtmlFile(tmpHtmlFilePath);
        if (!tmpHtmlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Error: Failed to create temporary HTML file";
            return;
        }
        QTextStream out(&tmpHtmlFile);
        out.setCodec("UTF-8");
        out << html.toUtf8();
        tmpHtmlFile.close();

    // Run wkhtmltopdf to convert the HTML file to a PDF file
    QString wkhtmltopdfPath = currentPath + "../../../AMS/libraries/wkhtmltopdf/bin/wkhtmltopdf.exe";  // Set the path to wkhtmltopdf executable
    QStringList args;
    args << "-q" << "-n" << "--enable-local-file-access";  // Optional flags to suppress output and disable JavaScript
    args << tmpHtmlFilePath << fileName;
    QProcess process;
    process.start(wkhtmltopdfPath, args);
    if (!process.waitForStarted() || !process.waitForFinished()) {
        qDebug() << "Error: Failed to start or wait for wkhtmltopdf process";
    }

    // Remove the temporary HTML file
    QFile::remove(tmpHtmlFilePath);
}

void dashboard_hospital::toggleDropdownWidget() {
    QPropertyAnimation *animation = nullptr;
    if (!animation) {
        animation =
            new QPropertyAnimation(ui->dropdownWidget, "geometry", this);
        animation->setDuration(
            400); // Set the duration of the animation (in milliseconds)
        animation->setEasingCurve(
            QEasingCurve::InOutCubic); // Set the easing curve for the animation
    }
    if (ui->dropdownWidget->height() == 0) {
        // Show the widget (expand it)
        animation->setStartValue(QRect(780, 80, 231, 0));
        animation->setEndValue(
            QRect(780, 80, 231,
                  ui->dropdownWidget
                      ->maximumHeight())); // Set the desired height of the
                                           // dropdownWidget when expanded
        animation->start();
    } else if (ui->dropdownWidget->height() ==
               ui->dropdownWidget->maximumHeight()) {
        // Hide the widget (collapse it)
        animation->setStartValue(
            QRect(780, 80, 231, ui->dropdownWidget->maximumHeight()));
        animation->setEndValue(QRect(780, 80, 231, 0));
        animation->start();
    }
}

void dashboard_hospital::toggleNotificationWidget() {
    QPropertyAnimation *animation = nullptr;
    if (!animation) {
        animation =
            new QPropertyAnimation(ui->notificationWidget, "geometry", this);
        animation->setDuration(
            400); // Set the duration of the animation (in milliseconds)
        animation->setEasingCurve(
            QEasingCurve::InOutCubic); // Set the easing curve for the animation
    }
    if (ui->notificationWidget->height() == 0) {
        // Show the widget (expand it)
        animation->setStartValue(QRect(70, 70, 400, 0));
        animation->setEndValue(
            QRect(70, 70, 400,
                  ui->notificationWidget
                      ->maximumHeight())); // Set the desired height of the
                                           // dropdownWidget when expanded
        animation->start();
    } else if (ui->notificationWidget->height() ==
               ui->notificationWidget->maximumHeight()) {
        // Hide the widget (collapse it)
        animation->setStartValue(
            QRect(70, 70, 400, ui->notificationWidget->maximumHeight()));
        animation->setEndValue(QRect(70, 70, 400, 0));
        animation->start();
    }
}
void dashboard_hospital::on_pushButton_15_released() { toggleDropdownWidget(); }

void dashboard_hospital::on_pushButton_9_released() {
    if (ui->stackedWidget->currentIndex()!=4)
        ui->stackedWidget->setCurrentIndex(4);
}

void dashboard_hospital::on_LiveMapBtn_released() {

    if (ui->stackedWidget->currentIndex()!=3)
        ui->stackedWidget->setCurrentIndex(3);
}

void dashboard_hospital::on_pushButton_12_released() {
    toggleNotificationWidget();
}

void dashboard_hospital::on_pushButton_11_released()
{
    selectMap dialog(this);
    connect(&dialog, &selectMap::locationEntered, this, &dashboard_hospital::onDialogClosed);
    dialog.exec();
}
void dashboard_hospital::onDialogClosed(const QString& text)
{
    ui->New_Hospital_adresse->setText(text);
}

void dashboard_hospital::on_AboutBtn_released()
{
    if (ui->stackedWidget->currentIndex()!=4)
        ui->stackedWidget->setCurrentIndex(4);
}

void dashboard_hospital::on_settingsBtn_released()
{
    if (ui->stackedWidget->currentIndex()!=5)
        ui->stackedWidget->setCurrentIndex(5);

}

void dashboard_hospital::on_pushButton_11_clicked()
{

}

void dashboard_hospital::on_pushButton_16_toggled(bool checked)
{
    if (checked) {
        // Start recording audio
        QString exeDir = QCoreApplication::applicationDirPath();
        QString audioFilePath = exeDir + "/temp.wav";
        audioRecorder = new QAudioRecorder(this);
        audioRecorder->setAudioInput(
            "default"); // Change this to the appropriate audio input
        audioRecorder->setOutputLocation(QUrl::fromLocalFile(audioFilePath));
        audioRecorder->record();
        qDebug("Recording...");

    } else {
        ui->pushButton_16->setDisabled(true);
        // Stop recording audio
        audioRecorder->stop();
        qDebug("Stopped Recording, Output:");
        qDebug() << audioRecorder->outputLocation();

        // Read the audio file into a QByteArray
        QFile audioFile(QCoreApplication::applicationDirPath() + "/" +
                        "temp.wav");
        audioFile.open(QIODevice::ReadOnly);
        QByteArray audioData = audioFile.readAll();
        audioFile.close();

        // Send a POST request to the Speech-to-Text API
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkRequest request(QUrl(
            "https://westeurope.stt.speech.microsoft.com/speech/recognition/"
            "conversation/cognitiveservices/v1?language=en-US"));
        request.setHeader(QNetworkRequest::ContentTypeHeader,
                          "audio/wav; codec=\"audio/pcm\"; samplerate=16000");
        request.setRawHeader("Ocp-Apim-Subscription-Key",
                             "f597c78d6d604be4a178d3f404998d99");
        QNetworkReply *reply = manager->post(request, audioData);

        // Process the reply when it is finished
        connect(reply, &QNetworkReply::finished, [=]() {
            // Re-enable the button
            ui->pushButton_16->setEnabled(true);

            // Read the response from the Speech-to-Text API
            QByteArray responseData = reply->readAll();

            // Parse the JSON response
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObj = jsonDoc.object();
            QString recognizedText = jsonObj.value("DisplayText").toString();

            // Print the recognized text
            qDebug() << "Recognized: " << recognizedText;

            // Delete the temporary file
            QFile::remove("temp.wav");

            // Cleanup the reply object
            reply->deleteLater();

            ui->lineEdit_7->setText(recognizedText);
            sendOpenAIRequest();
        });
    }
}

void dashboard_hospital::on_stackedWidget_currentChanged(int arg1)
{
    if (ui->pushButton_16->isChecked() && arg1 != 2)
        on_pushButton_16_toggled(false);
}

void dashboard_hospital::on_pushButton_12_clicked()
{

}

void dashboard_hospital::on_pushButton_2_released()
{
    QModelIndexList selectedIndexes =
            ui->tableView_5->selectionModel()->selectedIndexes();

        if (!selectedIndexes.isEmpty()) {
            int selectedRow = selectedIndexes.first().row();
            int id = ui->tableView_5->model()
                                       ->data(ui->tableView_5->model()->index(selectedRow, 0))
                                       .toInt();
            int status = ui->tableView_5->model()
                                       ->data(ui->tableView_5->model()->index(selectedRow, 2))
                                       .toInt();
            if (status == 2)
                assignAmbulanceToEmergency(id);
            else if (status == 4)
                assignAmbulanceToHospital(id);
        }
}

void dashboard_hospital::on_tableView_6_doubleClicked(const QModelIndex &index)
{
    QModelIndex thirdColumnIndex = index.sibling(index.row(), 2);
    if (thirdColumnIndex.data().toInt() == 3){
        QModelIndex firstColumnIndex = index.sibling(index.row(), 0);
        QVariant data = firstColumnIndex.data();
        int item = data.toInt();
        QSqlQuery query0;
        query0.prepare("SELECT Adresse, Matricule_Ambulance FROM EmergencyCall WHERE id = :id");
        query0.bindValue(":id", item);
        query0.exec();
        query0.next();
        QStringList adr1 = query0.value(0).toString().split(",");
        QString mat = query0.value(1).toString();
        QSqlQuery query1;
        query1.prepare("SELECT Emplacement FROM Ambulance WHERE Matricule = :mat");
        query1.bindValue(":mat", mat);
        query1.exec();
        query1.next();
        QStringList adr2 = query1.value(0).toString().split(",");
        emit setRoute(adr1[0], adr1[1], adr2[0], adr2[1]);
        emit setCenter(adr1[0], adr1[1]);
        QGeoCoordinate coord1(adr1[0].toDouble(), adr1[1].toDouble());
        QGeoCoordinate coord2(adr2[0].toDouble(), adr2[1].toDouble());

        ui->distanceToChange->setText(QString::number(coord1.distanceTo(coord2))+ "m");
        //time (in minutes) = (140,000 meters / (70,000 meters/hour)) * 60 minutes/hour = 120 minutes


            int timeInMinutes = qRound((coord1.distanceTo(coord2) / 70000) * 60);
            int hours = timeInMinutes / 60;
            int minutes = timeInMinutes % 60;
            int seconds = qRound((float)(timeInMinutes - qFloor(timeInMinutes)) * 60);
            ui->timeToChange->setText(QString("%1:%2:%3")
                .arg(hours, 2, 10, QChar('0'))
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0')));


    }else if (thirdColumnIndex.data().toInt() == 5){
        QModelIndex firstColumnIndex = index.sibling(index.row(), 0);
        QVariant data = firstColumnIndex.data();
        int item = data.toInt();
        QSqlQuery query0;

        query0.prepare("SELECT Adresse, Hospital_id FROM EmergencyCall WHERE id = :id");
        query0.bindValue(":id", item);
        query0.exec();
        query0.next();
        QStringList adr1 = query0.value(0).toString().split(",");
        QString mat = query0.value(1).toString();
        QSqlQuery query1;
        query1.prepare("SELECT Adresse FROM Hospital WHERE Id = :mat");
        query1.bindValue(":mat", mat);
        query1.exec();
        query1.next();
        QStringList adr2 = query1.value(0).toString().split(",");
        emit setRoute(adr1[0], adr1[1], adr2[0], adr2[1]);
        emit setCenter(adr1[0], adr1[1]);
        QGeoCoordinate coord1(adr1[0].toDouble(), adr1[1].toDouble());
        QGeoCoordinate coord2(adr2[0].toDouble(), adr2[1].toDouble());
        ui->distanceToChange->setText(QString::number(coord1.distanceTo(coord2))+ "m");
        //time (in minutes) = (140,000 meters / (70,000 meters/hour)) * 60 minutes/hour = 120 minutes


            int timeInMinutes = qRound((coord1.distanceTo(coord2) / 70000) * 60);
            int hours = timeInMinutes / 60;
            int minutes = timeInMinutes % 60;
            int seconds = qRound((float)(timeInMinutes - qFloor(timeInMinutes)) * 60);
            ui->timeToChange->setText(QString("%1:%2:%3")
                .arg(hours, 2, 10, QChar('0'))
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0')));
    }
}

void dashboard_hospital::on_pushButton_20_released()
{
    A.close_arduino();
    auto *dialog = new ModifyEmployee(&e, this);
    dialog->exec();
    ui->lineEdit_6->setText(e.getCIN());
    ui->lineEdit_3->setText(e.getPrenom());
    ui->lineEdit_4->setText(e.getNom());
    ui->lineEdit_8->setText(e.getMail());
    refresh();
}
