import QtQuick 2.9

Item {
    width: 640
    height: 480

    property int someNumber: 100

    Rectangle {
        anchors.fill: parent
        color: "yellow"
        objectName: "rect"

    }


    function myFunction(msg) {
        console.log("Got message: ", msg)
        return "some return value"
    }
}

