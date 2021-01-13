import QtQuick 2.9

Item {
    signal clicked()
    property alias text: textItem.text

    width: 40
    height: width

    anchors.right: parent.right
    opacity: 0.5

    Text {
        id: textItem
        text: "+"
        anchors.centerIn: parent
        font.pixelSize: 40
    }
    MouseArea {
        onClicked: parent.clicked()
        anchors.fill: parent
    }
}
