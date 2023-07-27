#ifndef AMBULANCE_H
#define AMBULANCE_H
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
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QStandardItemModel>

class ambulance {
  private:
    QString matricule;
    QString etat;
    int capacite;
    QString emplacement;
    QString equipements;
    QString remarques;

  public:
    ambulance();
    ambulance(QString, QString, int, QString, QString, QString);
    ~ambulance() {}

    QString get_matricule();
    QString get_etat();
    int get_capacite();
    QString get_emplacement();
    QString get_equipements();
    QString get_remarques();

    void set_matricule(QString);
    void set_etat(QString);
    void set_capacite(int);
    void set_emplacement(QString);
    void set_equipements(QString);
    void set_remarques(QString);

    void afficher();
    void modifier();
    void supprimer();
    void ajouter();
    QStandardItemModel* getPendingConf();
};

#endif // AMBULANCE_H
