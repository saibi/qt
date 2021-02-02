import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true

    title: qsTr("Hello World")

    id: colorButton
    width: 200
    height: 80

    color: mousearea.pressed ? "steelblue" : "lightsteelblue"

    MouseArea {
        id: mousearea
        anchors.fill: parent
    }
}
