import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.1

ApplicationWindow {
    width: 100
    height: 100
    visible: true

    Rectangle {
        width: parent.width
        height: parent.height
        color: "#cccccc"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(myStatus.active == true)
                    myStatus.active = false
                else
                    myStatus.active = true
            }
        }

        StatusIndicator {
            id: myStatus
            anchors.centerIn: parent
            color: "green"
        }
    }

}
