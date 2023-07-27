#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "animate.h"
#include "arduino.h"
#include "connsql.h"
#include "faceid.h"
#include "fingerprint.h"
#include "libraries/framelesswindow.h"
#include <iostream>
#include <random>
#include <QCryptographicHash>
#include <SimpleMail>
#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include "sms.h"



QString const FINGERPRINT_PORT = "COM3";
QString const HEARTRATE_PORT = "COM4";
int const EXIT_CODE_REBOOT = -123456789;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public CFramelessWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool login;




  private slots:

    void on_PromptInUpButton_clicked();

    void on_pushButton_released();

    void on_SignInUpButton_clicked();

    void on_Create_released();

    void on_Face_ID_Btn_released();

    void checkFingerprint();

    void on_checkBox_3_toggled(bool checked);

    void on_progressBar_valueChanged(int value);

    void on_checkBox_4_toggled(bool checked);

    void on_New_Type_2_currentIndexChanged(int index);

    void slotReboot();

    void on_ForgotPassword_released();

    void on_loginPassword_returnPressed();

    void on_loginUsername_returnPressed();

    void on_regPasswordConf_returnPressed();

private:
    Ui::MainWindow *ui;
    QByteArray data;
    arduino A;
    QString buffer;
};
#endif // MAINWINDOW_H
