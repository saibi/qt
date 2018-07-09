import QtQuick 2.0


    Rectangle {
        id: root

        width: parent.width
        height: parent.height

        property bool showDate: true
        property bool showSeconds: true
        property string currentTime: "23:59"
        property string currentDate: "31.12.12"
        property real borderProportion: 0.1
        property real timeTextProportion: 0.5
        property real dateTextProportion: 0.2
        property string textColor: "red"


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

