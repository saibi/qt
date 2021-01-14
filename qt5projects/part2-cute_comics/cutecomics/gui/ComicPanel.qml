import QtQuick 2.9

Item {
    id: panel
    signal clicked()

    property alias displayText: textItem.text
    property alias imageSource: image.source

    width: 100
    height: 100

    Image {
        id: image
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
    }

    Rectangle {
        anchors.fill: parent
        border.color: "#000"
        border.width: 2
        opacity: 0.5
    }

    Text {
        id: textItem
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: panel.clicked()
    }


}
