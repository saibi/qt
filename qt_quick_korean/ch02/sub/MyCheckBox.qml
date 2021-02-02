import QtQuick 2.9

Item {
    width: 10

    signal myChecked(bool checkValue)

    Image {
        id: checkImage
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        id: textItem
        anchors.left: checkImage.right
        anchors.leftMargin: 4
        anchors.verticalCenter: checkImage.verticalCenter
        text: "Option"
        font.pixelSize: checkImage.height - 6
    }

    states: [
        State {
            name: "checked"
            PropertyChanges {
                target: checkImage
                source: "../images/checked.svg"
            }
        },
        State {
            name: "unchecked"
            PropertyChanges {
                target: checkImage
                source: "../images/unchecked.svg"
            }
        }

    ]

    state: "unchecked"

    MouseArea {
        anchors.fill: checkImage
        onClicked: {
            if ( parent.state == "checked" ) {
                parent.state = "unchecked";
                parent.myChecked(false);
            } else {
                parent.state = "checked";
                parent.myChecked(true);
            }
        }

    }
}
