
    Rectangle {
        width: 100
        height: 100
        color: "grey"

        Rectangle {
            width:50
            height: 80
            color: "lightgrey"

            // put this rect in the front.
            //z: 1

            // this rect will clip the next element
            clip: true

            Text {
                text: "Sunday, 5 o'clock"
            }
        }
        Rectangle {
            width: 25
            height: 40
            color: "green"
            Text {
                anchors.baseline: parent.verticalCenter
                text: "Tree time!"
            }
        }
    }

