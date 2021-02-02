import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2


ApplicationWindow {
    width: 300
    height: 300
    visible: true

    Button {
        text: "FontDialog loading"
        onClicked: {
            fontDialog.visible = true
        }
    }

    FontDialog {
        id: fontDialog
        visible: false

        title: "Please choose a font"

        onAccepted: {
            console.log("You chose: " + fontDialog.font)
            Qt.quit()
        }
        onRejected: {
            console.log("Canceled")
            Qt.quit()
        }
        Component.onCompleted: visible = false
    }
}
