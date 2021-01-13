import QtQuick 2.9

Item {
    property alias displayText: textItem.text
    width: 100
    height: 100
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

}
