import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2


ApplicationWindow {
    width: 300
    height: 300
    visible: true

    Button {
        text: "Dialog loading"
        onClicked: {
            myDial.visible = true
        }
    }

    Dialog {
        id: myDial
        visible: false

        title: "Blue sky dialog"

        contentItem: Rectangle {
            color: "lightskyblue"
            implicitWidth: 200
            implicitHeight: 200

            Text {
                text: "Hello blue sky!"
                color: "navy"
                anchors.centerIn: parent
            }
        }
    }
}
