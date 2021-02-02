import QtQuick 2.9
import QtQuick.Window 2.3


Window {
    visible: true
    width: 400
    height: 200

    color: "lightblue"

    LineEdit2 {
        id: lineEdit
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        anchors.topMargin: 16
        width: 300
        height: 50
    }

    Text {
        anchors.top: lineEdit.bottom
        anchors.topMargin: 12

        anchors.left: parent.left
        anchors.leftMargin: 16
        font.pixelSize: 20
        text: "<b>Summary:</b> " + lineEdit.text
    }
}
