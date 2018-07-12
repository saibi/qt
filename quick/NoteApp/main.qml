import QtQuick 2.0
//import QtQuick.Window 2.2

Rectangle {
    id: window

 //   visible: true
    width: 800
    height: 600

    MarkerPanel {
        id: markerPanel
        width: 50
        anchors.topMargin: 20

        anchors {
            top: window.top
            right: window.right
            bottom: window.bottom
        }
    }

    Rectangle {
        id: toolbar

        width: 50

        color: "#444a4b"
        anchors {
            left: window.left
            top: window.top
            bottom: window.bottom
            topMargin: 100;
            bottomMargin: 100
        }

        Column {
            anchors.fill: parent
            anchors.topMargin: 30
            spacing: 20

            Repeater {
                model: 2

                Rectangle {
                    width: 50;
                    height: 50;
                    color: "red"
                }
            }
        }
    }

    Page {
        id: page1
        anchors {
            top: window.top
            bottom: window.bottom
            right: markerPanel.left
            left: toolbar.right
        }
    }
}
