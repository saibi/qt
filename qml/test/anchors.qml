    Rectangle {
        id: root
        width: 80
        height: 80
        color: "lightgrey"

        Text {
            id: timeText
            anchors.top: root.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 10
            text: "13:45"

        }

        Text {
            id: dateText
            anchors.bottom: root.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 5
            text: "23.02.2012"
        }

        MouseArea {
            anchors.fill: root
            onClicked: {
                Qt.quit();
            }
        }
    }
 
