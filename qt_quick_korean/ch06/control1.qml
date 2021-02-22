import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    width: 200; height: 200; visible: true
    ColumnLayout {
        anchors.centerIn: parent; spacing: 6
        Button { text: "My Byttoon" }
        Button {
            text: "A button"
            style: ButtonStyle {
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 25
                    border.width: control.activeFocus ? 2 : 1
                    border.color: "#888"
                    radius: 4
                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: control.pressed ? "#ccc" : "#eee"
                        }
                        GradientStop {
                            position: 1
                            color: control.pressed ? "#aaa" : "#ccc"
                        }
                    }
                }
            }
        }
    }
}


/*
	title: qsTr("Hello World");
    width: 400
    height: 600
	visible: true

	BusyIndicator {
		anchors.centerIn: parent

		running: true 
	}

    Button {
        anchors.centerIn: parent
        text: "button"
        tooltip: "description"
    }

    Calendar {
        width: parent.width
        weekNumbersVisible: true
    }

    Column {
        anchors.bottom: parent.bottom
        ProgressBar {
            value: 0.5
        }
    }

//


    title: qsTr("Button Action")
    width: 380; height: 320; visible: true

    ColumnLayout {
        anchors.centerIn: parent
        Button { action: actionButtonA }
        Button { action: actionButtonB }
    }
    Action {
        id: actionButtonA; text: "Button A"
        onTriggered: statusLabel.text = "Button A Triggered "
    }
    Action {
        id: actionButtonB; text: "Button B"
        checkable: true
        onCheckedChanged:
            statusLabel.text = "Button B checked: " + checked
    }
    statusBar: StatusBar {
        Label { id: statusLabel; text: "Status Bar" }
    }
*/
