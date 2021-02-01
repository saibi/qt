import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true
    height: 200
    width: 200

    Loader {
        id: loader
        focus: true

        source: "keyreader.qml"
    }

}
