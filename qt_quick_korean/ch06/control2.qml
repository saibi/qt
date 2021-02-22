import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.3

ApplicationWindow
{
    id: window

    width: 360
    height: 220
    visible: true

    title: "Qt Quick Controls 2"

    Action {
        id: actionNew
        text: qsTr("&New")
        shortcut: StandardKey.New
        onTriggered: area.text = "새로만들기 - New"

    }

    Action {
        id: actionOpen
        text: qsTr("&Open")
        shortcut: StandardKey.Open
        onTriggered: area.text = "열기 - Open"
    }

    menuBar: MenuBar {
        Menu {
            title: "file"
            MenuItem { action: actionNew }
            MenuItem { action: actionOpen }
        }
    }

    header: ToolBar {
        RowLayout {
            ToolButton { action: actionNew }
            ToolButton { action: actionOpen }
        }
    }

    Rectangle {
        anchors.fill: parent
        ColumnLayout {
            id: colLayout
            width: parent.width; height: parent.height
            Label {
                id: area
                width: parent.width
                height: 200
            }
            RowLayout {
                id: rowLayout
                Button {
                    text: "My Button"
                    Layout.fillWidth: true
                }
                Button {
                    id: button
                    text: "Second"
                    highlighted: true
                    Layout.fillWidth: true
                }
                Switch {
                    id: element
                    x: 95
                    y: 165
                    text: qsTr("Switch")
                }
            } // RowLayout
        } // ColumnLayout
    } // Rectangle
}

