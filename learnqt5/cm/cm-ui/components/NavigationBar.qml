import QtQuick 2.9

import assets 1.0

Item {
    property bool isCollapsed: true

    anchors {
        top: parent.top
        bottom: parent.bottom
        left: parent.left
    }


    width: isCollapsed ? Style.widthNavigationBarCollapsed : Style.heightNavigationBarExpanded

    Rectangle {
        anchors.fill: parent
        color: Style.colourNavigationBarBackground

        Column {
            NavigationButton {
                iconCharacter: "\uf0c9"
                description: ""
            }
            NavigationButton {
                iconCharacter: "\uf015"
                description: "Dashboard"
            }

            NavigationButton {
                iconCharacter: "\uf234"
                description: "New Client"
            }
            NavigationButton {
                iconCharacter: "\uf002"
                description: "Find Client"
            }
        }

        clip: true


    }

}
