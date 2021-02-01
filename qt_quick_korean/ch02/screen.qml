import QtQuick 2.9
import QtQuick.Window 2.3


Window {

    visible: true

    width : Screen.width / 2;
    height : Screen.height / 2;
    Rectangle
    {
        width : 300
        height: 200
        color: "blue"
        anchors.centerIn: parent
    }
}
