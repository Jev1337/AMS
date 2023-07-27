#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include "arduino.h"
#include <QDebug>
#include <QDialog>
#include <QJsonDocument>
#include <QMovie>
#include <QSerialPort>
#include <QTimer>
namespace Ui {
class fingerprint;
}

class fingerprint : public QDialog {
    Q_OBJECT

  public:
    explicit fingerprint(int id, QWidget *parent = nullptr);
    ~fingerprint();
  private slots:

    void checkFingerprint();

    void on_pushButton_released();

private:
    Ui::fingerprint *ui;
    QByteArray data;
    arduino A;
    QString buffer;
    QMovie *movie;
};

#endif // FINGERPRINT_H
