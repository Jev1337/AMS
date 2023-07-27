#ifndef FACEID_H
#define FACEID_H

#include <QDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/videoio/videoio.hpp>

#include <QBuffer>
#include <QDebug>
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QMovie>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QUrlQuery>
#include <QList>

namespace Ui {
class faceid;
}

class faceid : public QDialog {
    Q_OBJECT

  public:
    explicit faceid(QWidget *parent = nullptr);
    ~faceid();
    QString getResult() const { return face_token; }
    bool isSad() const {return sadness >= 80;}

  private:
    Ui::faceid *ui;
    cv::Mat frame;
    QTimer *timer;
    int tries = 0;
    cv::VideoCapture *cap = nullptr;
    QString face_token = "";
    double sadness;
    QMovie *movie;

  private slots:
    void updateCamera();
    bool detectFace(cv::Mat image);
    void on_pushButton_released();
};

#endif // FACEID_H
