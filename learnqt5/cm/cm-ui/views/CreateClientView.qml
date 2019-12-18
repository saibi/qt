import QtQuick 2.9
import assets 1.0
import components 1.0
import CM 1.0

Item {
    property Client newClient: masterController.ui_newClient

    Rectangle {
        anchors.fill: parent
        color: Style.colourBackground

        StringEditorSingleLine {
            stringDecorator: newClient.ui_name

        }
    }

    CommandBar {
        commandList: masterController.ui_commandController.ui_createClientViewContextCommands
    }
}
