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
            colorDialog.visible = true
        }
    }

    ColorDialog {
        id: colorDialog
        visible: false

        title: "Please choose a color"

        onAccepted: {
            console.log("You chose: " + colorDialog.color)
            Qt.quit()
        }
        onRejected: {
            console.log("Canceled")
            Qt.quit()
        }
        Component.onCompleted: visible = false

    }

}
