import QtQuick 2.0

Rectangle {
    id: root
    width: 100
    height: 62
    color: "#9e964a"

    property alias drag: mousearea.drag

    MouseArea {
        id: mousearea
        anchors.fill: parent
    }
}

