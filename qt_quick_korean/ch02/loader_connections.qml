import QtQuick 2.9
import QtQuick.Window 2.3


Window {
    visible: true; width: 200; height: 200
    Loader {
        id: myLoader
        source: "myitem.qml"
    }

    Connections {
        target: myLoader.item
        onMessage: console.log(msg)
    }
}
