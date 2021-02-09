import QtQuick 2.9


Item {
    id: item

    width: 640
    height: 480

    signal qmlSignal(string msg)

    MouseArea {
        anchors.fill: parent
        onClicked: {
            item.qmlSignal("Hello from QML")
        }
    }
}
