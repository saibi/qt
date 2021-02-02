import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true
    title: qsTr("Hello")

    width: 200
    height: 200

    function factorial(a) {
        a = parseInt(a);
        if ( a <= 0 )
            return 1;
        else
            return a * factorial(a - 1);

    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log(factorial(10))
            label.moveTo(mouse.x, mouse.y)
        }
    }

    Text {
        id: label

        function moveTo(newX, newY) {
            label.x = newX;
            label.y = newY;
        }

        text: "Move me!"
    }
}
