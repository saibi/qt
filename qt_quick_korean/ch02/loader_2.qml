import QtQuick 2.9
import QtQuick.Window 2.3

Window {

    visible : true

    width: 300
    height: 300

    Component {
        id: redSquare
        Rectangle {
            color: "red"
            width: 100
            height: 100
        }
    }

    Loader { sourceComponent: redSquare }
    Loader { sourceComponent: redSquare; x: 200; y: 100 }
}
