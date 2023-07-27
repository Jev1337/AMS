#include "fingerprint.h"
#include "ui_fingerprint.h"

fingerprint::fingerprint(int id, QWidget *parent)
    : QDialog(parent), ui(new Ui::fingerprint) {
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    movie = new QMovie(":/assets/images/fingerprint.gif");
    ui->label->setMovie(movie);
    movie->start();
    int ret = A.connect_arduino(); // lancer la connexion à arduino
    switch (ret) {
    case (0):
        qDebug() << "arduino is available and connected to : "
                 << A.getarduino_port_name();
        break;
    case (1):
        qDebug() << "arduino is available but not connected to :"
                 << A.getarduino_port_name();
        break;
    case (-1):
        qDebug() << "arduino is not available";
    }
    QString idF = QString::number(id);
    ui->label_4->setText("Please wait for Arduino to initialize!");
    if (ret == 0)
        QTimer::singleShot(3000, [this, idF]() {
            A.write_to_arduino("f" + idF.toUtf8());
            ui->label_4->setText("Please put your finger...");

        });
    QObject::connect(A.getserial(), SIGNAL(readyRead()), this,
                     SLOT(checkFingerprint()));
}

void fingerprint::checkFingerprint() {
    data = A.read_from_arduino();
    buffer += QString::fromLatin1(data);
    if (buffer.endsWith('\n')) {
        buffer.chop(2);
        // We received a complete line, parse it
        QJsonDocument doc = QJsonDocument::fromJson(buffer.toUtf8());
        if (doc.isNull()) {
            qWarning() << "Failed to parse JSON:" << buffer;
        } else {
            QJsonObject obj = doc.object();
            QString module = obj.value("Module").toString();
            QString id = obj.value("Id").toString();
            QString status = obj.value("Status").toString();
            qDebug() << "Module:" << module << "Id:" << id
                     << "Status:" << status;
            if (module == "fingerprint" && status == "REMOVE_FINGER") {
                ui->label_4->setText("Please remove your finger...");
            }
            if (module == "fingerprint" && status == "PUT_FINGER_AGAIN") {
                ui->label_4->setText("Please put your finger again...");
            }
            if (module == "fingerprint" && status == "OK") {
                A.close_arduino();
                movie->stop();
                ui->label->setMovie(nullptr); // Remove the movie from the QLabel
                delete movie; // Explicitly delete the dynamically allocated QMovie
                this->accept();
                this->close();

            }
            if (module == "fingerprint" &&
                (status == "UNK" || status == "ERR" || status == "INC")) {
                A.close_arduino();
                movie->stop();
                ui->label->setMovie(nullptr);
                delete movie;
                this->reject();
                this->close();
            }
        }
        buffer.clear();
    }
}

fingerprint::~fingerprint() { delete ui; }

void fingerprint::on_pushButton_released()
{
    A.close_arduino();
    movie->stop();
    ui->label->setMovie(nullptr); // Remove the movie from the QLabel
    delete movie; // Explicitly delete the dynamically allocated QMovie
    this->reject();
}
