import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    width: 300
    height: 200
    visible: true

    ListModel {
        id: gridModel

        ListElement { name: "Picture 1"; frame: "images/101.jpg" }
        ListElement { name: "Picture 2"; frame: "images/102.jpg" }
        ListElement { name: "Picture 3"; frame: "images/103.jpg" }
        ListElement { name: "Picture 4"; frame: "images/104.jpg" }
    }

    Component {
        id: contactDelegate
        Item {
            width: grid.cellWidth
            height: grid.cellHeight

            Column {
                anchors.fill: parent
                Image {
                    width: 80
                    height: 50
                    source: frame
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Text {
                    text: name
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }

    Component {
        id: highlight
        Rectangle {
            width: grid.cellWidth
            height: grid.cellHeight
            color: "lightsteelblue"
            radius: 5
            x: grid.currentItem.x
            y: grid.currentItem.y

            Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
            Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
        }
    }

    GridView {
        id: grid
        anchors.fill: parent
        cellWidth: 90
        cellHeight: 80
        model: gridModel
        delegate: contactDelegate
        highlight: highlight
        highlightFollowsCurrentItem: true
        focus: true

    }
}
