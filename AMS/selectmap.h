#ifndef SELECTMAP_H
#define SELECTMAP_H

#include <QDialog>
#include <QQuickItem>
#include <QVariant>
#include <QMessageBox>
#include <QVector>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
namespace Ui {
class selectMap;
}

class selectMap : public QDialog
{
    Q_OBJECT

public:
    explicit selectMap(QWidget *parent = nullptr);
    ~selectMap();
    QVector<double> getResult() const {
        QVector<double> location;
        location.append(latit);
        location.append(longit);
        return location;
    }

private:
    Ui::selectMap *ui;
    double latit;
    double longit;
signals:
    void setCenter(QVariant, QVariant);
    void addMarker(QVariant, QVariant);
    void locationEntered(const QString& text);


private slots:
    void onCoordinateSelected(double lat, double lng);
    void on_pushButton_2_released();
    void on_pushButton_released();
    void on_pushButton_3_released();
};

#endif // SELECTMAP_H
