#ifndef DASHBOARD_AMBULANCE_H
#define DASHBOARD_AMBULANCE_H

#include "ambulance.h"
#include "employee.h"
#include "libraries/framelesswindow.h"
#include "mainwindow.h"
#include "modifyambulance.h"
#include "userselection.h"
#include "modifyemployee.h"
#include <QAudioInput>
#include <QAudioRecorder>
#include <QDesktopServices>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMovie>
#include <QPrinter>
#include <QSqlTableModel>
#include <QTextToSpeech>
#include <QTimer>
#include <QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QLineSeries>
#include <QtCore/QDebug>
#include <QtCore/QTime>

#include "hospital.h"
#include "selectmap.h"

QT_CHARTS_USE_NAMESPACE

#include "arduino.h"
#include <QSerialPort>

#include <QLCDNumber>
#include <qlcdnumber.h>

namespace Ui {
class dashboard_ambulance;
}

class dashboard_ambulance : public CFramelessWindow {
    Q_OBJECT

  public:
    explicit dashboard_ambulance(employee &e, QWidget *parent = nullptr);
    ~dashboard_ambulance();
    void sendOpenAIRequest();
    void refreshList();
signals:
     void setCenter(QVariant, QVariant);
     void addMarker(QVariant, QVariant);
     void addHospitalMarker(QVariant, QVariant);
     void addPatientMarker(QVariant, QVariant, QVariant);
     void addAmbulanceMarker(QVariant, QVariant);
     void setRoute(QVariant, QVariant, QVariant, QVariant);

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

    void on_pushButton_33_released();

    void on_pushButton_48_released();

    void on_New_Btn_released();

    void on_pushButton_3_released();

    void on_pushButton_4_released();

    void toggleDropdownWidget();

    void on_pushButton_15_released();

    void on_pushButton_7_released();

    void on_pushButton_6_released();

    void on_pushButton_9_released();

    void on_pushButton_11_released();

    //  void on_pushButton_49_pressed();

    // void on_pushButton_32_released();

    void on_pushButton_2_released();

    void on_pushButton_2_toggled(bool checked);

    void on_AboutBtn_released();

    void on_pushButton_20_released();

    void on_settingsBtn_released();

    void changeToMain(int);

    void toggleDropdownWidget2();

    void on_pushButton_12_released();

    void on_EmergBtn_released();

    void loadmap();

    void on_cnf_released();

    void onDialogClosed(const QString& text);

    void showArdStats(QString);

    void on_stackedWidget_currentChanged(int arg1);

    void checkHeart();
    void on_heart_released();

    void on_stackedWidget_4_currentChanged(int arg1);

private:
    Ui::dashboard_ambulance *ui;
    employee e;
    bool admin;
    int depart;
    QAudioRecorder *audioRecorder;
    QTextToSpeech *tts;
    QChart ch;
    ambulance *a;
    arduino B;
    QByteArray data ;
    QString buffer;
    int key;
    int lastkey;
  protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // DASHBOARD_AMBULANCE_H
