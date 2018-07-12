import QtQuick 2.0

Image {
    id: root
//    width: 50
//  height: 90
//    color: "#0a7bfb"

    signal clicked()

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: root.clicked()
    }
}
