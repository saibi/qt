import QtQuick 2.12
import QtQuick.Controls 1.3
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.1

ApplicationWindow {
    width: 200
    height: 200
    visible: true

    DelayButton
    {
        id: myDelay
        delay: 3000
        text: progress < 1 ? "START" : "STOP"
        anchors.centerIn: parent

        onActivated: {
            console.log("DelayButton Actived (complete)");
        }
    }
}
