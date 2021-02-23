import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.1

ApplicationWindow
{
    width: 200
    height: 200
    visible: true

    ToggleButton {
        id: myBtn
        text: "OFF"
        anchors.centerIn: parent
        onClicked: {
            if (checked)
                myBtn.text = "ON"
            else
                myBtn.text = "OFF"
        }
    }
}
