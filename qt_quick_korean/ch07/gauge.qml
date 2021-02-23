import QtQuick 2.9
import QtQuick.Controls 1.3
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.1


ApplicationWindow {
    width: 100
    height: 400
    visible: true
    color: "black"

    Gauge {
        anchors.centerIn: parent
        width: 30
        height: 300
        minimumValue: 0
        value: 50
        maximumValue: 100
        tickmarkAlignment: Qt.AlignRight
    }

}
