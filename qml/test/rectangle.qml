    Rectangle {
        width: 100
        height: 100
        color: "grey"

        Rectangle {
            width: 50
            height:80
            color: "lightgrey"
            Text {
                text: "Sunday, 5 o'clock"
            }
        }

        Rectangle {
            width: 25
            height: 40
            color: "green"
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "Tree time!"
            }
        }
    }
