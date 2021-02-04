import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 300
    height: 300
    title: qsTr("Hello World")

    Text {
        id: myText
        anchors.centerIn: parent
        text: msg.author
        Component.onCompleted: {
            msg.author = "Hello"
            myText.text = msg.author
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            var str = "Who are you?"
            var result = msg.postMessage(str)
            console.log("Result of postMessage():", result)
            msg.refresh()
        }
    }
}
