import QtQuick 2.9
import QtQuick.Window 2.2
import Message 1.0

Window {
    visible: true
    width: 300
    height: 300
    title: qsTr("Hello World")

    Text {
        id: myText
        anchors.centerIn: parent
        text: msg.author
        font.pixelSize: 25
        Component.onCompleted: {
            msg.author = "Hello"
        }

        Msg {
            id: myMsg
            onNewMessagePosted: {
                console.log("Message received: ", subject)
                myText.font.pixelSize = 20
                myText.text = subject
            }

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
