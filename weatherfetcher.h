#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class WeatherFetcher : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString city READ city NOTIFY cityChanged)
    Q_PROPERTY(QString temp READ temp NOTIFY tempChanged)
    Q_PROPERTY(QString desc READ desc NOTIFY descChanged)

public:
    explicit WeatherFetcher(QObject *parent = nullptr);
    Q_INVOKABLE void fetchWeather(const QString &cityName);

    QString city() const { return m_city; }
    QString temp() const { return m_temp; }
    QString desc() const { return m_desc; }

signals:
    void cityChanged();
    void tempChanged();
    void descChanged();

private slots:
    void onFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager m_manager;
    QString m_city, m_temp, m_desc;
};
