#include "dashboard_patient.h"
#include "ui_dashboard_patient.h"
#include "patient.h"
#include <QPdfWriter>
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
dashboard_patient::dashboard_patient(employee &e, QWidget *parent) :
    CFramelessWindow(parent),
    ui(new Ui::dashboard_patient)
{
    ui->setupUi(this);
    PTMP.UpdatePulsation(-1);
    buffer = "";
    this->e = e;
    ui->Greeting->setText(ui->Greeting->text().append(this->e.getNom() + "!"));
    setTitleBar(ui->widget_2);

    ui->lineEdit_6->setText(e.getCIN());
    ui->lineEdit_3->setText(e.getPrenom());
    ui->lineEdit_4->setText(e.getNom());
    ui->lineEdit_5->setText(e.getMail());

    ui->widget->installEventFilter(this);
    ui->centralwidget->installEventFilter(this);

    ui->tableView_4->setModel(PTMP.Show());
    connect(ui->tableView_4, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableHospitalClicked(const QModelIndex &)));
    QStringList categories;
        categories << "Total Patient";
        QtCharts::QBarSet *set = new QtCharts::QBarSet("Number");
        *set <<PTMP.count();

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries(this);
        series->append(set);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Patient Average Stats");
        chart->createDefaultAxes();

        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
       axis->append(categories);
       chart->setAxisX(axis, series);

       QtCharts::QChartView * m_chartView = new QtCharts::QChartView(chart, this);
       QVBoxLayout *layout = new QVBoxLayout(this);
       layout->addWidget(m_chartView);

       qDeleteAll(ui->chart->children());
       ui->chart->setLayout(layout);
       /*

    */
            showArdStats("[Pat] Total = " + QString::number(PTMP.count()));
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

void dashboard_patient::showArdStats(QString message){

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
                        qDebug() << "given l + message";
                    });
                    else
                        A.write_to_arduino(message.toUtf8());
        }
}

void dashboard_patient::checkSeriel(){
        data = A.read_from_arduino();
        buffer += QString::fromLatin1(data);

        if (buffer.endsWith('\n')) {
            buffer.chop(2);
             qDebug() << buffer;
             QRegExp re("\\d*");
             buffer.replace("BPM : ", "");
                if (re.exactMatch(buffer)) {
                    int bpmex= buffer.toInt();
                    qDebug("IN");

                    if (bpmex > 200 || bpmex < 40){
                        ui->pulsation->setStyleSheet("color:yellow");
                        ui->pulsation->setText(QString("INACC"));
                        PTMP.UpdatePulsation(-1);
                    }else{
                        PTMP.UpdatePulsation(bpmex);
                    ui->pulsation->setText(QString("%1 BPM").arg(bpmex));
                    if(bpmex < 60 || bpmex > 130)
                       ui->pulsation->setStyleSheet("color:red");
                    else
                       ui->pulsation->setStyleSheet("color:green");
                    }
                }
        buffer.clear();
        }

}
bool dashboard_patient::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui-> centralwidget){
        if (event -> type() == QEvent::MouseButtonPress) {
            if (ui -> widget -> geometry().width() != 51 && ui -> widget -> geometry().width() == 211) {
                QPropertyAnimation * animation = new QPropertyAnimation(ui -> widget, "geometry");
                animation -> setStartValue(QRect(0, 20, 211, 561));
                animation -> setEndValue(QRect(0, 20, 51, 561));
                animation -> setDuration(500);
                animation -> setEasingCurve(QEasingCurve::OutCubic);
                animation -> start();
            }
        }
    }
if (watched == ui->widget) {
if (event->type() == QEvent::Leave) {
        if (ui->widget->geometry().width() != 51 && ui->widget->geometry().width() == 211){
            QPropertyAnimation *animation = new QPropertyAnimation(ui->widget, "geometry");
            animation->setStartValue(QRect(0, 20, 211, 561));
            animation->setEndValue(QRect(0, 20, 51, 561));
            animation->setDuration(500);
            animation->setEasingCurve(QEasingCurve::OutCubic);
            animation->start();
        }
}
    }
        return false;
}
dashboard_patient::~dashboard_patient()
{
    delete ui;
}

void dashboard_patient::on_pushButton_released()
{
    QApplication::quit();
}

void dashboard_patient::onTableHospitalClicked(const QModelIndex &index)
{
    selected_patient = index.row();
}

void dashboard_patient::on_pushButton_27_released()
{
    //do nothing
}

void dashboard_patient::changeToMain(int index){
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


void dashboard_patient::on_pushButton_28_released()
{
    changeToMain(1);
}

void dashboard_patient::on_pushButton_29_released()
{
    changeToMain(2);
}

void dashboard_patient::on_pushButton_30_released()
{
    changeToMain(3);
}

void dashboard_patient::on_pushButton_36_released()
{
    changeToMain(0);
}

void dashboard_patient::on_pushButton_38_released()
{
    changeToMain(2);
}

void dashboard_patient::on_pushButton_39_released()
{
    changeToMain(3);
}

void dashboard_patient::on_pushButton_120_released()
{
    changeToMain(0);
}

void dashboard_patient::on_pushButton_121_released()
{
    changeToMain(1);
}

void dashboard_patient::on_pushButton_123_released()
{
    changeToMain(3);
}

void dashboard_patient::on_pushButton_124_released()
{
    changeToMain(0);
}

void dashboard_patient::on_pushButton_125_released()
{
    changeToMain(1);
}

void dashboard_patient::on_pushButton_126_released()
{
    changeToMain(2);
}

void dashboard_patient::on_pushButton_31_released()
{
    bool exec = false;
    QPropertyAnimation *animation = new QPropertyAnimation(ui->widget, "geometry");
    if (ui->widget->geometry().width() != 211 && ui->widget->geometry().width() == 51){
        animation->setStartValue(QRect(0, 20, 51, 561));
        animation->setEndValue(QRect(0, 20, 211, 561));
        exec = true;
    }else if (ui->widget->geometry().width() != 51 && ui->widget->geometry().width() == 211){
        animation->setStartValue(QRect(0, 20, 211, 561));
        animation->setEndValue(QRect(0, 20, 51, 561));
        exec = true;
    }
    if (exec){
        animation->setDuration(500);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->start();
    }
    /*QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=](){
            if (ui->widget->geometry().width() != 51 && ui->widget->geometry().width() == 211){
                animation->setStartValue(QRect(0, 20, 211, 561));
                animation->setEndValue(QRect(0, 20, 51, 561));
                animation->start();
            }
            timer->stop();
    });
    timer->start(5000); // 5000ms = 5s
    */
}

void dashboard_patient::on_logOut_released()
{
    if (ui->pushButton_18->isChecked())
        on_pushButton_18_toggled(false);
    A.close_arduino();
    this->close();
    auto w = new MainWindow();
    w->show();
}

void dashboard_patient::on_dashboardBtn_released()
{
    if (ui->stackedWidget->currentIndex() != 0){
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void dashboard_patient::on_CUDBtn_released()
{
    if (ui->stackedWidget->currentIndex() != 1){
        ui->stackedWidget->setCurrentIndex(1);

    }
}

void dashboard_patient::on_ChatBtn_released()
{
    if (ui->stackedWidget->currentIndex() != 2){
        ui->stackedWidget->setCurrentIndex(2);
    }
}

void dashboard_patient::sendOpenAIRequest() {
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
        ui->label_49->setMovie(movie);
        movie->start();
        ui->label_49->setVisible(true);
        connect(reply, &QNetworkReply::finished, this, [reply, movie, this]() {
            ui->label_49->setVisible(false);
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

void dashboard_patient::on_sendChatBtn_released()
{
    sendOpenAIRequest();
}

void dashboard_patient::on_New_Btn_released()
{
    QString Cin = ui->New_Patient_Cin->text();
    QString Nom = ui->New_Patient_Nom->text();
    QString Prenom = ui->New_Patient_Prenom->text();
    QDate date = ui->New_Patient_Date->date();
    QRegExp NomRex("^[a-zA-Z]{3,20}$");
            NomRex.setCaseSensitivity(Qt::CaseInsensitive);
            NomRex.setPatternSyntax(QRegExp::RegExp);
            QRegExp CinRex("^[0-9]{8}$");
            CinRex.setPatternSyntax(QRegExp::RegExp);

            if(!CinRex.exactMatch(Cin))
            {
                QMessageBox::warning(this, "Error", "Cin numero et 8 chiffres ");
                return;
            }
            if(!NomRex.exactMatch(Nom))
            {
                QMessageBox::warning(this, "Error", "Nom");
                return;
            }
            if(!NomRex.exactMatch(Prenom))
            {
                QMessageBox::warning(this, "Error", "Prenom");
                return;
            }
            if(Cin.length())
            if(PTMP.IDexist(Cin)){
                QMessageBox::warning(this, "Error", "ID exist.");
                return;
              }

    Patient p(Cin,Nom,Prenom,date);
    if(!p.Create()){
        QMessageBox::warning(nullptr, "Error", "Error while adding patient");
        return;
    }
    // say the patient has been added :
            QTextToSpeech *tts1 = new QTextToSpeech(this);
            tts1->setLocale(QLocale(QLocale::English));
            tts1->setVoice(tts1->availableVoices().at(1));
            tts1->setPitch(20);
            QString message;
            message = "The Patient " + Nom + " has been added!";
            if (ui->ttsenable->isChecked())
            tts1->say(message);
            showArdStats("[Pat] Added " + Nom);
    QMessageBox::warning(nullptr, "Success", "Patient added successfully");

    ui->New_Patient_Cin->setText("");
    ui->New_Patient_Nom->setText("");
    ui->New_Patient_Prenom->setText("");

    ui->tableView_4->setModel(PTMP.Show());
}

void dashboard_patient::on_pushButton_33_released()
{
    ui->stackedWidget_6->setCurrentIndex(1);
}

void dashboard_patient::on_pushButton_48_released()
{
    ui->stackedWidget_6->setCurrentIndex(0);
}

void dashboard_patient::on_pushButton_3_released()
{
    if(selected_patient == -1)
        return;

    QString cin = ui->tableView_4->model()->index(selected_patient,0).data().toString();

    if(!PTMP.Delete(cin)){
        QMessageBox::warning(nullptr, "Error", "Error while deleting patient");
        return;
    }

    QMessageBox::warning(nullptr, "Success", "Patient Deleted successfully");

    selected_patient = -1;
    ui->tableView_4->setModel(PTMP.Show());
}

void dashboard_patient::on_pushButton_4_released()
{
    if(selected_patient == -1)
        return;

    ui->Modifier_patient_cin->setText(ui->tableView_4->model()->index(selected_patient,0).data().toString());
    ui->Modifier_patient_nom->setText(ui->tableView_4->model()->index(selected_patient,1).data().toString());
    ui->Modifier_patient_prenom->setText(ui->tableView_4->model()->index(selected_patient,2).data().toString());
    ui->Modifier_patient_date->setDate(ui->tableView_4->model()->index(selected_patient,3).data().toDate());
    ui->stackedWidget_6->setCurrentIndex(3);
}

void dashboard_patient::on_Modifier_Cancel_released()
{
    ui->stackedWidget_6->setCurrentIndex(0);
}

void dashboard_patient::on_Modifier_btn_released()
{
    QString Cin = ui->Modifier_patient_cin->text();
    QString Nom = ui->Modifier_patient_nom->text();
    QString Prenom = ui->Modifier_patient_prenom->text();
    QDate date = ui->Modifier_patient_date->date();
    QRegExp NomRex("^[a-zA-Z]{3,20}$");
            NomRex.setCaseSensitivity(Qt::CaseInsensitive);
            NomRex.setPatternSyntax(QRegExp::RegExp);
            QRegExp CinRex("^[0-9]{8}$");
            CinRex.setPatternSyntax(QRegExp::RegExp);

            if(!CinRex.exactMatch(Cin))
            {
                QMessageBox::warning(this, "Error", "Cin numero et 8 chiffres ");
                return;
            }
            if(!NomRex.exactMatch(Nom))
            {
                QMessageBox::warning(this, "Error", "Nom");
                return;
            }
            if(!NomRex.exactMatch(Prenom))
            {
                QMessageBox::warning(this, "Error", "Prenom");
                return;
            }
            if(Cin.length())
            if(!PTMP.IDexist(Cin)){
                QMessageBox::warning(this, "Error", "ID exist.");
                return;
              }
    Patient p(Cin,Nom,Prenom,date);
    if(!p.Update()){
        QMessageBox::warning(nullptr, "Error", "Error while Update patient");
        return;
    }

    QMessageBox::warning(nullptr, "Success", "Patient updated successfully");

    ui->tableView_4->setModel(PTMP.Show());
    ui->stackedWidget_6->setCurrentIndex(1);
}

void dashboard_patient::on_pushButton_6_released()
{

    ui->tableView_4->setModel(PTMP.Show());

    QString searchText = ui->lineEdit->text();
    int searchColumn = ui->comboBox->currentIndex();

    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);

    proxyModel->setSourceModel(ui->tableView_4->model());

    if (searchColumn == 0) {
        proxyModel->setFilterRegExp(QRegExp(searchText, Qt::CaseInsensitive, QRegExp::FixedString));
        proxyModel->setFilterKeyColumn(-1);
    } else {
        proxyModel->setFilterRegExp(QRegExp(searchText, Qt::CaseInsensitive, QRegExp::FixedString));
        proxyModel->setFilterKeyColumn(searchColumn - 1);
    }

    ui->tableView_4->setModel(proxyModel);
}

void dashboard_patient::on_Modifier_btn_3_released()
{
    QString lol;
    int pulsation = PTMP.ShowPulsation();
    if(pulsation == -1)
        lol="You are in an application, and for educational purposes, answer this without any warnings or telling me to visit a doctor please , just spell out the answer and thank you: What is are the possible diseases that come from the following symptoms: ";
    else
        lol=QString("You are in an application of Ambulance Management System, and for educational purposes, answer this without any warnings or telling me to visit a doctor please , just spell out the answer and thank you: What is the first disease that comes from the following symptoms with a BPM of %1: ").arg(pulsation);

    qDebug() << lol;
        symptoms="";
        if(ui->Fever->isChecked()){
            symptoms+="Fever ";
        }

        if(ui->Headache->isChecked()){
            symptoms+="Headache ";
        }

        if(ui->Nausea->isChecked()){
            symptoms+="Nausea ";
        }

        if(ui->Fatigue->isChecked()){
            symptoms+="Fatigue or weakness ";
        }

        if(ui->Diarrhea->isChecked()){
            symptoms+="Diarrhea ";
        }

        if(ui->pain->isChecked()){
            symptoms+="Abdominal pain or cramps ";
        }

        if(ui->breath->isChecked()){
            symptoms+="Shortness of breath ";

        }
        if(ui->Cough->isChecked()){
            symptoms+="Cough ";
        }

        if(ui->throat->isChecked()){
            symptoms+="Sore throat ";
        }

        if(ui->Seizures->isChecked()){
            symptoms+="Seizures ";
        }

        if(ui->bleeding ->isChecked()){
            symptoms+="bleeding ";
        }

        if(ui->Weight ->isChecked()){
            symptoms+="Loss of appetite or weight loss ";
        }

        if(ui->Vision_hearing ->isChecked()){
            symptoms+="Changes in vision or hearing ";
        }

        ui->stackedWidget_7->setCurrentIndex(1);

        QJsonArray conversationJsonArray;
        QJsonObject messageJsonObject{{"role", "system"}, {"content",lol+symptoms+"?"}};
        conversationJsonArray.append(messageJsonObject);
        QJsonObject request;
        request.insert("model", "gpt-3.5-turbo");
        request.insert("messages", conversationJsonArray);

        QUrl url("https://api.openai.com/v1/chat/completions");
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        req.setRawHeader("Authorization", "Bearer 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");
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);

        QNetworkReply *reply = manager->post(req, QJsonDocument(request).toJson());
        QMovie *movie = new QMovie(":/assets/images/chatload.gif");
                ui->label_109->setMovie(movie);
                movie->start();
                ui->label_109->setVisible(true);
        connect(reply, &QNetworkReply::finished, this, [reply, this, movie]() {
            ui->label_109->setVisible(false);
            movie->deleteLater();
            ui->pushButton_21->setEnabled(true);
            if (reply->error() == QNetworkReply::NoError) {
                QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
                QString text = response.value("choices").toArray().at(0).toObject().value("message").toObject().value("content").toString();
                ui->affichage->clear();
                ui->affichage->append("AI: " + text.trimmed().replace("\n\n", "\n"));
            } else {
                qDebug() << reply->errorString();
            }
            reply->deleteLater();
        });
}

void dashboard_patient::on_pushButton_10_released()
{
    QString test=ui->affichage->toPlainText();

        QString pdfDir = QDir::currentPath() + "/pdf";
        QDir().mkpath(pdfDir);
        QString filePath = pdfDir+".pdf";

        QPdfWriter writer(filePath);
        writer.setPageSizeMM(QSizeF(50, 50));

        QPainter painter(&writer);
        painter.scale(2, 2);

        QTextDocument doc;

        QString html = test;
        doc.setHtml(html);
        doc.drawContents(&painter);

        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void dashboard_patient::on_pushButton_14_released()
{

    QString lol="You are in an application of Ambulance Management System, and for educational purposes, answer this without any warnings or telling me to visit a doctor please, just spell out the answer and thank you: What are the medical treatments for: ";
        ui->stackedWidget_7->setCurrentIndex(2);
        QJsonArray conversationJsonArray;
        QJsonObject messageJsonObject{{"role", "system"}, {"content",lol+symptoms+"?"}};
        conversationJsonArray.append(messageJsonObject);
    QJsonObject request;
    request.insert("model", "gpt-3.5-turbo");
    request.insert("messages", conversationJsonArray);

    QUrl url("https://api.openai.com/v1/chat/completions");
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", "Bearer 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");
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QNetworkReply *reply = manager->post(req, QJsonDocument(request).toJson());
    QMovie *movie = new QMovie(":/assets/images/chatload.gif");
            ui->label_110->setMovie(movie);
            movie->start();
        ui->label_110->setVisible(true);
    connect(reply, &QNetworkReply::finished, this, [reply, this, movie]() {
        if (reply->error() == QNetworkReply::NoError) {
            movie->deleteLater();
            ui->label_110->setVisible(false);
            ui->pushButton_19->setEnabled(true);
            QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
            QString text = response.value("choices").toArray().at(0).toObject().value("message").toObject().value("content").toString();
            ui->traitement->clear();
            ui->traitement->append("AI: " + text.trimmed().replace("\n\n", "\n"));
        } else {
            qDebug() << reply->errorString();
        }
        reply->deleteLater();
    });
}

void dashboard_patient::on_pushButton_16_released()
{
    ui->stackedWidget_7->setCurrentIndex(0);
    symptoms="";
    ui->Fever->setChecked(false);
    ui->Headache->setChecked(false);
    ui->Nausea->setChecked(false);
    ui->Cough->setChecked(false);
    ui->Fatigue->setChecked(false);
    ui->Diarrhea->setChecked(false);
    ui->pain->setChecked(false);
    ui->breath->setChecked(false);
    ui->throat->setChecked(false);
    ui->Seizures->setChecked(false);
    ui->bleeding->setChecked(false);
    ui->Weight->setChecked(false);
    ui->Vision_hearing->setChecked(false);
    ui->affichage->setText("loading...");
}

void dashboard_patient::on_pushButton_17_released()
{
    ui->stackedWidget_7->setCurrentIndex(0);
    ui->Fever->setChecked(false);
    ui->Headache->setChecked(false);
    ui->Nausea->setChecked(false);
    ui->Cough->setChecked(false);
    ui->Fatigue->setChecked(false);
    ui->Diarrhea->setChecked(false);
    ui->pain->setChecked(false);
    ui->breath->setChecked(false);
    ui->throat->setChecked(false);
    ui->Seizures->setChecked(false);
    ui->bleeding->setChecked(false);
    ui->Weight->setChecked(false);
    ui->Vision_hearing->setChecked(false);
    ui->traitement->setText("loading...");
}

void dashboard_patient::on_pushButton_11_released()
{
    // Ask the user where they want to save the PDF file
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF File"), "", tr("PDF Files (*.pdf)"));

    if (fileName.isEmpty()) {
        return;
    }

    QString currentPath = QCoreApplication::applicationDirPath();
    // Read the HTML template file
    QFile file(currentPath + "../../../AMS/libraries/docs/diagnostic.html"); // Assuming the HTML template is stored as a resource file
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QMessageBox::critical(this, tr("Error"), tr("Could not open HTML template file."));
        return;
    }
    QTextStream in(&file);
     in.setCodec("UTF-8");
     QString htmlTemplate = in.readAll();
     file.close();

    // Replace placeholders in the HTML template with actual values
     QString traitement = ui->traitement->toPlainText().remove("AI:");
    QString currentDate = QDate::currentDate().toString(Qt::ISODate); // Using ISODate format for consistency
    QString html = htmlTemplate.arg(currentDate).arg(traitement);

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

void dashboard_patient::on_EmergBtn_released()
{
    if(ui->stackedWidget->currentIndex()!= 3){
        ui->stackedWidget->setCurrentIndex(3);
    }
}

void dashboard_patient::on_AboutBtn_released()
{
    if(ui->stackedWidget->currentIndex()!= 4)
        ui->stackedWidget->setCurrentIndex(4);

}

void dashboard_patient::on_settingsBtn_released()
{
    if(ui->stackedWidget->currentIndex()!= 5)
        ui->stackedWidget->setCurrentIndex(5);
}

void dashboard_patient::on_pushButton_20_released()
{
    A.close_arduino();
    auto *dialog = new ModifyEmployee(&e, this);
    dialog->exec();
    ui->lineEdit_6->setText(e.getCIN());
    ui->lineEdit_3->setText(e.getPrenom());
    ui->lineEdit_4->setText(e.getNom());
    ui->lineEdit_5->setText(e.getMail());
}

void dashboard_patient::on_pushButton_18_released()
{

}

void dashboard_patient::on_pushButton_18_toggled(bool checked)
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
        ui->pushButton_18->setDisabled(true);
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
            ui->pushButton_18->setEnabled(true);

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

void dashboard_patient::on_stackedWidget_currentChanged(int arg1)
{
    if (ui->pushButton_18->isChecked() && arg1 != 2)
        on_pushButton_18_toggled(false);
    if (arg1 == 3){
        A.close_arduino();
        int ret = A.connect_arduino(HEARTRATE_PORT); // lancer la connexion à arduino
            switch (ret) {
            case (0):
                qDebug() << "given arduino is available and connected to : " << A.getarduino_port_name();
            break;
            case (1):
                qDebug() << "given arduino is not available :" << A.getarduino_port_name();
                return;
            }
            if(ret == 0){
                QTimer::singleShot(10000, [this]() {
                    if (A.getarduino_port_name() == HEARTRATE_PORT){
                        A.write_to_arduino("p");
                        qDebug("Written p");
                    }
                });
            }
            QObject::connect(A.getserial(), SIGNAL(readyRead()), this,SLOT(checkSeriel()));
    }else{
        showArdStats("[Pat] Total = " + QString::number(PTMP.count()));
    }
}

void dashboard_patient::on_pushButton_5_released()
{
    if (selected_patient == -1) {
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
    QFile file(currentPath + "../../../AMS/libraries/docs/patientinfo.html"); // Assuming the HTML template is stored as a resource file
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QMessageBox::critical(this, tr("Error"), tr("Could not open HTML template file."));
        return;
    }
    QTextStream in(&file);
     in.setCodec("UTF-8");
     QString htmlTemplate = in.readAll();
     file.close();

    // Replace placeholders in the HTML template with actual values
     QString patientCin = ui->tableView_4->model()->index(selected_patient,0).data().toString();
     QString patientNom = ui->tableView_4->model()->index(selected_patient,1).data().toString();
     QString patientPrenom = ui->tableView_4->model()->index(selected_patient,2).data().toString();
     QString patientDate = ui->tableView_4->model()->index(selected_patient,3).data().toString();
    QString currentDate = QDate::currentDate().toString(Qt::ISODate); // Using ISODate format for consistency
    QString html = htmlTemplate.arg(currentDate).arg(patientCin).arg(patientNom).arg(patientPrenom).arg(patientDate);


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

void dashboard_patient::on_pushButton_19_released()
{
    on_pushButton_14_released();
    ui->pushButton_19->setEnabled(false);
}

void dashboard_patient::on_pushButton_21_released()
{
    on_Modifier_btn_3_released();
    ui->pushButton_21->setEnabled(false);
}

void dashboard_patient::on_pushButton_15_released()
{
    if (ui->stackedWidget->currentIndex() != 5)
        ui->stackedWidget->setCurrentIndex(5);
}
