import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello from QML")

    ColumnLayout {
        id: gridLayout
        anchors.fill: parent

        Label {
            id: greetings
            text: qsTr("Hello from QML!")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            color: "red"
            font.bold: true
            font.pointSize: 20.4
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter


            Label {
                id: nameLabel
                text: qsTr("My Name is:")
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            TextField {
                id: nameField
                text: qsTr("")
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                onAccepted: {
                    greetings.text ="Nice to meet you " + nameField.text + "!!!"
                }
            }
        }

        RoundButton {
            id: button
            text: qsTr("Enter Name")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            onClicked: {
                greetings.text = "Nice to meet you " + nameField.text + "!"
            }
        }
    }
}
