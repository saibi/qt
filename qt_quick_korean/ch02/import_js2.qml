import QtQuick 2.9
import QtQuick.Window 2.3

import "factorial.js" as MyLib

Window {
    visible: true
    title: qsTr("Hello World")

    id: item

    width: 500
    height: 200

    Text {
        width: parent.width
        height: parent.height

        property int input: 17

        text: "The number of " + input + " is: " + MyLib.factorial(input) + " , " + MyLib.factorialCallCount()
    }
}
