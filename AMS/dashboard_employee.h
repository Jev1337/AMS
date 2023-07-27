#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "employee.h"
#include "libraries/framelesswindow.h"
#include "mainwindow.h"
#include "modifyemployee.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QItemDelegate>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMovie>
#include <QProcess>
#include <QSerialPort>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QStandardItem>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QTextToSpeech>
#include <QTimer>
#include <QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtPrintSupport/QPrinter>
#include <opencv2/opencv.hpp>
#include <zbar.h>
#include <QAudioRecorder>

namespace Ui {
class dashboard;
}

class dashboard : public CFramelessWindow {
    Q_OBJECT

  public:
    explicit dashboard(employee &e, QWidget *parent = nullptr);
    ~dashboard();
    void sendOpenAIRequest();
    void refreshList();

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

    void on_pushButton_49_pressed();

    void on_pushButton_32_released();

    void on_pushButton_33_released();

    void on_pushButton_48_released();

    void on_pushButton_3_released();

    void on_New_Btn_released();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_stackedWidget_4_currentChanged(int arg1);

    void on_stackedWidget_currentChanged(int arg1);

    void on_stackedWidget_6_currentChanged(int arg1);

    void on_pushButton_5_released();

    void on_pushButton_7_released();

    void on_AboutBtn_released();

    void on_pushButton_4_released();

    bool updateCam();

    void on_ScanBtn_released();

    void on_checkBox_3_released();

    void on_checkBox_3_toggled(bool checked);

    void on_checkBox_4_toggled(bool checked);

    void on_New_Type_currentIndexChanged(int index);

    void toggleDropdownWidget();

    void on_pushButton_15_released();

    void on_pushButton_20_released();

    void on_settingsBtn_released();

    void on_pushButton_23_released();

    void on_pushButton_24_released();

    void on_pushButton_10_toggled(bool checked);

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_10_released();

    void changeToMain(int);

    void changeToAlpha(int);

    void toggleDropdownWidget2();

    void on_pushButton_13_released();

    void showArdStats(QString message);



private:
    Ui::dashboard *ui;
    employee e;
    QtCharts::QChartView *chartView;
    cv::VideoCapture *cap = nullptr;
    cv::Mat frame;
    QTimer *timerCam;
    QTextToSpeech *tts;
    arduino A;
    QAudioRecorder *audioRecorder;

  protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // DASHBOARD_H
