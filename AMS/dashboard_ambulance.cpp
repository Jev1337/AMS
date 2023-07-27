#include "dashboard_ambulance.h"
#include "ui_dashboard_ambulance.h"

dashboard_ambulance::dashboard_ambulance(employee &e, QWidget *parent)
    : CFramelessWindow(parent), ui(new Ui::dashboard_ambulance) {
    ui->setupUi(this);
    QObject::connect(B.getserial(), SIGNAL(readyRead()), this,
                     SLOT(checkHeart()));
    ui->plot->addGraph();
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssNone);
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->plot->addGraph();
    ui->plot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->plot->graph(1)->setScatterStyle(QCPScatterStyle::ssNone);
    ui->plot->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->plot->yAxis->setRange(-800, 800, Qt::AlignRight);

    key = lastkey = 0;

    this->e = e;
    if (e.getAdmin())
        admin = true;
    else{
        admin = false;
        ui->pushButton_7->setEnabled(false);
        auto *dialog = new userSelection(e);
        if (dialog->exec() == QDialog::Accepted) {
            depart = dialog->getResult();
            ui->stackedWidget->setCurrentIndex(2);
            ui->pushButton_7->setText("Switch to Admin Interface");
            ui->dashboardBtn->hide();
            ui->CUDBtn->hide();
            ui->pushButton_12->hide();
            ui->ChatBtn->move(ui->ChatBtn->pos().x(),
                              ui->ChatBtn->pos().y() - 100);
            ui->EmergBtn->move(ui->EmergBtn->pos().x(),
                               ui->EmergBtn->pos().y() - 100);
            ui->AboutBtn->move(ui->AboutBtn->pos().x(),
                               ui->AboutBtn->pos().y() - 100); 
        }else{
            on_logOut_released();
            return;
        }
    }
    ui->lineEdit_6->setText(e.getCIN());
    ui->lineEdit_3->setText(e.getPrenom());
    ui->lineEdit_4->setText(e.getNom());
    ui->lineEdit_5->setText(e.getMail());
    ui->greetingIns->setText(this->e.getNom() + " " + this->e.getPrenom());
    ui->Greeting->setText(ui->Greeting->text().append(this->e.getNom() + "!"));
    setTitleBar(ui->widget_2);
    ui->dropdownWidget->installEventFilter(this);
    ui->widget->installEventFilter(this);
    ui->centralwidget->installEventFilter(this);
    ui->dropdownWidget_2->installEventFilter(this);

    QTextToSpeech *tts = new QTextToSpeech(this);
    tts->setLocale(QLocale(QLocale::English));
    tts->setVoice(tts->availableVoices().at(1));
    tts->setPitch(20);
    QString employee;
    employee =
        "Hello" + e.getNom() + ", Welcome to Ambulance Management System!";
    tts->say(employee);

    QSqlQuery qtotal, qbroken, qunbroken;
    qreal tot = 0, broken = 0, unbroken = 0;

    qtotal.prepare("SELECT count(*) FROM AMBULANCE");
    qtotal.exec();
    qtotal.next();
    tot = qtotal.value(0).toInt();

    qbroken.prepare("SELECT count(*) FROM AMBULANCE WHERE etat='1'");
    qbroken.exec();
    qbroken.next();
    broken = qbroken.value(0).toInt();

    qunbroken.prepare("SELECT count(*) FROM AMBULANCE WHERE etat='0'");
    qunbroken.exec();
    qunbroken.next();
    unbroken = qunbroken.value(0).toInt();

    if (tot != 0){
    broken = broken / tot;
    broken = broken * 100;
    unbroken = unbroken / tot;
    unbroken = unbroken * 100;
    }
    else{
        broken = 0;
        unbroken = 0;
   }

    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.1);
    series->setPieSize(0.6);
    series->append(QString("%1% broken").arg(broken), broken);

    QPieSlice *slice =
        series->append(QString("%1% unbroken").arg(unbroken), unbroken);

    slice->setExploded();
    slice->setLabelVisible();

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle("Ambulance Stats");

    QChartView *chartview = new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->resize(450, 301);
    // chartview->scale(1.1,1.);
    chartview->setParent(ui->widget_52);
    chartview->show();

    broken = broken / 100;
    broken = broken * tot;
    unbroken = unbroken / 100;
    unbroken = unbroken * tot;

    ui->label_35->setText(QString::number(tot));
    ui->label_43->setText(QString::number(unbroken));
    ui->label_48->setText(QString::number(broken));




    QStandardItemModel *model = a->getPendingConf();
    ui->tableView_4->setModel(model);
    ui->gridLayout_9->addWidget(ui->tableView_4, 0, 0);
    ui->pushButton_34->setText(QString::number(model->rowCount()));
    if (model->rowCount() > 0)
        tts->say("There are pending confirmation emergency calls!");

    if (!admin)
        loadmap();

    showArdStats("Amb: T="+QString::number(tot)+ " Ub="+QString::number(unbroken)+ " B="+QString::number(broken)+" ");
}


void dashboard_ambulance::checkHeart(){
    QByteArray data = B.read_from_arduino();
        buffer += QString::fromLatin1(data);

        // Split the buffer into lines
        QStringList lines = buffer.split("\r\n", QString::SkipEmptyParts);

        // Process each line separately
        foreach (const QString& line, lines) {
            // Parse the line and convert it to a numerical value
            double value = line.left(3).toDouble();
            qDebug() << value;
            // Append the value to the series

                ui->plot->graph(0)->addData(key, value-200);
                ui->plot->graph(0)->rescaleValueAxis();

            if (value - 200 > 300 && value - 200 < 500){
                ui->plot->graph(1)->addData(key, value-200);
                //ui->plot->graph(1)->rescaleValueAxis();
            }
            lastkey = key;
            key++;
        }

        // Clear the buffer for the processed lines
        buffer = lines.last();
        ui->plot->xAxis->setRange(key, 500, Qt::AlignRight);
        ui->plot->replot();
        ui->plot->update();


        // Update the chart view to reflect the new data
        //chartView->chart()->update();
}

void dashboard_ambulance::showArdStats(QString message){
    bool first = B.isOpen();
    int ret = 0;

    if (!first){
        ret = B.connect_arduino(FINGERPRINT_PORT); // lancer la connexion à arduino
        switch (ret) {
        case (0):
            qDebug() << "arduino is available and connected to : "
                     << B.getarduino_port_name();
            break;
        case (1):
            qDebug() << "given arduino is not available";
            return;
            break;

        }
     } else
        if(B.getarduino_port_name() != FINGERPRINT_PORT){
            first = false;
            B.close_arduino();
            ret = B.connect_arduino(FINGERPRINT_PORT); // lancer la connexion à arduino
            switch (ret) {
            case (0):
                qDebug() << "arduino is available and connected to : "
                         << B.getarduino_port_name();
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

                        B.write_to_arduino("l" + message.toUtf8());
                    });
                    else
                        B.write_to_arduino(message.toUtf8());

        }
}

void dashboard_ambulance::toggleDropdownWidget2(){
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
        animation->setStartValue(QRect(60, 71, 400, 0));
        animation->setEndValue(
            QRect(60, 71, 400,
                  ui->dropdownWidget_2
                      ->maximumHeight())); // Set the desired height of the
                                           // dropdownWidget when expanded
        animation->start();
    } else if (ui->dropdownWidget_2->height() ==
               ui->dropdownWidget_2->maximumHeight()) {
        // Hide the widget (collapse it)
        animation->setStartValue(
            QRect(60, 71, 400, ui->dropdownWidget_2->maximumHeight()));
        animation->setEndValue(QRect(60, 71, 400, 0));
        animation->start();
    }
}

bool dashboard_ambulance::eventFilter(QObject *watched, QEvent *event) {
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
    }else if (watched == ui->dropdownWidget_2){
        if (event->type() == QEvent::Leave) {
            toggleDropdownWidget2();
        }
    }
    return false;
}

dashboard_ambulance::~dashboard_ambulance() { delete ui; }

void dashboard_ambulance::on_pushButton_released() {
    B.close_arduino();
    QApplication::quit(); }

void dashboard_ambulance::on_pushButton_27_released() {}

void dashboard_ambulance::changeToMain(int index){
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

void dashboard_ambulance::on_pushButton_28_released() {
    changeToMain(1);
}
void dashboard_ambulance::on_pushButton_29_released() {
    changeToMain(2);
}
void dashboard_ambulance::on_pushButton_30_released() {
    changeToMain(3);
}
void dashboard_ambulance::on_pushButton_36_released() {
    changeToMain(0);
}
void dashboard_ambulance::on_pushButton_38_released() {
    changeToMain(2);
}
void dashboard_ambulance::on_pushButton_39_released() {
    changeToMain(3);
}
void dashboard_ambulance::on_pushButton_120_released() {
    changeToMain(0);
}
void dashboard_ambulance::on_pushButton_121_released() {
    changeToMain(1);
}
void dashboard_ambulance::on_pushButton_123_released() {
    changeToMain(3);
}
void dashboard_ambulance::on_pushButton_124_released() {
    changeToMain(0);
}
void dashboard_ambulance::on_pushButton_125_released() {
    changeToMain(1);
}
void dashboard_ambulance::on_pushButton_126_released() {
    changeToMain(2);
}

void dashboard_ambulance::on_pushButton_31_released() {
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
void dashboard_ambulance::on_logOut_released() {
    B.close_arduino();
    QTimer::singleShot(0, this, SLOT(close()));
    auto w = new MainWindow();
    w->show();
}
void dashboard_ambulance::on_dashboardBtn_released() {
    if (ui->stackedWidget->currentIndex() != 0) {

        QSqlQuery qtotal, qbroken, qunbroken;
        qreal tot = 0, broken = 0, unbroken = 0;

        qtotal.prepare("SELECT * FROM AMBULANCE");
        qtotal.exec();
        qbroken.prepare("SELECT * FROM AMBULANCE WHERE etat='1'");
        qbroken.exec();

        qunbroken.prepare("SELECT * FROM AMBULANCE WHERE etat='0'");
        qunbroken.exec();



        while (qtotal.next()) {
            tot++;
        }
        while (qbroken.next()) {
            broken++;
        }
        while (qunbroken.next()) {
            unbroken++;
        }
        ui->stackedWidget->setCurrentIndex(0);

            if (tot != 0){
            broken = broken / tot;
            broken = broken * 100;
            unbroken = unbroken / tot;
            unbroken = unbroken * 100;
            }
            else{
                broken = 0;
                unbroken = 0;
           }

        QPieSeries *series = new QPieSeries();
        series->setHoleSize(0.1);
        series->setPieSize(0.6);
        series->append(QString("%1% broken").arg(broken), broken);

        QPieSlice *slice =
            series->append(QString("%1% unbroken").arg(unbroken), unbroken);
        slice->setExploded();
        slice->setLabelVisible();

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->setTitle(":Ambulance Stats:");

        QChartView *chartview = new QChartView(chart);
        chartview->setRenderHint(QPainter::Antialiasing);
        chartview->resize(450, 301);
        // chartview->scale(1.1,1.3);
        chartview->setParent(ui->widget_52);
        chartview->show();

        broken = broken / 100;
        broken = broken * tot;
        unbroken = unbroken / 100;
        unbroken = unbroken * tot;

        ui->label_35->setText(QString::number(tot));
        ui->label_43->setText(QString::number(unbroken));
        ui->label_48->setText(QString::number(broken));

        ui->label_48->setText(QString::number(broken));


        //showArdStats("Amb: T="+QString::number(tot)+ " Ub="+QString::number(unbroken)+ " B="+QString::number(broken)+" ");



    }
}
void dashboard_ambulance::on_CUDBtn_released() {
    if (ui->stackedWidget->currentIndex() != 1) {
        ui->stackedWidget->setCurrentIndex(1);
    }
}
void dashboard_ambulance::on_ChatBtn_released() {
    if (ui->stackedWidget->currentIndex() != 2) {
        ui->stackedWidget->setCurrentIndex(2);
    }
}

void dashboard_ambulance::sendOpenAIRequest() {
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
        ui->label_128->setMovie(movie);
        movie->start();
        ui->label_128->setVisible(true);
        connect(reply, &QNetworkReply::finished, this, [reply, movie, this]() {
            ui->label_128->setVisible(false);
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

void dashboard_ambulance::on_sendChatBtn_released() { sendOpenAIRequest(); }

void dashboard_ambulance::on_pushButton_33_released() {
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
    refreshList();
}
void dashboard_ambulance::on_pushButton_48_released() {
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

void dashboard_ambulance::refreshList() {
    // Create a QSqlTableModel to fetch data from the database
    QSqlTableModel *model = new QSqlTableModel();
    model->setTable("ambulance");
    model->removeColumn(model->fieldIndex("created_at"));
    model->removeColumn(model->fieldIndex("updated_at"));
    model->setHeaderData(0, Qt::Horizontal, "    Reg number    ");
    model->setHeaderData(1, Qt::Horizontal, "    State    ");
    model->setHeaderData(2, Qt::Horizontal, "     Nb Employe    ");
    model->setHeaderData(3, Qt::Horizontal, "    Location    ");
    model->setHeaderData(4, Qt::Horizontal, "    Equipment    ");
    model->setHeaderData(5, Qt::Horizontal, "    Remarks    ");
    model->select();

    // Create a QTableView to display the data
    ui->tableView->setModel(model);

    // Add the QTableView to your UI
    ui->gridLayout_2->addWidget(ui->tableView, 0, 0);
}
void dashboard_ambulance::on_New_Btn_released() {
    QString Emp_nb = ui->Number_of_Employees->text();
    QString Loc = ui->Location->text();
    QString state = ui->State->text();
    QString matri = ui->Matricule->text();

    if (matri.isEmpty())
    {  QMessageBox::warning(nullptr, "Error",
                            "Error: Please input the registration number!");
        return;
    }

    for (const QChar &m : matri) {
        if (!m.isDigit())
        { QMessageBox::warning(nullptr, "Error",
                               "Error: Registration number must be a number!");
           return;
        }
    }

    if (matri.length() < 6) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please the registration number min length 6!");
        return;
    }

    if (state.isEmpty()) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input the state of the ambulance!");
        return;
    }

    for (const QChar &c : state)
    {  if (!c.isDigit())
        { QMessageBox::warning(nullptr, "Error",
                               "Error: State must be a number!");
            return;
        }
    }

    if (state != "1" && state != "0") {
        QMessageBox::warning(nullptr, "Error",
                            "Error: State must be 0 or 1!");
        return;
    }

    if (Emp_nb.isEmpty()) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input the number of employees!");
        return;
    }

    for (const QChar &c : Emp_nb) {
        if (!c.isDigit())
        { QMessageBox::warning(nullptr, "Error",
                               "Error: The number of Employees must be a number!");
            return;
        }
    }

    if (Loc.isEmpty()) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input the location !");
        return;
    }

    ambulance a(ui->Matricule->text(), ui->State->text(),
                ui->Number_of_Employees->text().toInt(), ui->Location->text(),
                ui->Equipment->text(), ui->Remarks->toPlainText());
    a.ajouter();

    showArdStats("[AMB] Added "+matri+" ");


    // say the ambulance has been added :
    if (ui->ttsenable){
        QTextToSpeech *tts1 = new QTextToSpeech(this);
        tts1->setLocale(QLocale(QLocale::English));
        tts1->setVoice(tts1->availableVoices().at(1));
        tts1->setPitch(20);
        QString message;
        message = "The ambulance " + matri + " has been added!";
        tts1->say(message);
    }
    ui->Matricule->setText("");
    ui->State->setText("");
    ui->Number_of_Employees->setText("");
    ui->Location->setText("");
    ui->Equipment->setText("");
    ui->Remarks->setText("");
}
void dashboard_ambulance::on_pushButton_3_released() {
    QModelIndexList selectedIndexes =
        ui->tableView->selectionModel()->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {

        QMessageBox::StandardButton reply =
            QMessageBox::question(nullptr, "Confirmation", "Are you sure?",
                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // Get the row number of the selected item
            int selectedRow = selectedIndexes.first().row();
            QString matricule =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 0))
                    .toString();
            // Loop over all columns and retrieve the data
            QString etat =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 1))
                    .toString();
            int capacite =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 2))
                    .toInt();
            QString emplacement =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 3))
                    .toString();
            QString equipements =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 4))
                    .toString();
            QString remarks =
                ui->tableView->model()
                    ->data(ui->tableView->model()->index(selectedRow, 5))
                    .toString();
            ambulance a(matricule, etat, capacite, emplacement, equipements,
                        remarks);
            a.supprimer();
            refreshList();

            // say the ambulance is modified
            if (ui->ttsenable){
                QTextToSpeech *tts1 = new QTextToSpeech(this);
                tts1->setLocale(QLocale(QLocale::English));
                tts1->setVoice(tts1->availableVoices().at(1));
                tts1->setPitch(20);
                QString message;
                message = "The ambulance " + matricule + " has been deleted!";
                tts1->say(message);
            }
        }
    } else
        QMessageBox::warning(nullptr, "Error", "Please select a row first.");
}
void dashboard_ambulance::on_pushButton_4_released() {
    QModelIndexList selectedIndexes =
        ui->tableView->selectionModel()->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        // Get the row number of the selected item
        int selectedRow = selectedIndexes.first().row();
        QString matricule =
            ui->tableView->model()
                ->data(ui->tableView->model()->index(selectedRow, 0))
                .toString();
        // Loop over all columns and retrieve the data
        QString etat = ui->tableView->model()
                           ->data(ui->tableView->model()->index(selectedRow, 1))
                           .toString();
        int capacite = ui->tableView->model()
                           ->data(ui->tableView->model()->index(selectedRow, 2))
                           .toInt();
        QString emplacement =
            ui->tableView->model()
                ->data(ui->tableView->model()->index(selectedRow, 3))
                .toString();
        QString equipements =
            ui->tableView->model()
                ->data(ui->tableView->model()->index(selectedRow, 4))
                .toString();
        QString remarks =
            ui->tableView->model()
                ->data(ui->tableView->model()->index(selectedRow, 5))
                .toString();
        ambulance a(matricule, etat, capacite, emplacement, equipements,
                    remarks);
        auto *dialog = new Modifyambulance(&a, this);
        dialog->setModal(false);
        dialog->exec();
        refreshList();

        // say the ambulance is modified
        if (ui->ttsenable){
            QTextToSpeech *tts1 = new QTextToSpeech(this);
            tts1->setLocale(QLocale(QLocale::English));
            tts1->setVoice(tts1->availableVoices().at(1));
            tts1->setPitch(20);
            QString message;
            message = "The ambulance " + matricule + " has been modified!";
            tts1->say(message);
        }

    } else
        QMessageBox::warning(nullptr, "Error", "Please select a row first.");
}
void dashboard_ambulance::on_pushButton_6_released() {
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
void dashboard_ambulance::on_pushButton_9_released() {
    QModelIndexList selectedIndexes =
        ui->tableView->selectionModel()->selectedIndexes();
    if (!selectedIndexes.isEmpty()) {
        int selectedRow = selectedIndexes.first().row();

        // Ask the user where they want to save the PDF file
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF File"), "", tr("PDF Files (*.pdf)"));

        if (fileName.isEmpty()) {
            return;
        }

        QString currentPath = QCoreApplication::applicationDirPath();
        // Read the HTML template file
        QFile file(currentPath + "../../../AMS/libraries/docs/ambulanceinfo.html"); // Assuming the HTML template is stored as a resource file
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

            QMessageBox::critical(this, tr("Error"), tr("Could not open HTML template file."));
            return;
        }
        QTextStream in(&file);
         in.setCodec("UTF-8");
         QString htmlTemplate = in.readAll();
         file.close();
/*
            "Ambulance registration number =  %1"
            "<br>"
            "State (0: broken) :              %2"
            "<br>"
            "Number of employees :            %3"
            "<br>"
            "Location :                       %4"
            "<br>"
            "Equipments :                     %5"
            "<br>"
            "Other:                           %6"
            "<br>"*/
        // Replace placeholders in the HTML template with actual values
         QString registrationNumber = ui->tableView->model()->index(selectedRow,0).data().toString();
         QString State = ui->tableView->model()->index(selectedRow,1).data().toString();
         QString NbEmp = ui->tableView->model()->index(selectedRow,2).data().toString();
         QString Location = ui->tableView->model()->index(selectedRow,3).data().toString();

        QString currentDate = QDate::currentDate().toString(Qt::ISODate); // Using ISODate format for consistency
        QString html = htmlTemplate.arg(currentDate).arg(registrationNumber).arg(Location).arg(NbEmp);


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
    } else
        QMessageBox::warning(nullptr, "Error", "Please select a row first.");
}

void dashboard_ambulance::on_pushButton_11_released() {
    selectMap dialog(this);
        connect(&dialog, &selectMap::locationEntered, this, &dashboard_ambulance::onDialogClosed);
        dialog.exec();
}
void dashboard_ambulance::onDialogClosed(const QString& text)
{
    ui->Location->setText(text);
}

void dashboard_ambulance::loadmap(){
    ui->cnf->setVisible(false);

    ui->quickWidget->setSource(QUrl("qrc:/map.qml"));

    auto obj = ui->quickWidget->rootObject();

    connect(this, SIGNAL(setRoute(QVariant, QVariant, QVariant, QVariant)), obj, SLOT(setRoute(QVariant, QVariant, QVariant, QVariant)));
    connect(this, SIGNAL(setCenter(QVariant, QVariant)), obj, SLOT(setCenter(QVariant, QVariant)));
    connect(this, SIGNAL(addMarker(QVariant, QVariant)), obj, SLOT(addMarker(QVariant, QVariant)));
    connect(this,SIGNAL(addHospitalMarker(QVariant, QVariant)),obj,SLOT(addHospitalMarker(QVariant, QVariant)));
    connect(this,SIGNAL(addAmbulanceMarker(QVariant, QVariant)),obj,SLOT(addAmbulanceMarker(QVariant, QVariant)));
    connect(this,SIGNAL(addPatientMarker(QVariant, QVariant,QVariant)),obj,SLOT(addPatientMarker(QVariant, QVariant,QVariant)));

    emit setCenter(36.900068737969, 10.189164289212641);

    // load all the available ambulance in live map
    QSqlQueryModel *modelCurrentAmb = new QSqlQueryModel();
    modelCurrentAmb->setQuery(QString("Select * from AMBULANCE Where matricule = %1").arg(depart));

    QString address = modelCurrentAmb->index(0, 3).data().toString();
    QStringList latLongList = address.split(",");
    QStringList listlatlong;
        if (latLongList.size() >=
            2) { // make sure the list has at least 2 elements
            addAmbulanceMarker(latLongList[0], latLongList[1]);
        }
        QSqlQuery query;
        query.prepare("SELECT Adresse, Status, Hospital_ID FROM EMERGENCYCALL WHERE MATRICULE_AMBULANCE = :mat AND status != 6");
        query.bindValue(":mat", depart);
        query.exec();
        query.next();
        if (query.value(1).toInt() == 3){
            ui->status->setText("Status: Waiting for Ambulance Arrival Confirmation");
            listlatlong = query.value(0).toString().split(",");
            addPatientMarker(listlatlong[0], listlatlong[1], 0);
            ui->cnf->setVisible(true);
        }else if (query.value(1).toInt() == 5){
            ui->status->setText("Status: Waiting for Ambulance Arrival Confirmation");
            QSqlQuery query1;
            query1.prepare("SELECT Adresse FROM Hospital WHERE ID = :id");
            query1.bindValue(":id", query.value(2).toInt());
            query1.exec();
            query1.next();
            listlatlong = query1.value(0).toString().split(",");
            addHospitalMarker(listlatlong[0], listlatlong[1]);
            ui->cnf->setVisible(true);
        } else{
            ui->status->setText("Status: Waiting for Instructions...");
            return;
        }

        emit setRoute(latLongList[0], latLongList[1], listlatlong[0], listlatlong[1]);

}

void dashboard_ambulance::on_pushButton_7_released() {
    if (admin) {
        auto *dialog = new userSelection(e, this);
        if (dialog->exec() == QDialog::Accepted) {
            admin = false;
            depart = dialog->getResult();
            ui->stackedWidget->setCurrentIndex(2);
            ui->pushButton_7->setText("Switch to Admin Interface");
            ui->dashboardBtn->hide();
            ui->CUDBtn->hide();
            ui->heart->move(ui->heart->pos().x(),
                            ui->heart->pos().y() - 100);
            ui->ChatBtn->move(ui->ChatBtn->pos().x(),
                              ui->ChatBtn->pos().y() - 100);
            ui->EmergBtn->move(ui->EmergBtn->pos().x(),
                               ui->EmergBtn->pos().y() - 100);
            ui->AboutBtn->move(ui->AboutBtn->pos().x(),
                               ui->AboutBtn->pos().y() - 100);

            loadmap();
        }
    } else {
        admin = true;
        ui->stackedWidget->setCurrentIndex(0);
        ui->pushButton_7->setText("Switch to User Interface");
        ui->dashboardBtn->show();
        ui->CUDBtn->show();
        ui->heart->move(ui->heart->pos().x(),
                        ui->heart->pos().y() + 100);
        ui->ChatBtn->move(ui->ChatBtn->pos().x(), ui->ChatBtn->pos().y() + 100);
        ui->EmergBtn->move(ui->EmergBtn->pos().x(),
                           ui->EmergBtn->pos().y() + 100);
        ui->AboutBtn->move(ui->AboutBtn->pos().x(),
                           ui->AboutBtn->pos().y() + 100);



    }
}

void dashboard_ambulance::on_pushButton_2_toggled(bool checked) {
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
void dashboard_ambulance::on_pushButton_2_released() {}

void dashboard_ambulance::toggleDropdownWidget() {
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
void dashboard_ambulance::on_pushButton_15_released() {
    toggleDropdownWidget();
}







void dashboard_ambulance::on_AboutBtn_released()
{
    if (ui->stackedWidget->currentIndex()!= 5)
        ui->stackedWidget->setCurrentIndex(5);
}

void dashboard_ambulance::on_pushButton_20_released()
{
        B.close_arduino();
        auto *dialog = new ModifyEmployee(&e, this);
        dialog->exec();
        ui->lineEdit_6->setText(e.getCIN());
        ui->lineEdit_3->setText(e.getPrenom());
        ui->lineEdit_4->setText(e.getNom());
        ui->lineEdit_5->setText(e.getMail());

}

void dashboard_ambulance::on_settingsBtn_released()
{
    if (ui->stackedWidget->currentIndex()!= 6)
        ui->stackedWidget->setCurrentIndex(6);
}

void dashboard_ambulance::on_pushButton_12_released()
{
    toggleDropdownWidget2();
}

void dashboard_ambulance::on_EmergBtn_released()
{
    if (admin) {
        QMessageBox::warning(nullptr, "Error",
                             "You need to be an User! Please switch!");
        return;
    }else{
        if (ui->stackedWidget->currentIndex()!= 3)
            ui->stackedWidget->setCurrentIndex(3);
    }


}

void dashboard_ambulance::on_cnf_released()
{

    QSqlQuery query0;
    query0.prepare("SELECT STATUS FROM EMERGENCYCALL WHERE matricule_ambulance = :mat AND STATUS != 6");
    query0.bindValue(":mat", depart);
    query0.exec();
    query0.next();
    if (query0.value(0).toInt() == 3){
    ui->status->setText("Status: Waiting for Instructions...");
    QSqlQuery query;
    query.prepare("UPDATE EMERGENCYCALL SET STATUS = 4 WHERE matricule_ambulance= :mat AND STATUS!=6");
    query.bindValue(":mat", depart);
    query.exec();
    query.prepare("SELECT Adresse FROM EMERGENCYCALL WHERE matricule_ambulance= :mat AND STATUS!=6");
    query.bindValue(":mat", depart);
    query.exec();
    query.next();
    QString str= query.value(0).toString();
    query.prepare("UPDATE AMBULANCE SET Emplacement = :adr WHERE matricule = :mat");
    query.bindValue(":mat",depart);
    query.bindValue(":adr", str);
    query.exec();
    ui->pushButton_34->setText(QString::number(ui->pushButton_34->text().toInt()-1));
            QMessageBox::information(nullptr, "Sent!",
                                 "Sent a request for next instruction!");
    }else if (query0.value(0).toInt() == 5){
        ui->status->setText("Status: Waiting for Instructions...");
        QSqlQuery query;

        query.prepare("SELECT HOSPITAL_ID FROM EMERGENCYCALL WHERE matricule_ambulance= :mat AND STATUS!=6");
        query.bindValue(":mat", depart);
        query.exec();
        query.next();
        QString str= query.value(0).toString();
        query.prepare("SELECT ADRESSE FROM HOSPITAL WHERE ID= :id");
        query.bindValue(":id", str);
        query.exec();
        query.next();
        str = query.value(0).toString();
        query.prepare("UPDATE EMERGENCYCALL SET STATUS = 6 WHERE matricule_ambulance= :mat AND STATUS!=6");
        query.bindValue(":mat", depart);
        query.exec();
        query.prepare("UPDATE AMBULANCE SET Emplacement = :adr , ETAT = 0 WHERE matricule = :mat");
        query.bindValue(":mat",depart);
        query.bindValue(":adr", str);
        query.exec();
        ui->pushButton_34->setText(QString::number(ui->pushButton_34->text().toInt()-1));
                QMessageBox::information(nullptr, "Sent!",
                                     "Thank you for your service!");
    }
    loadmap();
}

void dashboard_ambulance::on_stackedWidget_currentChanged(int arg1)
{
    if (arg1 == 4){
        B.close_arduino();
        int ret = B.connect_arduino(HEARTRATE_PORT);
        if (ret == 1){
            QMessageBox::warning(nullptr, "Error",
                                        "Error: Arduino not connected!");
            ui->stackedWidget->setCurrentIndex(1);
            return;
           }
        if(ret == 0){
            QTimer::singleShot(10000, [this]() {
                if (B.getarduino_port_name() == HEARTRATE_PORT){
                    B.write_to_arduino("e");
                    qDebug("Written e");
                }
            });
        }

    }else{
        QSqlQuery qtotal, qbroken, qunbroken;
        qreal tot = 0, broken = 0, unbroken = 0;

        qtotal.prepare("SELECT count(*) FROM AMBULANCE");
        qtotal.exec();
        qtotal.next();
        tot = qtotal.value(0).toInt();

        qbroken.prepare("SELECT count(*) FROM AMBULANCE WHERE etat='1'");
        qbroken.exec();
        qbroken.next();
        broken = qbroken.value(0).toInt();

        qunbroken.prepare("SELECT count(*) FROM AMBULANCE WHERE etat='0'");
        qunbroken.exec();
        qunbroken.next();
        unbroken = qunbroken.value(0).toInt();

        showArdStats("[AMB] T="+QString::number(tot)+ " Ub="+QString::number(unbroken)+ " B="+QString::number(broken)+" ");
    }
}

void dashboard_ambulance::on_heart_released()
{
    if (admin){
        QMessageBox::warning(nullptr, "Error",
                                    "Error: You need to be an user in order to use this!");
        return;
   }
    if (ui->stackedWidget->currentIndex() != 4)
        ui->stackedWidget->setCurrentIndex(4);
}

void dashboard_ambulance::on_stackedWidget_4_currentChanged(int arg1)
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
