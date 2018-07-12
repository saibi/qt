import QtQuick 2.0

Rectangle {
    id: root
    width: 50
    height: 300

    Column {
        id: layout
        anchors.fill: parent
        spacing: 10

        Repeater {
            model: 3
            delegate:
                Marker { id : marker }
        }
    }
}
