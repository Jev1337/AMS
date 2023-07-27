#include "faceid.h"
#include "ui_faceid.h"

faceid::faceid(QWidget *parent) : QDialog(parent), ui(new Ui::faceid) {
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    movie = new QMovie(":/assets/images/scan.gif");
    ui->label->setMovie(movie);
    movie->start();
    sadness = 0;

    cap = new cv::VideoCapture(0);

    // Check if camera is opened successfully
    if (!cap->isOpened()) {
        qDebug("Error: Could not open camera");
        return;
    }

    timer = new QTimer(this);
    /*
       connect(timer, &QTimer::timeout, [=](){
            updateCamera();
        });
    timer->start(33);  // 30ms delay between updates
    */
    connect(timer, &QTimer::timeout, [=]() {
        updateCamera();
        tries++;
        timer->stop();
        if (detectFace(frame)) {
            cap->release();
            movie->stop();
            ui->label->setMovie(nullptr); // Remove the movie from the QLabel
            delete movie; // Explicitly delete the dynamically allocated QMovie
            this->accept();
            this->close();
        }
        if (tries == 3) {
            timer->stop();
            cap->release();
            movie->stop();
            ui->label->setMovie(nullptr); // Remove the movie from the QLabel
            delete movie; // Explicitly delete the dynamically allocated QMovie

            QMessageBox::warning(
                nullptr, "Error",
                "No face detected! Please contact your administrator...");
            this->reject();
            this->close();
        }
    });
    timer->start(2000);
}

faceid::~faceid() { delete ui; }

void faceid::updateCamera() {
    // Retrieve a frame from the camera
    *cap >> frame;
    if (frame.empty()) {
        qDebug("Error: Could not capture frame");
        return;
    }
    // Convert the frame to a QImage for display
    /*
    QImage img = QImage((const unsigned char*)(frame.data), frame.cols,
    frame.rows, QImage::Format_RGB888).rgbSwapped();
    ui->label->setPixmap(QPixmap::fromImage(img));
    ui->label->setScaledContents(true);
    qApp->processEvents();
    */
}

bool faceid::detectFace(cv::Mat image) {
    // Convert the OpenCV image to a QImage

    cv::Mat temp; // make the same cv::Mat
    cvtColor(image, temp,
             cv::COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
    QImage qimage((uchar *)temp.data, temp.cols, temp.rows, temp.step,
                  QImage::Format_RGB888);
    qimage.bits(); // enforce deep copy, see documentation

    QByteArray imageBytes;
    QBuffer buffer(&imageBytes);
    buffer.open(QIODevice::WriteOnly);
    qimage.save(&buffer, "PNG");
    QString base64Image = QString(imageBytes.toBase64());

    /* // Call the Face API to detect the face
     QUrl
     url("https://ams-vision2a.cognitiveservices.azure.com/face/v1.0/detect");
     QUrlQuery query;
     query.addQueryItem("returnFaceId", "true"); // include faceId parameter
     url.setQuery(query.query());
     QNetworkRequest request(url);
     request.setHeader(QNetworkRequest::ContentTypeHeader,
     "application/octet-stream");
     request.setRawHeader("Ocp-Apim-Subscription-Key",
     "1f813e1a3e9e471098cfff9381b936e3");


     QNetworkAccessManager nam;
     QNetworkReply *reply = nam.post(request,
     QByteArray::fromBase64(base64Image.toUtf8()));

     while (!reply->isFinished()) {
         qApp->processEvents();
     }

     QByteArray responseData = reply->readAll();
     qDebug() << responseData;
    */

    QUrl url("https://api-us.faceplusplus.com/facepp/v3/detect");
    QHttpMultiPart *multiPart =
        new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader,
                        QVariant("image/png"));
    imagePart.setHeader(
        QNetworkRequest::ContentDispositionHeader,
        QVariant("form-data; name=\"image_file\"; filename=\"image.png\""));
    imagePart.setBody(imageBytes);

    QHttpPart apiKeyPart;
    apiKeyPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                         QVariant("form-data; name=\"api_key\""));
    apiKeyPart.setBody("oVAqEDbCYmaILayXJdKAsuYbFcJ0LBP6");

    QHttpPart apiSecretPart;
    apiSecretPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                            QVariant("form-data; name=\"api_secret\""));
    apiSecretPart.setBody("e76obC1xsr-zSMynWZoQCt62vWDgtZ6O");

    QHttpPart apiRetAtt;
    apiRetAtt.setHeader(QNetworkRequest::ContentDispositionHeader,
                            QVariant("form-data; name=\"return_attributes\""));
    apiRetAtt.setBody("emotion");
    /*
    QHttpPart returnAttributesPart;
    returnAttributesPart.setHeader(QNetworkRequest::ContentDispositionHeader,
    QVariant("form-data; name=\"return_attributes\""));
    returnAttributesPart.setBody("");
    */
    multiPart->append(imagePart);
    multiPart->append(apiKeyPart);
    multiPart->append(apiSecretPart);
    multiPart->append(apiRetAtt);

    QNetworkRequest request(url);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);

    // Wait for the HTTP request to finish and get the response
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    // Parse the response JSON and retrieve the face ID
    QByteArray response = (reply->readAll());
    QJsonDocument json_doc = QJsonDocument::fromJson(response);

    if (json_doc.isEmpty()) {
        qDebug() << "Error: Empty JSON response";
        timer->start(2000);
        return false;
    }

    QJsonObject json_obj = json_doc.object();
    QJsonArray faces_arr = json_obj.value("faces").toArray();

    if (faces_arr.isEmpty()) {
        qDebug() << "Error: No faces detected";
        timer->start(2000);
        return false;
    }

    QJsonObject face_obj = faces_arr.first().toObject();
    face_token = face_obj.value("face_token").toString();
    QJsonObject attributes = face_obj.value("attributes").toObject();
    QJsonObject emotions = attributes.value("emotion").toObject();
    sadness = emotions.value("sadness").toDouble();
    qDebug() << "Face Token: " << face_token;
    return true;
}

void faceid::on_pushButton_released() {
    timer->stop();
    timer->deleteLater();
    cap->release();
    this->close();
    QDialog::reject();
}
