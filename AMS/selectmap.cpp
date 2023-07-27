#include "selectmap.h"
#include "ui_selectmap.h"
#include "dashboard_hospital.h"
selectMap::selectMap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectMap)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
        ui->quickWidget->show();

        auto obj = ui->quickWidget->rootObject();
        connect(this, SIGNAL(setCenter(QVariant, QVariant)), obj, SLOT(setCenter(QVariant, QVariant)));
        connect(this, SIGNAL(addMarker(QVariant, QVariant)), obj, SLOT(addMarker(QVariant, QVariant)));
        connect(obj, SIGNAL(coordinateSelected(double, double)), this, SLOT(onCoordinateSelected(double, double)));

        emit setCenter(36.900068737969,10.189164289212641);
        emit addMarker(36.900068737969,10.189164289212641);
}

void selectMap::onCoordinateSelected(double latitude, double longitude)
{
    latit = latitude;
    longit = longitude;
}

selectMap::~selectMap()
{
    delete ui;
}

void selectMap::on_pushButton_2_released()
{
    QUrl url("https://atlas.microsoft.com/search/address/json?");
    QUrlQuery query;
    query.addQueryItem("subscription-key", "Ct8DmwBEG7voYEOXYg8P3CkuN8O1WQ3c_JWOIUshqUw");
    query.addQueryItem("api-version", "1.0");
    query.addQueryItem("query", ui->lineEdit->text());
    query.addQueryItem("limit", "1");
    query.addQueryItem("countrySet", "TN");
    url.setQuery(query);

    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(QNetworkRequest(url));
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray data = reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    if (!obj.contains("results")) {
        // Handle error
        return;
    }

    QJsonArray results = obj["results"].toArray();
    if (results.isEmpty()) {
        // Handle error
        return;
    }

    QJsonObject result = results[0].toObject();
    QJsonObject position = result["position"].toObject();

    double lat = position["lat"].toDouble();
    double lng = position["lon"].toDouble();

    emit addMarker(lat, lng);
}

void selectMap::on_pushButton_released()
{
    QDialog::accept();
    QString text=QString("%1,%2").arg(latit).arg(longit);
    emit locationEntered(text);
    this -> close();
}

void selectMap::on_pushButton_3_released()
{
    QDialog::reject();
    this -> close();
}
