import QtQuick 2.0

Item {
    id: root
    width: 100
    height: 62

    property alias drag: mousearea.drag

    MouseArea {
        id: mousearea
        anchors.fill: parent
        hoverEnabled: true
    }

    Row {
        id: layout
        layoutDirection: Qt.RightToLeft
        anchors {
            verticalCenter: parent.verticalCenter;
            left: parent.left
            right: parent.right
            leftMargin: 15
            rightMargin: 15
        }
        spacing: 20

        opacity: mousearea.containsMouse ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 350 }
        }
    }
}

