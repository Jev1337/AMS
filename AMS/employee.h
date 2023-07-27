#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <QComboBox>
#include <QCryptographicHash>
#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QRegularExpression>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QString>
#include <QVector>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtNetwork>
#include <QStandardItemModel>
class employee {
  public:
    employee();
    employee(QString, QString, int);
    employee(QString, int, QString, QString, QString, QString, QString, QString, int);
    QString getCIN();
    int getType();
    QString getMail();
    QString getNom();
    QString getPrenom();
    QString getDateNaiss();
    QString getRemarks();
    QString getPhone();
    QStringList getArds();

    int getAdmin();
    void setCIN(QString);
    void setType(int);
    void setMail(QString);
    void setNom(QString);
    void setPrenom(QString);
    void setDateNaiss(QString);
    void setPhone(QString);
    void setRemarks(QString);
    bool getAccountTypes(QComboBox *);
    QSqlRelationalTableModel *getAccountList();
    QStandardItemModel * getMessages();
    bool addSelf(QString password);
    bool removeSelf();
    bool updateSelf();
    bool changePw(QString);
    QString GetCurrentFaceID(QString email);
    bool setFaceID(QString);
    int GetLastFingerprintID();
    bool setFingerprint(int);
    void chartRefresh(QtCharts::QChartView *);
    QString getEmpAvgAge();
    QString getEmpMonth();
    QString getTotalEmp();
    int getCurrentFingerprint();
    double getEmpIncreasePercent();
    bool cinExistance(QString);
    bool emailExistance(QString);
    bool getDeps(int id, QComboBox *comboBox);
    void setAdmin(bool);
    QString getCINfromEMAIL(QString email);
    QString getPHONEfromEMAIL(QString email);

  private:
    QString CIN;
    int type;
    QString mail;
    QString nom;
    QString prenom;
    QString phone;
    QString datenaiss;
    QString remarks;
    int admin;
    QStringList ards;
};

bool EmailValidator(QString);
bool PasswordValidator(QString);
bool CINValidator(QString);
bool NameValidator(QString);
#endif // EMPLOYEE_H
