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
}
