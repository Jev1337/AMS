#ifndef DASHBOARD_COMMS_H
#define DASHBOARD_COMMS_H

#include "communication.h"
#include "employee.h"
#include "libraries/framelesswindow.h"
#include "mainwindow.h"
#include "modifycomms.h"
#include "selectmap.h"
#include "userselection.h"
#include "modifyemployee.h"
#include <QAudioRecorder>
#include <QFileDialog>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMovie>
#include <QPrinter>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QTextToSpeech>
#include <QTimer>
#include <QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

namespace Ui {
class dashboard_comms;
}

class dashboard_comms : public CFramelessWindow {
    Q_OBJECT

  public:
    explicit dashboard_comms(employee &e, QWidget *parent = nullptr);
    ~dashboard_comms();
    void sendOpenAIRequest();
    void refreshList();
  signals:
    void setCenter(QVariant, QVariant);
    void addMarker(QVariant, QVariant);
  private slots:
    void onCoordinateSelected(double lat, double lng);

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

    void on_EmergBtn_released();

    void on_stackedWidget_currentChanged(int arg1);

    void on_pushButton_2_released();

    void on_pushButton_7_released();

    void toggleDropdownWidget();

    void on_pushButton_15_released();

    void on_pushButton_6_released();

    void on_pushButton_9_released();

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_pushButton_17_released();

    void on_VoiceBtn_released();

    void on_pushButton_11_released();

    void on_pushButton_14_released();

    void on_pushButton_18_released();

    void on_pushButton_10_released();

    void on_pushButton_10_toggled(bool checked);

    void on_pushButton_19_released();

    void on_pushButton_16_toggled(bool checked);

    void on_pushButton_61_released();

    void on_AboutBtn_released();

    void on_settingsBtn_released();

    void changeToMain(int);

    void showArdStats(QString message);

    void refresh();

    void on_stackedWidget_4_currentChanged(int arg1);

private:
    Ui::dashboard_comms *ui;
    double latit;
    double longit;
    employee e;
    bool admin;
    int depart;
    QMediaPlayer *player;
    QAudioRecorder *audioRecorder;
    QAudioRecorder *audioRecorderAI;
    QtCharts::QChartView* chartView;
    arduino A;


  protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // DASHBOARD_COMMS_H
