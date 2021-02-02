import QtQuick 2.9
import QtQuick.Window 2.3

import "script.js" as MyScript

Window {
    visible: true
    title: qsTr("Hello World")

    id: item

    width: 500
    height: 200

    MouseArea {
        anchors.fill: parent
        onClicked: {
            MyScript.showCalculations(10)
            console.log("Call from qml: ", MyScript.factorial(10))
        }
    }
    Text {

        anchors.fill: parent
        text: "click me"
    }
}
