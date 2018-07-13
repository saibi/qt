import QtQuick 2.0
//import QtQuick.LocalStorage 2.0 as Sql

import "noteDB.js" as NoteDB

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
        anchors.fill: toolbar
        color: "white"
        opacity: 0.15

        radius: 16
        border {
            color: "#600"
            width: 4
        }
    }

    Column {
        id: toolbar
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
            onClicked: pagePanel.currentPage.newNote()
        }

        Tool {
            id: clearAllTool
            source: "images/clear.png"
            onClicked: pagePanel.currentPage.clear()
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

    Component.onCompleted: {
        NoteDB.openDB();
    }
}
