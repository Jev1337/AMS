#include "dashboard_comms.h"
#include "ui_dashboard_comms.h"

dashboard_comms::dashboard_comms(employee &e, QWidget *parent)
    : CFramelessWindow(parent), ui(new Ui::dashboard_comms) {
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    chartView = new QtCharts::QChartView(ui->widget_52);
    ui->lineEdit_10->setText(e.getCIN());
    ui->lineEdit_5->setText(e.getPrenom());
    ui->lineEdit_6->setText(e.getNom());
    ui->lineEdit_9->setText(e.getMail());
    this->e = e;
    if (e.getAdmin())
        admin = true;
    else{
        admin = false;
        ui->pushButton_6->setEnabled(false);
        auto *dialog = new userSelection(e);
        if (dialog->exec() == QDialog::Accepted) {
            depart = dialog->getResult();
            ui->stackedWidget->setCurrentIndex(2);
            ui->pushButton_6->setText("Switch to Admin Interface");
            ui->dashboardBtn->hide();
            ui->CUDBtn->hide();
            ui->ChatBtn->move(ui->ChatBtn->pos().x(),
                              ui->ChatBtn->pos().y() - 100);
            ui->EmergBtn->move(ui->EmergBtn->pos().x(),
                               ui->EmergBtn->pos().y() - 100);
            ui->VoiceBtn->move(ui->EmergBtn->pos().x(),
                               ui->VoiceBtn->pos().y() - 100);
            ui->AboutBtn->move(ui->AboutBtn->pos().x(),
                               ui->AboutBtn->pos().y() - 100);
        }else{
            on_logOut_released();
            return;
        }
    }
    communication c;
    refresh();
    ui->greetingIns->setText(this->e.getNom() + " " + this->e.getPrenom());
    ui->Greeting->setText(ui->Greeting->text().append(this->e.getNom() + "!"));
    setTitleBar(ui->widget_2);
    ui->dropdownWidget->installEventFilter(this);
    ui->widget->installEventFilter(this);
    ui->centralwidget->installEventFilter(this);
    on_pushButton_19_released();

    // voice of welcoming
    QTextToSpeech *tts = new QTextToSpeech(this);
    tts->setLocale(QLocale(QLocale::English));
    tts->setVoice(tts->availableVoices().at(1));
    tts->setPitch(20);
    QString employee;
    employee =
        "Hello" + e.getNom() + ", Welcome to Ambulance Management System!";
    tts->say(employee);
}

void dashboard_comms::refresh(){
    communication c;
    refreshList();
    ui->label_35->setText(c.stat());
    ui->label_43->setText(c.getBest()[0]);
    ui->label_40->setText("With "+ c.getBest()[1] + " Emergency Calls");
    ui->label_48->setText(c.getFinished());
    c.getEmergencyCallStat(chartView);
    showArdStats("[Com] Total = " + c.stat() + " Best ID = " + c.getBest()[0]);
}

void dashboard_comms::showArdStats(QString message){

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

bool dashboard_comms::eventFilter(QObject *watched, QEvent *event) {
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
    }
    return false;
}

dashboard_comms::~dashboard_comms() { delete ui; }

void dashboard_comms::on_pushButton_released() { QApplication::quit(); }

void dashboard_comms::on_pushButton_27_released() {
    // do nothing
}

void dashboard_comms::changeToMain(int index){
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

void dashboard_comms::on_pushButton_28_released() {
    changeToMain(1);
}

void dashboard_comms::on_pushButton_29_released() {
    changeToMain(2);
}

void dashboard_comms::on_pushButton_30_released() {
    changeToMain(3);
}

void dashboard_comms::on_pushButton_36_released() {
    changeToMain(0);
}

void dashboard_comms::on_pushButton_38_released() {
    changeToMain(2);
}

void dashboard_comms::on_pushButton_39_released() {
    changeToMain(3);
}

void dashboard_comms::on_pushButton_120_released() {
    changeToMain(0);
}

void dashboard_comms::on_pushButton_121_released() {
    changeToMain(1);
}

void dashboard_comms::on_pushButton_123_released() {
    changeToMain(3);
}

void dashboard_comms::on_pushButton_124_released() {
    changeToMain(0);
}

void dashboard_comms::on_pushButton_125_released() {
    changeToMain(1);
}

void dashboard_comms::on_pushButton_126_released() {
    changeToMain(2);
}

void dashboard_comms::on_pushButton_31_released() {
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

void dashboard_comms::on_logOut_released() {
    if (ui->pushButton_16->isChecked())
        on_pushButton_16_toggled(false);
    A.close_arduino();
    QTimer::singleShot(0, this, SLOT(close()));
    auto w = new MainWindow();
    w->show();
}

void dashboard_comms::on_dashboardBtn_released() {
    if (ui->stackedWidget->currentIndex() != 0) {
        ui->stackedWidget->setCurrentIndex(0);
        refresh();
    }
}

void dashboard_comms::on_CUDBtn_released() {

    if (ui->stackedWidget->currentIndex() != 1) {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void dashboard_comms::on_ChatBtn_released() {
    if (ui->stackedWidget->currentIndex() != 2) {
        ui->stackedWidget->setCurrentIndex(2);
    }
}

void dashboard_comms::sendOpenAIRequest() {
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
        ui->label_51->setMovie(movie);
        movie->start();
        ui->label_51->setVisible(true);
        connect(reply, &QNetworkReply::finished, this, [reply, movie, this]() {
            ui->label_51->setVisible(false);
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

void dashboard_comms::on_sendChatBtn_released() { sendOpenAIRequest(); }

void dashboard_comms::refreshList() {
    // Create a QSqlTableModel to fetch data from the database
    QSqlTableModel *model = new QSqlTableModel();
    model->setTable("communicationcenter");
    model->removeColumn(model->fieldIndex("created_at"));
    model->removeColumn(model->fieldIndex("updated_at"));
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Address");
    model->setHeaderData(3, Qt::Horizontal, "Phone Number");
    model->setHeaderData(4, Qt::Horizontal, "Email");
    model->setHeaderData(5, Qt::Horizontal, "Remarks");
    model->select();

    // Create a QTableView to display the data
    ui->tableView->setModel(model);

    // Add the QTableView to your UI
    ui->gridLayout->addWidget(ui->tableView, 0, 0);
}

void dashboard_comms::on_pushButton_33_released() {
    if (ui->stackedWidget_6->currentIndex() != 1) {
        ui->stackedWidget_6->setCurrentIndex(2);
        QMovie *movie = new QMovie(":/assets/images/preloader.gif");
        ui->label_7->setMovie(movie);
        movie->start();
        QTimer *timer = new QTimer();
        QObject::connect(timer, &QTimer::timeout, [=]() {
            ui->stackedWidget_6->setCurrentIndex(1);
            timer->stop();
            timer->deleteLater();
            movie->stop();
            movie->deleteLater();
        });
        timer->start(300);
    }
    refresh();
}

void dashboard_comms::on_pushButton_48_released() {
    if (ui->stackedWidget_6->currentIndex() != 0) {
        ui->stackedWidget_6->setCurrentIndex(2);
        QMovie *movie = new QMovie(":/assets/images/preloader.gif");
        ui->label_7->setMovie(movie);
        movie->start();
        QTimer *timer = new QTimer();
        QObject::connect(timer, &QTimer::timeout, [=]() {
            ui->stackedWidget_6->setCurrentIndex(0);
            timer->stop();
            timer->deleteLater();
            movie->stop();
            movie->deleteLater();
        });
        timer->start(300);
    }
}

void dashboard_comms::on_New_Btn_released() {
    QString idString = ui->New_ID->text();
    bool ok;
    int id = idString.toInt(&ok);
    QString email = ui->New_Email->text();
    QString phone = ui->New_Phone->text();

    if (!ok) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: ID must contain only numbers!");
        return;
    }
    if (idString.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "Error: Please input your id!");
        return;
    }
    // ID is valid, continue with your code
    if (ui->New_Name_2->text() == "") {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input your name!");
        return;
    }
    if (ui->New_Address->text() == "") {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input your adresse!");
        return;
    }
    if (email.isEmpty()) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input your email!");
        return;
    }
    if (!email.contains('@') || !email.contains('.')) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input a valid email address!");
        return;
    }
    if (phone.isEmpty()) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input your phone!");
        return;
    }
    for (const QChar &c : phone) {
        if (!c.isDigit()) {
            QMessageBox::warning(
                nullptr, "Error",
                "Error: Phone number must contain only numbers!");
            return;
        }
    }
    communication c(id, ui->New_Name_2->text(), ui->New_Address->text(),
                    ui->New_Phone->text(), ui->New_Email->text(),
                    ui->New_Remarks->toPlainText());
    c.ajouter();

    // say the comm has been added :
    if (ui->ttsenable){
        QTextToSpeech *tts1 = new QTextToSpeech(this);
        tts1->setLocale(QLocale(QLocale::English));
        tts1->setVoice(tts1->availableVoices().at(1));
        tts1->setPitch(20);
        QString message;
        message = "The Communication " + idString + " has been added!";
        tts1->say(message);
    }
    showArdStats("[Com] Added CommCtr: " + idString);
}

void dashboard_comms::on_pushButton_3_released() {
    QModelIndexList selectedIndexes =
        ui->tableView->selectionModel()->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {

        QMessageBox::StandardButton reply = QMessageBox::question(
            nullptr, "Confirmation",
            "Are you sure? Once removed, the communication center and it's "
            "relations will be gone.",
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // Get the row number of the selected item
            int selectedRow = selectedIndexes.first().row();
            int id = ui->tableView->model()
                         ->data(ui->tableView->model()->index(selectedRow, 0))
                         .toInt();
            // Loop over all columns and retrieve the data
            QString nom =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 1))
                    .toString();
            QString adresse =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 2))
                    .toString();
            QString numtel =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 3))
                    .toString();
            QString email =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 4))
                    .toString();
            QString remarks =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 5))
                    .toString();
            communication c(id, nom, adresse, numtel, email, remarks);
            c.supprimer();
            refresh();
        }
    } else
        QMessageBox::warning(nullptr, "Error", "Please select a row first.");
}

void dashboard_comms::on_pushButton_4_released() {
    QModelIndexList selectedIndexes =
        ui->tableView->selectionModel()->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        // Get the row number of the selected item
        int selectedRow = selectedIndexes.first().row();
        int id = ui->tableView->model()
                     ->data(ui->tableView->model()->index(selectedRow, 0))
                     .toInt();
        // Loop over all columns and retrieve the data
        QString nom = ui->tableView->model()
                          ->data(ui->tableView->model()->index(selectedRow, 1))
                          .toString();
        QString adresse =
            ui->tableView->model()
                ->data(ui->tableView->model()->index(selectedRow, 2))
                .toString();
        QString numtel =
            ui->tableView->model()
                ->data(ui->tableView->model()->index(selectedRow, 3))
                .toString();
        QString email =
            ui->tableView->model()
                ->data(ui->tableView->model()->index(selectedRow, 4))
                .toString();
        QString remarks =
            ui->tableView->model()
                ->data(ui->tableView->model()->index(selectedRow, 5))
                .toString();
        communication c(id, nom, adresse, numtel, email, remarks);
        A.close_arduino();
        auto *dialog = new modifycomms(&c, this);
        dialog->setModal(false);
        dialog->exec();
        refresh();
    } else
        QMessageBox::warning(nullptr, "Error", "Please select a row first.");
}

void dashboard_comms::on_EmergBtn_released() {
    if (admin) {
        QMessageBox::warning(nullptr, "Error",
                             "You need to be an User! Please switch!");
        return;
    }

    if (ui->stackedWidget->currentIndex() != 3) {
        ui->stackedWidget->setCurrentIndex(3);
    }
}

void dashboard_comms::on_stackedWidget_currentChanged(int arg1) {
    if (arg1 == 3) {
        QQuickItem * obj = ui->quickWidget->rootObject();
        connect(this, SIGNAL(setCenter(QVariant, QVariant)), obj,
                SLOT(setCenter(QVariant, QVariant)));
        connect(this, SIGNAL(addMarker(QVariant, QVariant)), obj,
                SLOT(addMarker(QVariant, QVariant)));
        connect(obj, SIGNAL(coordinateSelected(double, double)), this,
                SLOT(onCoordinateSelected(double, double)));
        emit setCenter(36.900068737969, 10.189164289212641);
        emit addMarker(36.900068737969, 10.189164289212641);
    }
    if (arg1 == 0) {
        communication c;
        ui->label_35->setText(c.stat());
    }
    if (ui->pushButton_16->isChecked() && arg1 != 2)
        on_pushButton_16_toggled(false);
}

void dashboard_comms::onCoordinateSelected(double latitude, double longitude) {
    latit = latitude;
    longit = longitude;
}

void dashboard_comms::on_pushButton_2_released() {
    QUrl url("https://atlas.microsoft.com/search/address/json?");
    QUrlQuery query;
    query.addQueryItem("subscription-key",
                       "Ct8DmwBEG7voYEOXYg8P3CkuN8O1WQ3c_JWOIUshqUw");
    query.addQueryItem("api-version", "1.0");
    query.addQueryItem("query", ui->lineEdit->text());
    query.addQueryItem("limit", "1");
    query.addQueryItem("countrySet", "TN");
    url.setQuery(query);

    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray data = reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    if (!obj.contains("results")) {
        // Handle error
        return;
    }

    QJsonArray results = obj["results"].toArray();
    if (results.isEmpty()) {
        // Handle error
        return;
    }

    QJsonObject result = results[0].toObject();
    QJsonObject position = result["position"].toObject();

    double lat = position["lat"].toDouble();
    double lng = position["lon"].toDouble();

    emit addMarker(lat, lng);
    emit setCenter(lat, lng);
}

void dashboard_comms::on_pushButton_7_released() {
    qDebug() << latit;
    qDebug() << longit;
    ui->stackedWidget_2->setCurrentIndex(1);
}

void dashboard_comms::toggleDropdownWidget() {
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
void dashboard_comms::on_pushButton_15_released() { toggleDropdownWidget(); }

void dashboard_comms::on_pushButton_6_released() {
    if (admin) {
        auto *dialog = new userSelection(e, this);
        if (dialog->exec() == QDialog::Accepted) {
            admin = false;
            depart = dialog->getResult();
            ui->stackedWidget->setCurrentIndex(2);
            ui->pushButton_6->setText("Switch to Admin Interface");
            ui->dashboardBtn->hide();
            ui->CUDBtn->hide();
            ui->ChatBtn->move(ui->ChatBtn->pos().x(),
                              ui->ChatBtn->pos().y() - 100);
            ui->EmergBtn->move(ui->EmergBtn->pos().x(),
                               ui->EmergBtn->pos().y() - 100);
            ui->VoiceBtn->move(ui->EmergBtn->pos().x(),
                               ui->VoiceBtn->pos().y() - 100);
            ui->AboutBtn->move(ui->AboutBtn->pos().x(),
                               ui->AboutBtn->pos().y() - 100);
        }
    } else {
        admin = true;
        ui->stackedWidget->setCurrentIndex(0);
        ui->pushButton_6->setText("Switch to User Interface");
        ui->dashboardBtn->show();
        ui->CUDBtn->show();
        ui->ChatBtn->move(ui->ChatBtn->pos().x(), ui->ChatBtn->pos().y() + 100);
        ui->EmergBtn->move(ui->EmergBtn->pos().x(),
                           ui->EmergBtn->pos().y() + 100);
        ui->VoiceBtn->move(ui->EmergBtn->pos().x(),
                           ui->VoiceBtn->pos().y() + 100);
        ui->AboutBtn->move(ui->AboutBtn->pos().x(),
                           ui->AboutBtn->pos().y() + 100);
    }
}

void dashboard_comms::on_pushButton_9_released() {
    QSqlQuery query;
    query.prepare("INSERT INTO EMERGENCYCALL "
                  "(PROBLEM,STATUS,ADRESSE,COMMUNICATION_CENTER_ID , CREATED_AT , UPDATED_AT) VALUES "
                  "(:problem,'2',:adr, :id, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)");
    query.bindValue(":problem", ui->textEdit_2->toPlainText());
    query.bindValue(":adr",
                    QString::number(latit) + ", " + QString::number(longit));
    query.bindValue(":id", depart);
    query.exec();
    QMessageBox::information(
        nullptr, "Info",
        "Success! All hospitals are informed of this emergency!");
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(2);
}

void dashboard_comms::on_lineEdit_3_textChanged(const QString &arg1) {
    Q_UNUSED(arg1);
    refreshList();
    QString searchText = ui->lineEdit_3->text();
    int searchColumn = ui->comboBox_2->currentIndex();
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

void dashboard_comms::on_pushButton_17_released() {
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
            "../../../AMS/libraries/docs/commctr.html"); // Assuming the
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
        QString id = ui->tableView->model()
                           ->data(ui->tableView->model()->index(selectedRow, 0))
                           .toString();
        communication c;
        QString date = c.getDatefromId(id.toInt());
        QString currentDate = QDate::currentDate().toString(Qt::ISODate); // Using ISODate format for consistency
        QString html = htmlTemplate.arg(id).arg(date).arg(currentDate);

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

void dashboard_comms::on_VoiceBtn_released() {
    if (ui->stackedWidget->currentIndex() != 4)
        ui->stackedWidget->setCurrentIndex(4);
}

void dashboard_comms::on_pushButton_11_released() {
    QModelIndexList selectedIndexes =
        ui->tableView_2->selectionModel()->selectedIndexes();
    if (!selectedIndexes.isEmpty()) {
        QString filename =
            ui->tableView_2->model()->data(selectedIndexes.at(0)).toString();
        QString filenameEncoded = QUrl::toPercentEncoding(filename);

        QUrl url(
            QString("https://grandelation.com/static/assets/uploads/wav/%1")
                .arg(filenameEncoded));

        player->setMedia(QMediaContent(url));
        connect(player, &QMediaPlayer::stateChanged, this,
                [=](QMediaPlayer::State state) {
                    if (state == QMediaPlayer::StoppedState) {
                        ui->pushButton_11->setEnabled(true);
                    }
                });
        player->play();
        ui->pushButton_11->setEnabled(false);

    } else {
        // No row selected
    }
}

void dashboard_comms::on_pushButton_14_released() { player->stop(); }

void dashboard_comms::on_pushButton_18_released() {
    QModelIndexList selectedIndexes =
        ui->tableView_2->selectionModel()->selectedIndexes();
    if (!selectedIndexes.isEmpty()) {
        QString filename =
            ui->tableView_2->model()->data(selectedIndexes.at(0)).toString();
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this,
                [=](QNetworkReply *reply) {
                    if (reply->error() == QNetworkReply::NoError) {
                        qDebug() << "File deleted successfully";
                        on_pushButton_19_released();
                    } else {
                        qDebug()
                            << "Error deleting file:" << reply->errorString();
                    }

                    reply->deleteLater();
                    manager->deleteLater();
                });
        QString filenameEncoded = QUrl::toPercentEncoding(filename);
        QUrl url(
            QString("https://grandelation.com/file/%1").arg(filenameEncoded));
        QNetworkRequest request(url);
        manager->deleteResource(request);
    }
}

void dashboard_comms::on_pushButton_10_released() {}

void dashboard_comms::on_pushButton_10_toggled(bool checked) {
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
        audioRecorder->stop();
        QString exeDir = QCoreApplication::applicationDirPath();
        // Prompt user for file name
        QString fileName =
            QInputDialog::getText(this, "Save Recording", "Enter a file name:");
        if (fileName.isEmpty()) {
            // User cancelled or entered an empty file name
            qDebug("Recording discarded.");
            QFile::remove(exeDir + "/temp.wav");
            return;
        }

        // Upload the file to the server
        QUrl url("https://grandelation.com/file");
        QHttpMultiPart *multiPart =
            new QHttpMultiPart(QHttpMultiPart::FormDataType);

        // Add the file part
        QHttpPart filePart;
        filePart.setHeader(QNetworkRequest::ContentTypeHeader,
                           QVariant("audio/wav"));
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QVariant("form-data; name=\"file\"; filename=\"" +
                                    fileName + ".wav\""));
        QFile *file = new QFile(exeDir + "/temp.wav");
        file->open(QIODevice::ReadOnly);
        filePart.setBodyDevice(file);
        file->setParent(multiPart); // we cannot delete the file from memory
                                    // now, so delete it with the multiPart
        multiPart->append(filePart);

        // Send the request
        QNetworkRequest request(url);
        QNetworkAccessManager manager;
        QNetworkReply *reply = manager.post(request, multiPart);
        multiPart->setParent(reply); // delete the multiPart with the reply
        qDebug("Uploading recording...");

        // Wait for the reply to finish
        QEventLoop loop;
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        // Check if the upload was successful
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error uploading recording:" << reply->errorString();
        } else {
            qDebug() << "Recording uploaded successfully.";
            on_pushButton_19_released();
        }

        // Clean up
        QFile::remove(exeDir + "/temp.wav");
    }
}

void dashboard_comms::on_pushButton_19_released() {
    QStandardItemModel *model = new QStandardItemModel(this);
    ui->tableView_2->setModel(model);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished, this,
            [=](QNetworkReply *reply) {
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray data = reply->readAll();
                    QJsonDocument doc = QJsonDocument::fromJson(data);
                    QJsonArray files = doc.object()["files"].toArray();
                    model->setHorizontalHeaderLabels(
                        {"filename", "Length", "Creation Date"});
                    qDebug() << data;
                    for (const auto &file : files) {
                        QJsonObject fileObj = file.toObject();
                        QList<QStandardItem *> rowItems;

                        rowItems.append(
                            new QStandardItem(fileObj["filename"].toString()));
                        rowItems.append(new QStandardItem(
                            QString::number(fileObj["length"].toDouble())));
                        rowItems.append(new QStandardItem(
                            fileObj["created_date"].toString()));
                        model->appendRow(rowItems);
                    }
                } else {
                    qDebug() << "Bruh";
                }
                reply->deleteLater();
            });
    QUrl url("https://grandelation.com/files");
    QNetworkRequest request(url);
    manager->get(request);
}

void dashboard_comms::on_pushButton_16_toggled(bool checked)
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

void dashboard_comms::on_pushButton_61_released()
{
    A.close_arduino();
    auto *dialog = new ModifyEmployee(&e, this);
    dialog->exec();
    ui->lineEdit_10->setText(e.getCIN());
    ui->lineEdit_5->setText(e.getPrenom());
    ui->lineEdit_6->setText(e.getNom());
    ui->lineEdit_9->setText(e.getMail());
    refresh();
}

void dashboard_comms::on_AboutBtn_released()
{
    if (ui->stackedWidget->currentIndex() != 5)
        ui->stackedWidget->setCurrentIndex(5);
}

void dashboard_comms::on_settingsBtn_released()
{
    if (ui->stackedWidget->currentIndex() != 6)
        ui->stackedWidget->setCurrentIndex(6);
}

void dashboard_comms::on_stackedWidget_4_currentChanged(int arg1)
{
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
                    QMessageBox::warning(nullptr, "Error", "Error 0x06: Please make sure you are connected to a stable network!");
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
