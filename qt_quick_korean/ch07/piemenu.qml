import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.1

ApplicationWindow
{
    width: 300
    height: 300
    visible: true

    MouseArea {
        anchors.fill: parent
        onClicked: {
            pieMenu.popup(mouseX, mouseY)
        }

    }

    PieMenu {
        id: pieMenu

        triggerMode: TriggerMode.TriggerOnClick
        MenuItem {
            text: "Action 1"
            iconSource: "qrc:/images/qtlogo.png"
            onTriggered: {
                console.log('Action 1 click')
            }
        }
        MenuItem {
            text: "Action 2"
            iconSource: "qrc:/images/pinwheel.png"
            onTriggered: {
                console.log('Action 2 click')
            }
        }
        MenuItem {
            text: "Action 3"
            iconSource: "qrc:/images/qtlogo.png"
            onTriggered: {
                console.log('Action 3 click')
            }
        }
    }

}
