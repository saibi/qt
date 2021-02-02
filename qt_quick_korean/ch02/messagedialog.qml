import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2


ApplicationWindow {
    width: 300
    height: 300
    visible: true

    Button {
        text: "MessageDialog loading"
        onClicked: {
            messageDialog.visible = true
        }
    }

    MessageDialog {
        id: messageDialog

        icon: StandardIcon.Question

        title: "file already exists"
        detailedText: "Do you want to overwrite?"

        standardButtons: StandardButton.Yes | StandardButton.YesToAll | StandardButton.No | StandardButton.NoToAll | StandardButton.Abort

        onYes: console.log("yes")
        onNo: console.log("no")

        onRejected: {
            console.log("Canceled")
            Qt.quit()
        }
        Component.onCompleted: visible = false
    }
}
