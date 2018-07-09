    Rectangle {
        id: root
        width: parent.width
        height: parent.height
        color: "lightgrey"

        Column {
            id: clockText
            anchors.centerIn: root
            spacing: 20

            Text {
                id: timeText
                anchors.horizontalCenter: parent.horizontalCenter
                text: "13:45"
            }
            Text {
                id: dateText
                anchors.horizontalCenter: parent.horizontalCenter
                text: "23.02.2012"
            }
        }

        MouseArea {
            anchors.fill: root
            onClicked: {
                Qt.quit();
            }
        }
    }

