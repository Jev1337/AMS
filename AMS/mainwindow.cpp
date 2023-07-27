#include "mainwindow.h"
#include "dashboard_ambulance.h"
#include "dashboard_comms.h"
#include "dashboard_employee.h"
#include "dashboard_hospital.h"
#include "dashboard_patient.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : CFramelessWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->registerUI->hide();
    setTitleBar(ui->widget);
    ui->regPasswordConf->hide();
    login = true;
    buffer = "";

    int ret = A.connect_arduino(FINGERPRINT_PORT); // lancer la connexion à arduino
    switch (ret) {
    case (0):
        qDebug() << "arduino is available and connected to : "
                 << A.getarduino_port_name();
        break;
    case (1):
        qDebug() << "given arduino is not available";
    }

    QObject::connect(A.getserial(), SIGNAL(readyRead()), this,
                     SLOT(checkFingerprint()));
    if (ret == 0)
        QTimer::singleShot(3000, [this]() {
            A.write_to_arduino("f0");
            qDebug("Written f");
        });
}

void MainWindow::checkFingerprint() {
    data = A.read_from_arduino();
    buffer += QString::fromLatin1(data);
    if (buffer.endsWith('\n')) {
        buffer.chop(2);
        // We received a complete line, parse it
        QJsonDocument doc = QJsonDocument::fromJson(buffer.toUtf8());
        if (doc.isNull()) {
            qWarning() << "Failed to parse JSON:" << buffer;
            A.write_to_arduino("0");
        } else {
            QJsonObject obj = doc.object();
            QString module = obj.value("Module").toString();
            QString id = obj.value("Id").toString();
            QString status = obj.value("Status").toString();
            qDebug() << "Module:" << module << "Id:" << id
                     << "Status:" << status;
            if (module == "fingerprint" && status != "OK"){
                A.write_to_arduino("0");
            }
            if (module == "fingerprint" && status == "OK") {
                employee e(ui->loginUsername->text(), id, 3);
                if (e.getType() == -1) {
                    QMessageBox::warning(this, "Error",
                                         "Sorry! Your password is incorrect!");
                    A.write_to_arduino("0");
                }
                if (e.getType() == -2) {
                    QMessageBox::warning(this, "Error",
                                         "Sorry! Your account does not exist!");
                    A.write_to_arduino("0");
                }
                if (e.getType() != -1 && e.getType() != -2) {
                    A.close_arduino();
                    if (e.getType() == 1) {
                        auto *w1 = new dashboard(e);
                        w1->show();
                        this->close();
                    }
                    if (e.getType() == 2) {
                        auto *w2 = new dashboard_comms(e);
                        w2->show();
                        this->close();
                    }
                    if (e.getType() == 3) {
                        auto *w3 = new dashboard_hospital(e);
                        w3->show();
                        this->close();
                    }
                    if (e.getType() == 4) {
                        auto *w4 = new dashboard_patient(e);
                        w4->show();
                        this->close();
                    }
                    if (e.getType() == 5) {
                        auto *w5 = new dashboard_ambulance(e);
                        w5->show();
                        this->close();
                    }



                }
            }
        }
        buffer.clear();
    }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_PromptInUpButton_clicked() {
    QGraphicsOpacityEffect *effect1 = new QGraphicsOpacityEffect;
    QGraphicsOpacityEffect *effect2 = new QGraphicsOpacityEffect;
    QGraphicsOpacityEffect *effect3 = new QGraphicsOpacityEffect;
    QGraphicsOpacityEffect *effect4 = new QGraphicsOpacityEffect;
    QGraphicsOpacityEffect *effect5 = new QGraphicsOpacityEffect;
    ui->Gretting->setGraphicsEffect(effect1);         // label
    ui->Description->setGraphicsEffect(effect2);      // label
    ui->SignInUp->setGraphicsEffect(effect3);         // label
    ui->PromptInUpButton->setGraphicsEffect(effect4); // button
    ui->regPasswordConf->setGraphicsEffect(effect5);  // textedit
    QPoint startValue, endValue;
    int startOpacity = 1, endOpacity = 0, duration = 1000;
    if (ui->InfoContainer->pos().x() == 380) {
        login = false;
        startValue = ui->InfoContainer->pos();
        endValue = QPoint(0, 20);
        updatePos(ui->InfoContainer, startValue, endValue, duration);
        startValue = ui->LoginRegisterContainer->pos();
        endValue = QPoint(380, 20);
        updatePos(ui->LoginRegisterContainer, startValue, endValue, duration);
        updateText(effect1, ui->Gretting, "Welcome Back!", startOpacity,
                   endOpacity, duration);
        updateText(
            effect2, ui->Description,
            "To keep connected with us please\nlogin with your personal info",
            startOpacity, endOpacity, duration);
        updateText(effect3, ui->SignInUp, "Create an Account", startOpacity,
                   endOpacity, 500);
        updateText(effect4, ui->PromptInUpButton, "Sign In", startOpacity,
                   endOpacity, duration);
        updateText(effect5, ui->regPasswordConf, "", startOpacity, endOpacity,
                   500);

        ui->SignInUpButton->setText("Next");
        ui->regPasswordConf->show();
        ui->ForgotPassword->hide();
        ui->Face_ID_Btn->hide();

    } else {
        login = true;
        startValue = ui->InfoContainer->pos();
        endValue = QPoint(380, 20);
        updatePos(ui->InfoContainer, startValue, endValue, duration);
        startValue = ui->LoginRegisterContainer->pos();
        endValue = QPoint(0, 20);
        updatePos(ui->LoginRegisterContainer, startValue, endValue, duration);
        updateText(effect1, ui->Gretting, "Hello, Friend!", startOpacity,
                   endOpacity, duration);
        updateText(
            effect2, ui->Description,
            "Enter your personal details and start\nyour journey with us",
            startOpacity, endOpacity, duration);
        updateText(effect3, ui->SignInUp, "Sign In", startOpacity, endOpacity,
                   500);
        updateText(effect4, ui->PromptInUpButton, "Sign Up", startOpacity,
                   endOpacity, duration);
        updateText(effect5, ui->regPasswordConf, "", startOpacity, endOpacity,
                   500);

        ui->SignInUpButton->setText("Sign In");
        ui->regPasswordConf->hide();
        ui->ForgotPassword->show();
        ui->Face_ID_Btn->show();
    }
}

void MainWindow::on_pushButton_released() { QApplication::quit(); }

void MainWindow::on_SignInUpButton_clicked() {
    if (!EmailValidator(ui->loginUsername->text())) {
        QMessageBox::warning(this, "Error",
                             "Sorry! You must write a correct email!");
        return;
    }
    if (!login) {

        if (ui->loginPassword->text() != ui->regPasswordConf->text()) {
            QMessageBox::warning(this, "Error",
                                 "Sorry! Both passwords must be the same!");
            return;
        }
        if (!PasswordValidator(ui->loginPassword->text())) {
            QMessageBox::warning(this, "Error",
                                 "Sorry! You must write a correct password! (8 "
                                 "Characters minimum with atleast one number)");
            return;
        }
        employee e;
        if (e.emailExistance(ui->loginUsername->text())) {
            QMessageBox::warning(this, "Error",
                                 "Sorry! This account already exists!");
            return;
        }

        ui->New_Type_2->clear();
        ui->New_Type_2->setCurrentIndex(-1);
        qDebug() << e.getAccountTypes(ui->New_Type_2);

        ui->registerUI->setCurrentWidget(ui->selection);
        ui->registerUI->show();
        QPropertyAnimation *animation =
            new QPropertyAnimation(ui->registerUI, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(0, 20, 0, 470));
        animation->setEndValue(QRect(0, 20, 770, 470));
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->start();

        QObject::connect(animation, &QPropertyAnimation::finished, [=]() {
            ui->InfoContainer->hide();
            ui->LoginRegisterContainer->hide();
        });

    } else {
        employee e(ui->loginUsername->text(), ui->loginPassword->text(), 1);
        if (e.getType() == -1) {
            QMessageBox::warning(this, "Error",
                                 "Sorry! Your password is incorrect!");
        }
        if (e.getType() == -2) {
            QMessageBox::warning(this, "Error",
                                 "Sorry! Your account does not exist!");
        }
        if (e.getType() != -1 && e.getType() != -2) {
            A.close_arduino();
            if (e.getType() == 1) {
                auto *w1 = new dashboard(e);
                w1->show();
                this->close();
            }
            if (e.getType() == 2) {
                auto *w2 = new dashboard_comms(e);
                w2->show();
                this->close();
            }
            if (e.getType() == 3) {
                auto *w3 = new dashboard_hospital(e);
                w3->show();
                this->close();
            }
            if (e.getType() == 4) {
                auto *w4 = new dashboard_patient(e);
                w4->show();
                this->close();
            }
            if (e.getType() == 5) {
                auto *w5 = new dashboard_ambulance(e);
                w5->show();
                this->close();
            }
        }
    }
}

void MainWindow::on_Create_released() {
    if (!CINValidator(ui->New_CIN_2->text())) {
        QMessageBox::warning(this, "Error", "CIN Inserted is Invalid!");
        return;
    }
    if (!CINValidator(ui->phone->text())) {
        QMessageBox::warning(this, "Error", "Phone Inserted is Invalid!");
        return;
    }
    if (!NameValidator(ui->New_Name_2->text())) {
        QMessageBox::warning(
            this, "Error",
            "Your name must be 3 to 20 letters long and only alphabetic.");
        return;
    }
    if (!NameValidator(ui->New_LastName_2->text())) {
        QMessageBox::warning(
            this, "Error",
            "Your last name must be 3 to 20 letters long and only alphabetic.");
        return;
    }
    employee tst;
    if (tst.cinExistance(ui->New_CIN_2->text())) {
        QMessageBox::warning(
            this, "Error",
            "An account with the same CIN is already registered!");
        return;
    }


    A.close_arduino();
    int admin;
    if (ui->checkBox_4->isChecked())
        admin = 0;
    else
        admin = 1;
    employee e(ui->New_CIN_2->text(), ui->New_Type_2->currentIndex(),
               ui->loginUsername->text(), ui->New_Name_2->text(),
               ui->New_LastName_2->text(),
               ui->New_DOB_2->date().toString("yyyy-MM-dd"), "", ui->phone->text(), admin);
    e.addSelf(ui->loginPassword->text());
    if (ui->checkBox_2->isChecked()) {
        auto *dialog = new faceid();
        dialog->setModal(false);

        if (dialog->exec() == QDialog::Accepted) {

            e.setFaceID(dialog->getResult());
        } else {
            QMessageBox::warning(
                this, "Error",
                "Either face has not been detected or something interrupted "
                "Face ID. Please click ok to continue...");
        }
    }
    if (ui->checkBox_3->isChecked()) {

        if (e.GetLastFingerprintID() != 127) {
            auto *dialog = new fingerprint(e.GetLastFingerprintID() + 1);
            if (dialog->exec() == QDialog::Accepted) {
                e.setFingerprint(e.GetLastFingerprintID() + 1);
            } else {
                QMessageBox::warning(this, "Error", "Error");
            }
        } else
            QMessageBox::warning(this, "Error", "Reached Fingerprint Limit");
    }

    // Create a list of fake messages to display in the label
    QStringList fakeMessages;
    fakeMessages << "Initializing connection with database..."
                 << "Encrypting data..."
                 << "Creating user account..."
                 << "Verifying email address..."
                 << "Sending confirmation email..."
                 << "Generating access token..."
                 << "Setting up user profile..."
                 << "Configuring preferences..."
                 << "Loading user dashboard..."
                 << "Account creation complete!";

    // Initialize the progress bar and label
    ui->progressBar->setValue(0);
    ui->label->setText(fakeMessages[0]);

    // Create a timer with a random interval to simulate progress updates
    QTimer* timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, [=]() {
        // Update the progress bar
        int value = ui->progressBar->value();
        if (value < ui->progressBar->maximum()) {
            value += qrand() % 7 + 3; // Increase the value by a random amount between 3 and 7
            ui->progressBar->setValue(value);
        } else {
            timer->stop(); // Stop the timer when the progress bar is full
            delete timer;
        }

        // Update the label with a random fake message
        int index = qrand() % fakeMessages.size();
        ui->label->setText(fakeMessages[index]);
    });

    // Start the timer with a random interval between 250ms and 1000ms
    int interval = qrand() % 351 + 150;
    timer->start(interval);

    // Run the progress updates for 4 seconds
    QTimer::singleShot(4000, this, [=]() {
        ui->progressBar->setValue(ui->progressBar->maximum());
        ui->label->setText("Account creation complete!");
        delete timer;
    });
    ui->registerUI->setCurrentIndex(1);
}

void MainWindow::on_Face_ID_Btn_released() {

    if (EmailValidator(ui->loginUsername->text())) {
        employee e;
        if (!e.emailExistance(ui->loginUsername->text())){
            QMessageBox::warning(
                this, "Error",
                "Error 0x04: Your email does not exist!");
            return;
           }
        auto *dialog = new faceid();
        dialog->setModal(false);

        if (dialog->exec() == QDialog::Accepted) {

            QString storedFaceID =
                e.GetCurrentFaceID(ui->loginUsername->text());


            if (storedFaceID != "") {
                QTimer *timer = new QTimer(this);
                QUrl url("https://api-us.faceplusplus.com/facepp/v3/compare");
                QHttpMultiPart *multiPart =
                    new QHttpMultiPart(QHttpMultiPart::FormDataType);

                QHttpPart imagePart1;
                imagePart1.setHeader(QNetworkRequest::ContentTypeHeader,
                                     QVariant("text/plain"));
                imagePart1.setHeader(
                    QNetworkRequest::ContentDispositionHeader,
                    QVariant("form-data; name=\"face_token1\""));
                imagePart1.setBody(storedFaceID.toUtf8());

                QHttpPart imagePart2;
                imagePart2.setHeader(QNetworkRequest::ContentTypeHeader,
                                     QVariant("text/plain"));
                imagePart2.setHeader(
                    QNetworkRequest::ContentDispositionHeader,
                    QVariant("form-data; name=\"face_token2\""));
                imagePart2.setBody(dialog->getResult().toUtf8());

                QHttpPart apiKeyPart;
                apiKeyPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                                     QVariant("form-data; name=\"api_key\""));
                apiKeyPart.setBody("oVAqEDbCYmaILayXJdKAsuYbFcJ0LBP6");

                QHttpPart apiSecretPart;
                apiSecretPart.setHeader(
                    QNetworkRequest::ContentDispositionHeader,
                    QVariant("form-data; name=\"api_secret\""));
                apiSecretPart.setBody("e76obC1xsr-zSMynWZoQCt62vWDgtZ6O");

                multiPart->append(imagePart1);
                multiPart->append(imagePart2);
                multiPart->append(apiKeyPart);
                multiPart->append(apiSecretPart);

                QNetworkRequest request(url);
                QNetworkAccessManager *manager =
                    new QNetworkAccessManager(this);
                QNetworkReply *reply = manager->post(request, multiPart);
                multiPart->setParent(reply);

                // Wait for the HTTP request to finish and get the response
                QEventLoop loop;
                connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
                loop.exec();

                // Parse the response JSON and retrieve the confidence value
                QByteArray response = (reply->readAll());
                QJsonDocument json_doc = QJsonDocument::fromJson(response);

                if (json_doc.isEmpty()) {
                    qDebug() << "Error: Empty JSON response";
                    timer->start(2000);
                    return;
                }

                QJsonObject json_obj = json_doc.object();
                double confidence = json_obj.value("confidence").toDouble();
                qDebug() << "Face Comparison Confidence: " << confidence;

                // Check if the confidence value is greater than a threshold
                double threshold =
                    80.0; // You can adjust this threshold as per your needs
                if (confidence > threshold) {
                    if (dialog->isSad())
                        QMessageBox::information(
                            this, "Hey there!",
                            "Hey! FaceID worked just fine. However we have a feeling that you are sad! Please make sure to take pauses during heavy work! Cheer up and good luck with your day! ;)");
                    // Face match found
                    qDebug() << "Face match found!";
                    employee e(ui->loginUsername->text(), dialog->getResult(),
                               2);
                    if (e.getType() != -1 && e.getType() != -2) {
                        A.close_arduino();
                        if (e.getType() == 1) {
                            auto *w1 = new dashboard(e);
                            w1->show();
                            this->close();
                        }
                        if (e.getType() == 2) {
                            auto *w2 = new dashboard_comms(e);
                            w2->show();
                            this->close();
                        }
                        if (e.getType() == 3) {
                            auto *w3 = new dashboard_hospital(e);
                            w3->show();
                            this->close();
                        }
                        if (e.getType() == 4) {
                            auto *w4 = new dashboard_patient(e);
                            w4->show();
                            this->close();
                        }
                        if (e.getType() == 5) {
                            auto *w5 = new dashboard_ambulance(e);
                            w5->show();
                            this->close();
                        }
                    }
                } else {
                    QMessageBox::warning(this, "Error",
                                         "Error 0x02: FaceID Incorrect!");
                }
            } else
                QMessageBox::warning(
                    this, "Error",
                    "Error 0x01: Your FaceID may not be setup or has expired!");
        }
    } else
        QMessageBox::warning(this, "Error",
                             "Error 0x03: Please enter your email first!");
}

void MainWindow::on_checkBox_3_toggled(bool checked)
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



void MainWindow::on_progressBar_valueChanged(int value)
{
    if (value == 100){
        slotReboot();
    }
}

void MainWindow::on_checkBox_4_toggled(bool checked)
{
    if (checked && ui->New_Type_2->currentIndex() != 1
            && ui->New_Type_2->currentIndex() != 4){
        QMessageBox::warning(this, "Error", "Incorrect Department: No User mode for this type");
        ui->checkBox_4->setChecked(false);
    }
}

void MainWindow::on_New_Type_2_currentIndexChanged(int index)
{
    if (ui->checkBox_4->isChecked() && index != 1 && index != 4){
        QMessageBox::warning(this, "Error", "Incorrect Department: No User mode for this type");
        ui->checkBox_4->setChecked(false);
    }

}

void MainWindow::slotReboot(){
    qDebug() << "Performing application reboot...";
    qApp->exit( EXIT_CODE_REBOOT );
}

void MainWindow::on_ForgotPassword_released()
{
    if (!EmailValidator(ui->loginUsername->text())) {
        QMessageBox::warning(this, "Error",
                             "Sorry! You must write a correct email!");
        return;
    }
    employee echange;
    echange.setMail(ui->loginUsername->text());
    if (!echange.emailExistance(echange.getMail())){
        QMessageBox::information(this, "Sent",
                             "If your account exists in our database, we sent you an email!");
        return;
    }


    SimpleMail::Sender sender("mail.ams-esprit.tn", 465, SimpleMail::Sender::SslConnection);

    sender.setUser("no-reply@ams-esprit.tn");
    sender.setPassword("ANU#^gP7m)oq");

    SimpleMail::MimeMessage message;

    message.setSender(SimpleMail::EmailAddress("no-reply@ams-esprit.tn", "Ambulance Management System"));
    message.addTo(SimpleMail::EmailAddress(QString("<%1>").arg(echange.getMail())));
    message.setSubject("Your new password is here!");

    auto text = new SimpleMail::MimeHtml;

    QString currentPath = QCoreApplication::applicationDirPath();
    QFile file(
        currentPath +
        "../../../AMS/libraries/docs/email.html");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QMessageBox::critical(this, tr("Error"),
                              tr("Corruption Detected: Could not open HTML template file."));
        return;
    }

    QTextStream stream(&file);

    QString htmlText = stream.readAll();
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
       const int randomStringLength = 12; // assuming you want random strings of 12 characters

       QString password;
       for(int i=0; i<randomStringLength; ++i)
       {
           int index = qrand() % possibleCharacters.length();
           QChar nextChar = possibleCharacters.at(index);
           password.append(nextChar);
       }
    text->setHtml(htmlText.arg(QDate::currentDate().toString(Qt::ISODate)).arg(password).toUtf8());
    message.addPart(text);

    if (sender.sendMail(message)){ // Blocks untill mail is delivered or errored
        echange.setCIN(echange.getCINfromEMAIL(ui->loginUsername->text()));
        echange.changePw(password);
        echange.setPhone(echange.getPHONEfromEMAIL(echange.getMail()));
        sendSms(echange.getPhone());
        QMessageBox::information(this, "Sent",
                             "If your account exists in our database, we sent you an email!");
    }
    else
        QMessageBox::warning(this, "Error",
                             "Error while sending an email! Please contact us!");
}

void MainWindow::on_loginPassword_returnPressed()
{
    on_SignInUpButton_clicked();
}

void MainWindow::on_loginUsername_returnPressed()
{
    on_SignInUpButton_clicked();
}

void MainWindow::on_regPasswordConf_returnPressed()
{
    on_SignInUpButton_clicked();
}
