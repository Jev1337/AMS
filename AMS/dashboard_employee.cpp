#include "dashboard_employee.h"
#include "ui_dashboard_employee.h"

dashboard::dashboard(employee &e, QWidget *parent)
    : CFramelessWindow(parent), ui(new Ui::dashboard) {
    ui->setupUi(this);
    this->e = e;
    timerCam = new QTimer(this);
    chartView = new QtCharts::QChartView(ui->widget_7);
    cap = nullptr;
    e.chartRefresh(chartView);


    if (e.getRemarks() != ""){
        ui->pushButton_34->setText(QString::number(e.getRemarks().count("\n")+1));
    }

    ui->greetingIns_11->setText(e.getNom() + " " + e.getPrenom());
    ui->lineEdit_6->setText(e.getCIN());
    ui->lineEdit_3->setText(e.getPrenom());
    ui->lineEdit_4->setText(e.getNom());
    ui->lineEdit_5->setText(e.getMail());
    ui->stat1->setText(e.getTotalEmp());
    ui->stat2->setText(e.getEmpMonth());
    ui->stat3->setText(e.getEmpAvgAge());
    ui->prcInc->setText(QString::number(e.getEmpIncreasePercent()) + "%");
    if (e.getEmpIncreasePercent() >= 0.0) {
        QPixmap inc(":/assets/icons/cil-arrow-top.png");
        ui->inc->setPixmap(inc);
    } else {
        QPixmap dec(":/assets/icons/cil-arrow-bottom.png");
        ui->inc->setPixmap(dec);
    }

    ui->Greeting->setText(ui->Greeting->text().append(this->e.getNom() + "!"));
    setTitleBar(ui->widget_2);
    e.getAccountTypes(ui->New_Type);

    ui->widget->installEventFilter(this);
    ui->centralwidget->installEventFilter(this);

    connect(timerCam, &QTimer::timeout, [=]() {
        if (updateCam()) {
            timerCam->stop();
            cap->release();
            ui->pushButton_4->setEnabled(true);

        }else
        if (ui->stackedWidget->currentIndex() != 3) {
            timerCam->stop();
            cap->release();
            ui->pushButton_4->setEnabled(true);
        }
    });

    ui->dropdownWidget->installEventFilter(this);
    ui->widget->installEventFilter(this);
    ui->centralwidget->installEventFilter(this);
    ui->dropdownWidget_2->installEventFilter(this);
    // voice of welcoming
    tts = new QTextToSpeech(this);
    tts->setLocale(QLocale(QLocale::English));
    tts->setVoice(tts->availableVoices().at(1));
    tts->setPitch(20);
    QString employee;
    employee =
        "Hello" + e.getNom() + ", Welcome to Ambulance Management System!";
    if (ui->ttsenable->isChecked())
        tts->say(employee);

    QStandardItemModel *model = e.getMessages();
    ui->tableView_4->setModel(model);
    ui->gridLayout_9->addWidget(ui->tableView_4, 0, 0);

    refreshList();


}

bool dashboard::eventFilter(QObject *watched, QEvent *event) {

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
    } else if (watched == ui->dropdownWidget_2){
        if (event->type() == QEvent::Leave) {
            toggleDropdownWidget2();
        }
    }
    return false;
}

dashboard::~dashboard() { delete ui; }

void dashboard::on_pushButton_released() { QApplication::quit(); }

void dashboard::on_pushButton_27_released() {
    // do nothing
}

void dashboard::showArdStats(QString message){

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

void dashboard::changeToMain(int index){
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

void dashboard::on_pushButton_28_released() {
    changeToMain(1);
}

void dashboard::on_pushButton_29_released() {
    changeToMain(2);
}

void dashboard::on_pushButton_30_released() {
    changeToMain(3);
}

void dashboard::on_pushButton_36_released() {
    changeToMain(0);
}

void dashboard::on_pushButton_38_released() {
    changeToMain(2);
}

void dashboard::on_pushButton_39_released() {
    changeToMain(3);
}

void dashboard::on_pushButton_120_released() {
    changeToMain(0);
}

void dashboard::on_pushButton_121_released() {
    changeToMain(1);
}

void dashboard::on_pushButton_123_released() {
    changeToMain(3);
}

void dashboard::on_pushButton_124_released() {
    changeToMain(0);
}

void dashboard::on_pushButton_125_released() {
    changeToMain(1);
}

void dashboard::on_pushButton_126_released() {
    changeToMain(2);
}

void dashboard::on_pushButton_31_released() {
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

void dashboard::on_logOut_released() {
    if (ui->pushButton_10->isChecked())
        on_pushButton_10_toggled(false);
    timerCam->stop();
    if (cap != nullptr)
        cap->release();
    ui->pushButton_4->setEnabled(true);
    A.close_arduino();
    this->close();
    auto w = new MainWindow();
    w->show();

}

void dashboard::on_dashboardBtn_released() {
    if (ui->stackedWidget->currentIndex() != 0) {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void dashboard::on_CUDBtn_released() {
    if (ui->stackedWidget->currentIndex() != 1) {
        ui->stackedWidget->setCurrentIndex(1);
        ui->New_Type->clear();
        ui->New_Type->setCurrentIndex(-1);
        e.getAccountTypes(ui->New_Type);
    }
}

void dashboard::on_ChatBtn_released() {
    if (ui->stackedWidget->currentIndex() != 2) {
        ui->stackedWidget->setCurrentIndex(2);
    }
}

void dashboard::sendOpenAIRequest() {
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
        ui->label_35->setMovie(movie);
        movie->start();
        ui->label_35->setVisible(true);
        connect(reply, &QNetworkReply::finished, this, [reply, movie, this]() {
            ui->label_35->setVisible(false);
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
                if (ui->ttsenable->isChecked())
                    tts->say(text.trimmed().replace("\n\n",""));
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

void dashboard::on_sendChatBtn_released() { sendOpenAIRequest(); }

void dashboard::on_pushButton_49_pressed() {}

void dashboard::on_pushButton_32_released() {}

void dashboard::refreshList() {
    // Create a QSqlTableModel to fetch data from the database
    QSqlRelationalTableModel *model = e.getAccountList();

    ui->tableView_3->setModel(model);
    ui->gridLayout_7->addWidget(ui->tableView_3, 0, 0);

    // Create a QTableView to display the data
    ui->tableView->setModel(model);

    // Add the QTableView to your UI
    ui->gridLayout->addWidget(ui->tableView, 0, 0);

    showArdStats("[Emp] Total = " + e.getTotalEmp() + " Avg Age = " + e.getEmpAvgAge());
}

void dashboard::changeToAlpha(int index){
    if (ui->stackedWidget_6->currentIndex() != index) {
        ui->stackedWidget_6->setCurrentIndex(2);
        QMovie *movie = new QMovie(":/assets/images/preloader.gif");
        ui->label_7->setMovie(movie);
        movie->start();
        QTimer *timer = new QTimer();
        QObject::connect(timer, &QTimer::timeout, [=]() {
            ui->stackedWidget_6->setCurrentIndex(index);
            timer->stop();
            timer->deleteLater();
            movie->stop();
            movie->deleteLater();
        });
        timer->start(300);
    }
}

void dashboard::on_pushButton_33_released() {
    changeToAlpha(1);
}

void dashboard::on_pushButton_48_released() {
    changeToAlpha(0);
}

void dashboard::on_pushButton_3_released() {
    QModelIndexList selectedIndexes =
        ui->tableView->selectionModel()->selectedIndexes();
    if (!selectedIndexes.isEmpty()) {

        QMessageBox::StandardButton reply =
            QMessageBox::question(nullptr, "Confirmation",
                                  "Are you sure? Once removed, the user will "
                                  "not be able to sign in again.",
                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // Get the row number of the selected item
            int selectedRow = selectedIndexes.first().row();
            QString cin =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 0))
                    .toString();
            // Loop over all columns and retrieve the data
            QString type =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 1))
                    .toString();
            QString email =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 2))
                    .toString();
            QString name =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 3))
                    .toString();
            QString lastname =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 4))
                    .toString();
            QString dob =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 5))
                    .toString();
            QString phone =
                    ui->tableView->model()
                        ->data(ui->tableView->model()->index(selectedRow, 6))
                        .toString();
            QString remarks =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 7))
                    .toString();

            int typ;
            if (type == "administrator")
                typ = 1;
            if (type == "comms_emp")
                typ = 2;
            if (type == "hospital_emp")
                typ = 3;
            if (type == "patientmgr_emp")
                typ = 4;
            if (type == "ambulance_emp")
                typ = 5;
            employee edel(cin, typ, email, name, lastname, dob, remarks, phone, 1);
            edel.removeSelf();
            refreshList();
        }
    } else
        QMessageBox::warning(nullptr, "Error", "Please select a row first.");
}

void dashboard::on_New_Btn_released() {
    if (!CINValidator(ui->New_CIN->text())) {
        QMessageBox::warning(this, "Error", "CIN Inserted is Invalid!");
        return;
    }
    if (!CINValidator(ui->phone->text())) {
        QMessageBox::warning(this, "Error", "Phone Inserted is Invalid!");
        return;
    }
    if (!NameValidator(ui->New_Name->text())) {
        QMessageBox::warning(
            this, "Error",
            "Your name must be 3 to 20 letters long and only alphabetic.");
        return;
    }
    if (!NameValidator(ui->New_LastName->text())) {
        QMessageBox::warning(
            this, "Error",
            "Your last name must be 3 to 20 letters long and only alphabetic.");
        return;
    }
    if (ui->New_Remarks->toPlainText().length() > 255) {
        QMessageBox::warning(this, "Error", "Remarks Exceeded 255 Characters.");
        return;
    }
    employee tst;
    if (tst.cinExistance(ui->New_CIN->text())) {
        QMessageBox::warning(
            this, "Error",
            "An account with the same CIN is already registered!");
        return;
    }
    employee emp(ui->New_CIN->text(), ui->New_Type->currentIndex(),
                 ui->New_Email->text(), ui->New_Name->text(),
                 ui->New_LastName->text(),
                 ui->New_DOB->date().toString("yyyy-MM-dd"),
                 ui->New_Remarks->toPlainText(), ui->phone->text(), !ui->checkBox_4->isChecked());
    emp.addSelf(ui->New_Password->text());
    if (ui->checkBox_2->isChecked()) {
        auto *dialog = new faceid();

        if (dialog->exec() == QDialog::Accepted) {
            emp.setFaceID(dialog->getResult());
        }else
            QMessageBox::warning(this, "Error", "Error 0x08: Error getting FaceID!");
    }
    if (ui->checkBox_3->isChecked()){
        if (emp.GetLastFingerprintID() != 127) {
            A.close_arduino();
            auto *dialog = new fingerprint(e.GetLastFingerprintID() + 1);
            if (dialog->exec() == QDialog::Accepted) {
                emp.setFingerprint(e.GetLastFingerprintID() + 1);
            } else {
                QMessageBox::warning(this, "Error", "Error 0x07: Error sent within the fingerprint sensor!");
            }
            showArdStats("[Emp] Total = " + e.getTotalEmp() + " Avg Age = " + e.getEmpAvgAge());
        } else
            QMessageBox::warning(this, "Error", "Reached Fingerprint Limit");
    }
        // say the emp has been added :

        QString message;
        message = "The Employee " + ui->New_CIN->text() + " has been added!";
        if (ui->ttsenable->isChecked())
            tts->say(message);
        showArdStats("[Emp] Added CIN: " + ui->New_CIN->text());
}

void dashboard::on_lineEdit_textChanged(const QString &arg1) {
    Q_UNUSED(arg1);
    refreshList();
    QString searchText = ui->lineEdit->text();
    int searchColumn = ui->comboBox->currentIndex();
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(ui->tableView->model());
    if (searchColumn == 0) {
        proxyModel->setFilterRegExp(
            QRegExp(searchText, Qt::CaseInsensitive, QRegExp::FixedString));
        proxyModel->setFilterKeyColumn(-1);
    } else {
        proxyModel->setFilterRegExp(
            QRegExp(searchText, Qt::CaseInsensitive, QRegExp::FixedString));
        proxyModel->setFilterKeyColumn(searchColumn - 1);
    }
    ui->tableView->setModel(proxyModel);
}

void dashboard::on_stackedWidget_4_currentChanged(int arg1) {
    if (arg1 == 0) {
        chartView = new QtCharts::QChartView(ui->widget_7);
        e.chartRefresh(chartView);
        ui->stat1->setText(e.getTotalEmp());
        ui->stat2->setText(e.getEmpMonth());
        ui->stat3->setText(e.getEmpAvgAge());
        ui->prcInc->setText(QString::number(e.getEmpIncreasePercent()) + "%");
        if (e.getEmpIncreasePercent() >= 0.0) {
            QPixmap inc(":/assets/icons/cil-arrow-top.png");
            ui->inc->setPixmap(inc);
        } else {
            QPixmap dec(":/assets/icons/cil-arrow-bottom.png");
            ui->inc->setPixmap(dec);
        }
    }
    if (arg1 == 2){

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

        QElapsedTimer timer;
        timer.start();
        QSqlQuery query;
        query.prepare("SELECT * FROM EMPLOYEE");
        query.exec();
        ui->stat1_2->setText(QString::number(timer.elapsed()) + "."+ QString::number(timer.nsecsElapsed())[0] + "ms" );
        timer.restart();
        QNetworkAccessManager manager;
        QUrl url("https://api.openai.com");
        QNetworkReply *reply = manager.get(QNetworkRequest(url));
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        qint64 ms = timer.elapsed();
        ui->stat1_5->setText(QString::number(ms -118) + " ms");
        reply->deleteLater();

        timer.restart();
        QUrl url1("https://dynamodb.us-west-1.amazonaws.com");
        QNetworkReply *reply1 = manager.get(QNetworkRequest(url1));
        QEventLoop loop1;
        QObject::connect(reply1, &QNetworkReply::finished, &loop1, &QEventLoop::quit);
        loop1.exec();
        qint64 ms1 = timer.elapsed();
        ui->stat1_6->setText(QString::number(ms1- 118) + " ms");
        reply1->deleteLater();

        timer.restart();
        QUrl url2("https://westeurope.stt.speech.microsoft.com");
        QNetworkReply *reply2 = manager.get(QNetworkRequest(url2));
        QEventLoop loop2;
        QObject::connect(reply2, &QNetworkReply::finished, &loop2, &QEventLoop::quit);
        loop2.exec();
        qint64 ms2 = timer.elapsed();
        ui->stat1_3->setText(QString::number(ms2-118) + " ms");
        reply2->deleteLater();


    }
}

void dashboard::on_stackedWidget_currentChanged(int arg1) {

    if (ui->pushButton_10->isChecked() && arg1 != 2)
        on_pushButton_10_toggled(false);
    if (arg1 == 0) {
        on_stackedWidget_4_currentChanged(ui->stackedWidget_4->currentIndex());
    }
    if (arg1 == 1)
        on_stackedWidget_6_currentChanged(ui->stackedWidget_6->currentIndex());
}

void dashboard::on_stackedWidget_6_currentChanged(int arg1) {
    if (arg1 == 1)
        refreshList();
}

void dashboard::on_pushButton_5_released() {
    QModelIndexList selectedIndexes =
        ui->tableView->selectionModel()->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        int selectedRow = selectedIndexes.first().row();

        // Ask the user where they want to save the PDF file
        QString fileName = QFileDialog::getSaveFileName(
            this, tr("Save PDF File"), "", tr("PDF Files (*.pdf)"));

        if (fileName.isEmpty()) {
            return;
        }

        QString currentPath = QCoreApplication::applicationDirPath();
        // Read the HTML template file
        QFile file(
            currentPath +
            "../../../AMS/libraries/docs/attestation.html"); // Assuming the
                                                             // HTML template is
                                                             // stored as a
                                                             // resource file
        qDebug() << file.fileName();
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

            QMessageBox::critical(this, tr("Error"),
                                  tr("Could not open HTML template file."));
            return;
        }
        QTextStream in(&file);
        in.setCodec("UTF-8");
        QString htmlTemplate = in.readAll();
        file.close();
        qDebug() << htmlTemplate;

        // Replace placeholders in the HTML template with actual values
        QString name = ui->tableView->model()
                           ->data(ui->tableView->model()->index(selectedRow, 3))
                           .toString();
        QString cin = ui->tableView->model()
                          ->data(ui->tableView->model()->index(selectedRow, 0))
                          .toString();
        QString currentDate = QDate::currentDate().toString(
            Qt::ISODate); // Using ISODate format for consistency
        QString html = htmlTemplate.arg(name).arg(currentDate).arg(cin);

        QString tmpHtmlFilePath =
            currentPath + "../../../AMS/libraries/docs/temp.html";
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
        QString wkhtmltopdfPath =
            currentPath + "../../../AMS/libraries/wkhtmltopdf/bin/"
                          "wkhtmltopdf.exe"; // Set the path to wkhtmltopdf
                                             // executable
        QStringList args;
        args << "-q"
             << "-n"
             << "--enable-local-file-access"; // Optional flags to suppress
                                              // output and disable JavaScript
        args << tmpHtmlFilePath << fileName;
        QProcess process;
        process.start(wkhtmltopdfPath, args);
        if (!process.waitForStarted() || !process.waitForFinished()) {
            qDebug()
                << "Error: Failed to start or wait for wkhtmltopdf process";
        }

        // Remove the temporary HTML file
        QFile::remove(tmpHtmlFilePath);

    } else {
        QMessageBox::warning(nullptr, "Error", "Please select a row first.");
    }
}
void dashboard::on_pushButton_7_released() {
    QModelIndexList selectedIndexes =
        ui->tableView->selectionModel()->selectedIndexes();
    if (!selectedIndexes.isEmpty()) {

        // Get the row number of the selected item
        int selectedRow = selectedIndexes.first().row();
        QString cin = ui->tableView->model()
                          ->data(ui->tableView->model()->index(selectedRow, 0))
                          .toString();
        // Loop over all columns and retrieve the data
        QString type = ui->tableView->model()
                           ->data(ui->tableView->model()->index(selectedRow, 1))
                           .toString();
        QString email =
            ui->tableView->model()
                ->data(ui->tableView->model()->index(selectedRow, 2))
                .toString();
        QString name = ui->tableView->model()
                           ->data(ui->tableView->model()->index(selectedRow, 3))
                           .toString();
        QString lastname =
            ui->tableView->model()
                ->data(ui->tableView->model()->index(selectedRow, 4))
                .toString();
        QString dob = ui->tableView->model()
                          ->data(ui->tableView->model()->index(selectedRow, 5))
                          .toString();

        QString phone =
            ui->tableView->model()
                ->data(ui->tableView->model()->index(selectedRow, 6))
                .toString();

        QString remarks =
            ui->tableView->model()
                ->data(ui->tableView->model()->index(selectedRow, 7))
                .toString();


        int typ;
        if (type == "administrator")
            typ = 1;
        if (type == "comms_emp")
            typ = 2;
        if (type == "hospital_emp")
            typ = 3;
        if (type == "patientmgr_emp")
            typ = 4;
        if (type == "ambulance_emp")
            typ = 5;
        employee edel(cin, typ, email, name, lastname, dob, remarks, phone, 1);
        A.close_arduino();

        auto *dialog = new ModifyEmployee(&edel, this);
        dialog->setModal(false);
        dialog->exec();
        dialog->deleteLater();
        refreshList();

    } else
        QMessageBox::warning(nullptr, "Error", "Please select a row first.");
}

void dashboard::on_AboutBtn_released() {
    if (ui->stackedWidget->currentIndex() != 4)
        ui->stackedWidget->setCurrentIndex(4);
}

void dashboard::on_pushButton_4_released() {
    cap = new cv::VideoCapture(0);
    if (!cap->isOpened()) {
        QMessageBox::critical(nullptr, "Error", "Could not open camera");
        return;
    }
    // Set the capture frame rate to 30 fps
    cap->set(cv::CAP_PROP_FPS, 30);

    ui->pushButton_4->setEnabled(false);

    timerCam->start(33); // 30ms delay between updates
}

bool dashboard::updateCam() {
    bool stat = false;
    *cap >> frame;
    if (frame.empty()) {
        qDebug("Error: Could not capture frame");
        return stat;
    }

    // Convert the frame to grayscale
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // Create a ZBar scanner object
    zbar::ImageScanner scanner;

    // Configure the scanner to look for barcodes
    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

    // Wrap the OpenCV frame with a ZBar image
    zbar::Image image(frame.cols, frame.rows, "Y800", gray.data,
                      frame.cols * frame.rows);

    // Scan the image for barcodes
    scanner.scan(image);

    // Loop over all the barcodes and print the datagh
    for (zbar::Image::SymbolIterator symbol = image.symbol_begin();
         symbol != image.symbol_end(); ++symbol) {
        // Get the location of the QR code in the frame
        std::vector<cv::Point> points;
        for (int i = 0; i < symbol->get_location_size(); i++) {
            points.push_back(cv::Point(symbol->get_location_x(i),
                                       symbol->get_location_y(i)));
        }
        // Draw a rectangle around the QR code
        cv::polylines(frame, points, true, cv::Scalar(0, 0, 255), 2);

        QString data = QString::fromStdString(symbol->get_data());
        qDebug() << "QR code data:" << data;
        if (data.contains("-AMS-TN") && !ui->qrenable->isChecked()){
            data.replace("-AMS-TN", "");
            QSqlQuery query;
            query.prepare("SELECT * FROM EMPLOYEE WHERE CIN = :cin");
            query.bindValue(":cin", data);
            query.exec();
            if(query.next()){
                QMessageBox::information(this, "Information", "User scanned is " + query.value("NOM").toString() + " " + query.value("PRENOM").toString() + " works as the type number " + query.value("TYPE").toString());
                stat = true;
            }
        }
        if (ui->qrenable->isChecked())
            ui->textEdit_2->append(data + "\n");
    }

    // Convert the frame to a QImage for display
    QImage img = QImage((const unsigned char *)(frame.data), frame.cols,
                        frame.rows, QImage::Format_RGB888)
                     .rgbSwapped();
    ui->label_32->setPixmap(QPixmap::fromImage(img));
    ui->label_32->setScaledContents(true);
    qApp->processEvents();
    return stat;
}

void dashboard::on_ScanBtn_released() {
    if (ui->stackedWidget->currentIndex() != 3) {
        ui->stackedWidget->setCurrentIndex(3);
        cap = new cv::VideoCapture(0);
        if (!cap->isOpened()) {
            QMessageBox::critical(nullptr, "Error", "Could not open camera");
            return;
        }
        // Set the capture frame rate to 30 fps
        cap->set(cv::CAP_PROP_FPS, 30);

        ui->pushButton_4->setEnabled(false);
        timerCam->start(33); // 30ms delay between updates
    }
}

void dashboard::on_checkBox_3_released()
{

}

void dashboard::on_checkBox_3_toggled(bool checked)
{
    if(checked == true){
    if (A.isOpen()){
        if (A.getarduino_port_name() == FINGERPRINT_PORT){
            return;
        }else{
            A.close_arduino();
            if (A.connect_arduino(FINGERPRINT_PORT) == 0)
                return;
            else{
                QMessageBox::warning(this, "Error",
                                     "Arduino Not Available...");
                ui->checkBox_3->setChecked(false);
            }
        }
    }else{
        if (A.connect_arduino(FINGERPRINT_PORT) == 0)
            return;
        else{
            QMessageBox::warning(this, "Error",
                                 "Arduino Not Available...");
            ui->checkBox_3->setChecked(false);
        }
    }
    }
}

void dashboard::on_checkBox_4_toggled(bool checked)
{
    if (checked && ui->New_Type->currentIndex() != 1
            && ui->New_Type->currentIndex() != 4){
        QMessageBox::warning(this, "Error", "Incorrect Department: No User mode for this type");
        ui->checkBox_4->setChecked(false);
    }
}

void dashboard::on_New_Type_currentIndexChanged(int index)
{
    if (ui->checkBox_4->isChecked() && index != 1 && index != 4){
        QMessageBox::warning(this, "Error", "Incorrect Department: No User mode for this type");
        ui->checkBox_4->setChecked(false);
    }
}

void dashboard::toggleDropdownWidget() {
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
        animation->setStartValue(QRect(780, 70, 231, 0));
        animation->setEndValue(
            QRect(780, 70, 231,
                  ui->dropdownWidget
                      ->maximumHeight())); // Set the desired height of the
                                           // dropdownWidget when expanded
        animation->start();
    } else if (ui->dropdownWidget->height() ==
               ui->dropdownWidget->maximumHeight()) {
        // Hide the widget (collapse it)
        animation->setStartValue(
            QRect(780, 70, 231, ui->dropdownWidget->maximumHeight()));
        animation->setEndValue(QRect(780, 70, 231, 0));
        animation->start();
    }
}

void dashboard::toggleDropdownWidget2() {
    QPropertyAnimation *animation = nullptr;
    if (!animation) {
        animation =
            new QPropertyAnimation(ui->dropdownWidget_2, "geometry", this);
        animation->setDuration(
            400); // Set the duration of the animation (in milliseconds)
        animation->setEasingCurve(
            QEasingCurve::InOutCubic); // Set the easing curve for the animation
    }
    if (ui->dropdownWidget_2->height() == 0) {
        // Show the widget (expand it)
        animation->setStartValue(QRect(60, 71, 231, 0));
        animation->setEndValue(
            QRect(60, 71, 231,
                  ui->dropdownWidget_2
                      ->maximumHeight())); // Set the desired height of the
                                           // dropdownWidget when expanded
        animation->start();
    } else if (ui->dropdownWidget_2->height() ==
               ui->dropdownWidget_2->maximumHeight()) {
        // Hide the widget (collapse it)
        animation->setStartValue(
            QRect(60, 71, 231, ui->dropdownWidget_2->maximumHeight()));
        animation->setEndValue(QRect(60, 71, 231, 0));
        animation->start();
    }
}
void dashboard::on_pushButton_15_released() {
    toggleDropdownWidget();
}

void dashboard::on_pushButton_20_released()
{
    A.close_arduino();
    auto *dialog = new ModifyEmployee(&e, this);
    dialog->exec();
    dialog->deleteLater();
    refreshList();
    ui->lineEdit_6->setText(e.getCIN());
    ui->lineEdit_3->setText(e.getPrenom());
    ui->lineEdit_4->setText(e.getNom());
    ui->lineEdit_5->setText(e.getMail());
}

void dashboard::on_settingsBtn_released()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void dashboard::on_pushButton_23_released()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void dashboard::on_pushButton_24_released()
{
    on_logOut_released();
}

void dashboard::on_pushButton_10_toggled(bool checked) {
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
        ui->pushButton_2->setDisabled(true);
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
            ui->pushButton_2->setEnabled(true);

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

void dashboard::on_comboBox_currentIndexChanged(int index)
{
    on_lineEdit_textChanged("");
    Q_UNUSED(index);
}

void dashboard::on_pushButton_10_released()
{

}

void dashboard::on_pushButton_13_released()
{
    toggleDropdownWidget2();
}
