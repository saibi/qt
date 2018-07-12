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

        width:50

        color: "#444a4b"

        anchors {
            left:window.left
            top: window.top
        }


        Rectangle {
            anchors.fill: parent
            color: "white"
            opacity: 0.15

            anchors {
                left: window.left
                top: window.top
                bottom: window.bottom
                topMargin: 100;
                bottomMargin: 100
            }

            radius: 16
            border {
                color: "#600"
                width: 4
            }
        }

        Column {
            anchors.fill: parent
            spacing: 16
            anchors {
                top: window.top
                left: window.left
                bottom: window.bottom
                topMargin: 50
                bottomMargin: 50
                leftMargin: 8
            }

            Tool {
                id: newNoteTool
                source: "images/add.png"
            }

            Tool {
                id: clearAllTool
                source: "images/clear.png"
            }
        }
    }

    PagePanel {
        id: pagePanel

        state: markerPanel.activeMarker
        anchors {
            right: markerPanel.left
            left: toolbar.right
            top : parent.top
            bottom: parent.bottom
            leftMargin: 1
            rightMargin: -50
            topMargin: 3
            bottomMargin: 15
        }
    }
}
