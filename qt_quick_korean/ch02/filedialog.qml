import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2


ApplicationWindow {
    width: 300
    height: 300
    visible: true

    Button {
        text: "FileDialog loading"
        onClicked: {
            fileDialog.visible = true
        }
    }

    FileDialog {
        id: fileDialog
        visible: false
        folder: shortcuts.home

        title: "Please choose a file"

        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            Qt.quit()
        }
        onRejected: {
            console.log("Canceled")
            Qt.quit()
        }
        Component.onCompleted: visible = false
    }
}
