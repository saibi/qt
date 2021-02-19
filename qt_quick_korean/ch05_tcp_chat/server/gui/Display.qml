import QtQuick 2.9

Rectangle {
    property alias text: output.text
    color: "transparent"
    border.color: "transparent"
    BorderImage {
        anchors.fill: parent
        border {
            left: 5
            top: 5
            right: 5
            bottom: 6
        }
        horizontalTileMode: BorderImage.Stretch
        verticalTileMode: BorderImage.Stretch
        source: "qrc:/qt-project.org/imports/chat/gui/textoutput.png"
    }

    TextEdit {
        id: output
        anchors {
            margins: 3
            fill: parent
        }
        selectionColor: "transparent"
    }

}
