import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true
    width: 400
    height: 400

    id: relay

    signal messageReceived(string person, string notice)

    Component.onCompleted: {
        relay.messageReceived.connect(sendToPost)
        relay.messageReceived.connect(sendToEmail)
    }

    function sendToPost(person, notice)
    {
        console.log("Sending to post: " + person + ", " + notice)
    }

    function sendToEmail(person, notice)
    {
        console.log("Sending to email: " + person + ", " + notice)
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            relay.messageReceived("Tom", "Happy birthday")
        }
    }

}
