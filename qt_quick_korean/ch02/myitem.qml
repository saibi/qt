import QtQuick 2.9


Rectangle {
    id: myItem

    signal message(string msg)

    width: 100; height: 100

    color: "green"

    MouseArea
    {
        anchors.fill: parent
        onClicked: myItem.message("clicked!")
    }
}
