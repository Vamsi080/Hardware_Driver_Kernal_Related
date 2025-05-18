#include "weatherfetcher.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QUrl>

WeatherFetcher::WeatherFetcher(QObject *parent) : QObject(parent) {
    connect(&m_manager, &QNetworkAccessManager::finished,
            this, &WeatherFetcher::onFinished);
}

void WeatherFetcher::fetchWeather(const QString &cityName) {
    const QString apiKey = "5bd43b68ee00660f8e3b6954e769a531"; // Replace with your key
    const QString urlStr = QString("http://api.openweathermap.org/data/2.5/weather?q=%1&units=metric&appid=%2")
                               .arg(cityName).arg(apiKey);
    QUrl url(urlStr);
    m_manager.get(QNetworkRequest(url));
}

void WeatherFetcher::onFinished(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        const QByteArray data = reply->readAll();
        QJsonObject json = QJsonDocument::fromJson(data).object();

        m_city = json["name"].toString();
        emit cityChanged();

        QJsonObject main = json["main"].toObject();
        m_temp = QString::number(main["temp"].toDouble()) + " °C";
        emit tempChanged();

        QJsonArray weatherArray = json["weather"].toArray();
        if (!weatherArray.isEmpty()) {
            m_desc = weatherArray[0].toObject()["description"].toString();
            emit descChanged();
        }
    }
    reply->deleteLater();
}
