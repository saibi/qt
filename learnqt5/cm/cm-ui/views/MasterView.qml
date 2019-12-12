import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import assets 1.0
import components 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Client Management")

    // hidden by StackView
    Text {
        text: masterController.ui_welcomeMessage
    }

    Connections {
        target: masterController.ui_navigationController
        onGoCreateClientView: contentFrame.replace("qrc:/views/CreateClientView.qml")
        onGoDashboardView: contentFrame.replace("qrc:/views/DashboardView.qml")
        onGoEditClientView: contentFrame.replace("qrc:/views/EditClientView.qml", {selectedClient:client})
        onGoFindClientView: contentFrame.replace("qrc:/views/FindClientView.qml")
    }

    Rectangle {
        id: navigationBar
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
        }

        width: 100
        color: "#000000"


//        Column {
//            Button {
//                text: "Dashboard"
//                onClicked: masterController.ui_navigationController.goDashboardView()

//            }

//            Button {
//                text: "New Client"
//                onClicked: masterController.ui_navigationController.goCreateClientView()

//            }

//            Button {
//                text: "Find Client"
//                onClicked: masterController.ui_navigationController.goFindClientView()

//            }

//        }

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

    StackView {
        id: contentFrame
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
            left: navigationBar.right

        }
        initialItem: Qt.resolvedUrl("qrc:/views/SplashView.qml")
        clip: true
    }

    Component.onCompleted: contentFrame.replace("qrc:/views/DashboardView.qml");

}
