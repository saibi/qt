import QtQuick 2.9
import QtQuick.Window 2.3

import "script.js" as MyScript

Window {
    visible: true
    title: qsTr("Hello World")

    id: item

    width: 200
    height: 200

    MouseArea {
        id: mouseArea
        anchors.fill: parent

    }

    Component.onCompleted:  {
        mouseArea.clicked.connect(MyScript.jsFunction)
    }

}
