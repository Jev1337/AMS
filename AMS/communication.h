#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <QComboBox>
#include <QCryptographicHash>
#include <QDebug>
#include <QList>
#include <QMessageBox>
#include <QRegularExpression>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVector>
#include <QDate>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

class communication {
  private:
    int id;
    QString nom;
    QString adresse;
    QString num_tel;
    QString email;
    QString remarques;

  public:
    communication();
    communication(int, QString, QString, QString, QString, QString);
    ~communication() {}
    int getId();
    QString getNom();
    QString getAdresse();
    QString getNum_tel();
    QString getEmail();
    QString getRemarques();
    QString getDatefromId(int);
    void setID(int);
    void setNom(QString);
    void setAdresse(QString);
    void setNum_tel(QString);
    void setEmail(QString);
    void setRemarques(QString);
    void afficher();
    void modifier();
    void supprimer();
    void ajouter();
    void recherche();
    QString stat();
    void getEmergencyCallStat(QtCharts::QChartView *chartView);
    QStringList getBest();
    QString getFinished();
    void pdf();
};

#endif // COMMUNICATION_H
