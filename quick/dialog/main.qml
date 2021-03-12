import QtQuick 2.0
//import QtQuick.Window 2.2
import "../js/style.js" as Style
//Window {
//    visible: true

Rectangle {
    id: root

    property string message: "Error! This is a long message with details"

    width: 100
    height: 40

    color: Style.backgroundColor
    border.color: Style.penColor
    border.width: Style.borderWidth
    radius: Style.borderRadius

    visible: true

    function show(text) {
        root.message = text;
        root.visible = true;
    }

    function hide() {
        root.visible = false;
    }


    Text {
        id: messageText
        anchors.top: parent.top
        anchors.topMargin: Style.baseMargin
        anchors.left: parent.left
        anchors.right: parent.right
        horizontalAlignment: Text.AlignHCenter
        wrapMode: "WordWrap"
        text: root.message
        font.pixelSize: Style.textPixelSize
        color: Style.penColor
        onPaintedHeightChanged: {
            root.height = messageText.paintedHeight + okButton.height + 3 * Style.baseMargin;

        }

    }


    Button {
        id: okButton
        text: qsTr("OK")
        anchors.top: messageText.bottom
        anchors.topMargin: Style.baseMargin
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: root.hide()
    }

}

//}
