import QtQuick 2.9
import QtQuick.Window 2.3


Window {

    visible: true;
    width: 360
    height: 360

    SquareButton
    {
        width: 360
        height: 360

        onActivated: console.log("Activated at " + xPosition + "," + yPosition)
        onDeactivated: console.log("Deactivated!")
    }
}
