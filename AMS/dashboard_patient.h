#ifndef DASHBOARD_PATIENT_H
#define DASHBOARD_PATIENT_H

#include <QMainWindow>
#include "mainwindow.h"
#include <QTimer>
#include "libraries/framelesswindow.h"
#include <QMovie>
#include <QTextToSpeech>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QAudioRecorder>
#include "employee.h"
#include "patient.h"
#include "modifyemployee.h"

#include "arduino.h"
#include <QSerialPort>
#include <qlcdnumber.h>
namespace Ui {
class dashboard_patient;
}

class dashboard_patient : public CFramelessWindow
{
    Q_OBJECT

public:
    explicit dashboard_patient(employee &e, QWidget *parent = nullptr);
    ~dashboard_patient();
    void sendOpenAIRequest();

private slots:
    void on_pushButton_released();

    void on_pushButton_27_released();

    void on_pushButton_28_released();

    void on_pushButton_29_released();

    void on_pushButton_30_released();

    void on_pushButton_36_released();

    void on_pushButton_38_released();

    void on_pushButton_39_released();

    void on_pushButton_120_released();

    void on_pushButton_121_released();

    void on_pushButton_123_released();

    void on_pushButton_124_released();

    void on_pushButton_125_released();

    void on_pushButton_126_released();

    void on_pushButton_31_released();

    void on_logOut_released();

    void on_dashboardBtn_released();

    void on_CUDBtn_released();

    void on_ChatBtn_released();

    void on_sendChatBtn_released();

    void on_New_Btn_released();

    void on_pushButton_33_released();

    void on_pushButton_48_released();

    void onTableHospitalClicked(const QModelIndex &);

    void on_pushButton_3_released();

    void on_pushButton_4_released();

    void on_Modifier_Cancel_released();

    void on_Modifier_btn_released();

    void on_pushButton_6_released();

    void on_Modifier_btn_3_released();

    void on_pushButton_10_released();

    void on_pushButton_14_released();

    void on_pushButton_16_released();

    void on_pushButton_17_released();

    void on_pushButton_11_released();

    void on_EmergBtn_released();

    void checkSeriel();

    void on_AboutBtn_released();

    void on_settingsBtn_released();

    void on_pushButton_20_released();

    void on_pushButton_18_released();

    void on_pushButton_18_toggled(bool checked);

    void on_stackedWidget_currentChanged(int arg1);

    void changeToMain(int);

    void on_pushButton_5_released();

    void showArdStats(QString message);

    void on_pushButton_19_released();

    void on_pushButton_21_released();

    void on_pushButton_15_released();

private:
    Ui::dashboard_patient *ui;
    employee e;
    Patient PTMP;
    int selected_patient = -1;
    QString symptoms;
    arduino A;
    QAudioRecorder* audioRecorder;
    QByteArray data;
    QString buffer;
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif // DASHBOARD_PATIENT_H
