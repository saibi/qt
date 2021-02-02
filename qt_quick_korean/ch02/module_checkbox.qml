import QtQuick 2.9
import QtQuick.Window 2.3
import "sub"

Window {
    id: root
    visible: true

    width: 500
    height: 100

    color: "lightblue"

    MyCheckBox {
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter

        onMyChecked: checkValue ? root.color = "red" : root.color = "lightblue"
    }
}
