#include "sms.h"


void sendSms(QString dest){
    // Step 1: Get a token
        QNetworkAccessManager manager;
        QNetworkRequest request(QUrl("https://api.orange.com/oauth/v3/token"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setRawHeader("Authorization", "Basic RnllZ3NrN0MyREVMMVdMWTZ4NVV1MGo1RTAwaFRUT3Y6d3BoZzlRQWRIY1pFZmlRWA==");

        QByteArray postData("grant_type=client_credentials");

        QNetworkReply* reply = manager.post(request, postData);
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Failed to get token:" << reply->errorString();
            reply->deleteLater();
            return;
        }

        QByteArray response = reply->readAll();
        reply->deleteLater();

        QJsonParseError jsonError;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            qDebug() << "Failed to parse token response:" << jsonError.errorString();
            return;
        }

        QString accessToken = jsonResponse.object().value("access_token").toString();

        // Step 2: Send SMS
        QNetworkRequest smsRequest(QUrl("https://api.orange.com/smsmessaging/v1/outbound/tel%3A%2B21652920276/requests"));
        smsRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        smsRequest.setRawHeader("Authorization", ("Bearer " + accessToken).toUtf8());

        QJsonObject smsData;
        dest.replace("+216", "");
        smsData["address"] = "tel:+216"+dest;
        smsData["senderAddress"] = "tel:+21652920276";

        QJsonObject smsText;
        smsText["message"] = "AMS AUTH: Your password has been reset. Please check your E-Mail for your new password!";
        smsData["outboundSMSTextMessage"] = smsText;

        QJsonObject smsRequestData;
        smsRequestData["outboundSMSMessageRequest"] = smsData;

        QJsonDocument smsRequestJson(smsRequestData);
        QByteArray smsPostData = smsRequestJson.toJson();

        reply = manager.post(smsRequest, smsPostData);
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Failed to send SMS:" << reply->errorString();
            reply->deleteLater();
            return;
        }

        qDebug() << "SMS sent successfully!";
        reply->deleteLater();
}
