#include "alarmhandler.h"
#include "weatherfetcher.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    AlarmHandler alarm;
    WeatherFetcher weather;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("AlarmHandler", &alarm);
    engine.rootContext()->setContextProperty("weatherFetcher", &weather);  // lowercase for QML usage

    const QUrl url(QStringLiteral("qrc:/Smart_Clock/main.qml"));
    engine.load(url);
    if (engine.rootObjects().isEmpty())
        return -1;

    weather.fetchWeather("Bangalore"); // initial fetch

    QTimer weatherTimer;
    weatherTimer.setInterval(900000); // 15 minutes
    QObject::connect(&weatherTimer, &QTimer::timeout, [&weather]() {
        weather.fetchWeather("Bangalore");
    });
    weatherTimer.start();

    return app.exec();
}
