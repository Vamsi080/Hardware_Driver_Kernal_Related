#pragma once
#include <QObject>
#include <QTime>
#include <QTimer>

class AlarmHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString alarm READ alarm NOTIFY alarmChanged)
public:
    explicit AlarmHandler(QObject *parent = nullptr)
        : QObject(parent), m_checkTimer(new QTimer(this)) {
        m_checkTimer->setInterval(1000);
        connect(m_checkTimer, &QTimer::timeout, this, &AlarmHandler::checkAlarm);
        m_checkTimer->start();
    }

    QString alarm() const {
        return m_alarm.toString("HH:mm");
    }

public slots:
    void setAlarm(const QString &hhmm){
        m_alarm = QTime::fromString(hhmm, "HH:mm");
        emit alarmChanged();
    }
    void clearAlarm(){
        m_alarm = QTime();
        emit alarmChanged();
    }

signals:
    void alarmTriggered();
    void alarmChanged();

private slots:
    void checkAlarm(){
        if (!m_alarm.isValid()) return;
        QTime now = QTime::currentTime();
        if (now.hour() == m_alarm.hour() && now.minute() == m_alarm.minute() && now.second() == 0) {
            emit alarmTriggered();
        }
    }

private:
    QTime m_alarm;
    QTimer *m_checkTimer;
};
