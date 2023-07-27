#ifndef DASHBOARD_HOSPITAL_H
#define DASHBOARD_HOSPITAL_H

#include "employee.h"
#include "hospital.h"
#include "libraries/framelesswindow.h"
#include "mainwindow.h"
#include "userselection.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMovie>
#include <QQuickItem>
#include <QTextToSpeech>
#include <QTimer>
#include <QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QAudioRecorder>
#include <QGeoCoordinate>
#include "modifyemployee.h"

namespace Ui {
class dashboard_hospital;
}

class dashboard_hospital : public CFramelessWindow {
    Q_OBJECT

  public:
    explicit dashboard_hospital(employee &e, QWidget *parent = nullptr);
    ~dashboard_hospital();
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

    void on_pushButton_3_released();

    void on_pushButton_48_released();

    void on_pushButton_33_released();

    void onTableHospitalClicked(const QModelIndex &);

    void on_pushButton_4_released();

    void on_Modifier_Cancel_released();

    void on_Modifier_btn_released();

    void on_pushButton_6_released();

    void on_pushButton_5_released();

    void toggleDropdownWidget();

    void toggleNotificationWidget();

    void on_pushButton_15_released();

    void on_pushButton_9_released();

    void assignAmbulanceToEmergency(int);

    void assignAmbulanceToHospital(int);

    void on_LiveMapBtn_released();

    void on_pushButton_12_released();

    void FollowEmergency(QGeoCoordinate, QGeoCoordinate);

    void on_pushButton_11_released();

    void onDialogClosed(const QString& text);

    void on_AboutBtn_released();

    void on_settingsBtn_released();

    void on_pushButton_11_clicked();

    void on_pushButton_16_toggled(bool checked);

    void on_stackedWidget_currentChanged(int arg1);

    void changeToMain(int);

    void on_pushButton_12_clicked();

    void on_pushButton_2_released();

    void on_tableView_6_doubleClicked(const QModelIndex &index);

    void showArdStats(QString message);

    void on_pushButton_20_released();

signals:
    void setCenter(QVariant, QVariant);
    void addMarker(QVariant, QVariant);
    void addHospitalMarker(QVariant, QVariant);
    void addPatientMarker(QVariant, QVariant, QVariant);
    void addAmbulanceMarker(QVariant, QVariant);
    void addPatientOccupiedMarker(QVariant, QVariant);
    void addAmbulanceOccupiedMarker(QVariant, QVariant);
    void setRoute(QVariant, QVariant, QVariant, QVariant);

  private:
    Ui::dashboard_hospital *ui;
    employee e;
    Hospital Htmp;
    int selected_hospital = -1;
    bool admin;
    int depart;
    double latit;
    double longit;
    arduino A;
    QAudioRecorder *audioRecorder;


  public:
    void refresh();

  protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // DASHBOARD_HOSPITAL_H
