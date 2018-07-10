import QtQuick 2.0

    Rectangle {
        id: root

        width: parent.width
        height: parent.height

        property bool showDate: true
        property bool showSeconds: true
        property string currentTime
        property string currentDate
        property real borderProportion: 0.1
        property real timeTextProportion: 0.5
        property real dateTextProportion: 0.2
        property string textColor: "red"
        property string timeFormat: "hh :mm"
        property string dateFormat: "yy/MM/dd"

        color: "transparent"

        function getFormattedDateTime(format) {
            var date = new Date
            return Qt.formatDateTime(date, format)
        }

        function updateTime() {
            root.currentTime = "<big>" +
                    getFormattedDateTime(timeFormat) +
                    "</big>" +
                    (root.showSeconds ? " <sup><small>" + getFormattedDateTime("ss") + "</small></sup>" : "");
            root.currentDate = getFormattedDateTime(dateFormat);
        }

        Image {
            id: background
            source: "resources/light_background.png"
            fillMode: "Tile"
            anchors.fill: parent
            onStatusChanged: if ( background.status == Image.Error )
                                 console.log( qsTr("Background image \"" ) + source + qsTr("\" cannot be loaded\""))
        }

        FontLoader {
            id: ledFont
            source: "resources/LED_REAL.TTF"
            onStatusChanged: if ( ledFont.status == FontLoader.Error )
                                 console.log( qsTr("Font \"") + source + qsTr("\" cannot be loaded") )
        }

        Timer {
            id: updateTimer
            running: Qt.application.active && visible == true
            repeat: true
            triggeredOnStart: true
            onTriggered: {
                root.updateTime()

                interval = 1000 * (root.showSeconds ? 1 : (60 - getFormattedDateTime("ss")))
            }
        }

        onShowSecondsChanged: {
            root.updateTime()
        }

        Column {
            id: clockText
            anchors.centerIn: parent
            spacing: root.height * root.borderProportion

            Text {
                id: timeText
                text: root.currentTime
                font.pixelSize: root.height * root.timeTextProportion
                font.family: ledFont.name
                font.bold: true
                color: root.textColor
                style: Text.Raised
                styleColor: "black"
            }

            Text {
                id: dateText
                text: root.currentDate
                color: root.textColor
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: ledFont.name
                font.pixelSize : root.height * root.dateTextProportion
                visible: root.showDate
                style: Text.Raised
                styleColor: "black"
            }
        }
    }

