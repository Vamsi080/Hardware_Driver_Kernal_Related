import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 6.6

ApplicationWindow {
    visible: true
    width: 320
    height: 240
    title: "Smart Clock"

    Connections {
        target: AlarmHandler
        function onAlarmTriggered() {
            alarmLabel.text = "⏰ WAKE UP!"
            buzzer.play()
        }
    }

    Timer {
        interval: 1000; running: true; repeat: true
        onTriggered: timeLbl.text = Qt.formatDateTime(new Date(), "hh:mm:ss")
    }

    ColumnLayout {
        anchors.fill: parent; anchors.margins: 12; spacing: 10

        Text {
            id: timeLbl
            text: Qt.formatDateTime(new Date(), "hh:mm:ss")
            font.pixelSize: 42
            Layout.alignment: Qt.AlignHCenter
        }

        Text {
            text: Qt.formatDateTime(new Date(), "dddd, dd MMM yyyy")
            font.pixelSize: 16
            color: "gray"
            Layout.alignment: Qt.AlignHCenter
        }

        Text {
            id: weatherText
            text: weatherFetcher.city + ": " + weatherFetcher.temp + ", " + weatherFetcher.desc
            font.pixelSize: 16
            color: "lightblue"
            Layout.alignment: Qt.AlignHCenter
        }

        RowLayout {
            spacing: 12
            Layout.alignment: Qt.AlignHCenter
            Button { text: "Set Alarm"; onClicked: alarmDlg.open() }
            Button {
                text: "Clear"
                onClicked: {
                    AlarmHandler.clearAlarm()
                    alarmLabel.text = "Alarm Cleared"
                }
            }
        }

        Text {
            id: alarmLabel
            text: AlarmHandler.alarm === "" ? "No Alarm" : "Alarm: " + AlarmHandler.alarm
            font.pixelSize: 14
            color: "red"
            Layout.alignment: Qt.AlignHCenter
        }
    }

    Dialog {
        id: alarmDlg
        modal: true
        title: "Set Alarm"
        standardButtons: Dialog.Ok | Dialog.Cancel

        contentItem: ColumnLayout {
            anchors.margins: 12
            spacing: 8
            RowLayout {
                spacing: 8
                Label { text: "Hour:" }
                SpinBox { id: hourSpin; from: 0; to: 23 }
            }
            RowLayout {
                spacing: 8
                Label { text: "Minute:" }
                SpinBox { id: minuteSpin; from: 0; to: 59 }
            }
        }

        onAccepted: {
            const hh = ("0" + hourSpin.value).slice(-2)
            const mm = ("0" + minuteSpin.value).slice(-2)
            const t = `${hh}:${mm}`
            AlarmHandler.setAlarm(t)
            alarmLabel.text = "Alarm: " + t
        }
    }

    SoundEffect {
        id: buzzer
        source: "qrc:/alarm.wav"
    }
}
