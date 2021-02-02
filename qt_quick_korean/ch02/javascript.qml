import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true
    title: qsTr("Hello")

    id: colorButton

    width: 200
    height: calculateHeight();

    color: mousearea.pressed ? "steelblue" : "lightsteelblue"

    MouseArea {
        id: mousearea
        anchors.fill: parent
    }

    function calculateHeight()
    {
        return colorButton.width / 2;
    }
}
