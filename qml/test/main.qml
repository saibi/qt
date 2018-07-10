import QtQuick 2.9
import QtQuick.Window 2.2

import "js/logic.js" as Logic
import "js/style.js" as Style

Window {
    visible: true
    width: 400
    height: 120
    title: qsTr("Window")


    // example code : start

    Rectangle {
        id: root

        width: parent.width
        height: parent.height

        property bool showDate: true
        property bool showSeconds: true
        property string currentTime
        property string currentDate
        property string textColor: "green"

        color: "transparent"

        function updateTime() {
            root.currentTime = "<big>" +
                    Logic.getFormattedDateTime(Style.timeFormat) +
                    "</big>" +
                    (root.showSeconds ? " <sup><small>" + Logic.getFormattedDateTime("ss") + "</small></sup>" : "");
            root.currentDate = Logic.getFormattedDateTime(Style.dateFormat);
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
            spacing: root.height * Style.borderProportion

            Text {
                id: timeText
                textFormat: Text.RichText
                text: root.currentTime
                font.pixelSize: root.height * Style.timeTextProportion
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
                font.pixelSize : root.height * Style.dateTextProportion
                visible: root.showDate
                style: Text.Raised
                styleColor: "black"
            }
        }
    }

    // example code : end
}






// default code
//import QtQuick 2.9
//import QtQuick.Window 2.2

//Window {
//    visible: true
//    width: 640
//    height: 480
//    title: qsTr("Hello World")
//}
