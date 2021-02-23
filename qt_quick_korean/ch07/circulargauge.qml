import QtQuick 2.9
import QtQuick.Controls 1.3
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.1

import QtQuick.Controls.Styles 1.4



ApplicationWindow {
    width: 500
    height: 500
    visible: true

    color: "black"

    CircularGauge
    {
        id: myCircular
        anchors.centerIn: parent

        property bool accelerating: false

        value: accelerating ? maximumValue : 0

        Keys.onSpacePressed: {
            accelerating = true
            console.log("value:" + value.toPrecision(2))

        }

        Keys.onReleased: {
            if ( event.key === Qt.Key_Space )
            {
                accelerating = false;
                event.accepted = true
            }

        }

        Component.onCompleted: forceActiveFocus()

        Behavior on value {
            NumberAnimation { duration: 1000 }
        }


        style: CircularGaugeStyle
        {
            id: style
            needle: Rectangle
            {
                y: outerRadius * 0.15
                implicitWidth: outerRadius * 0.03
                implicitHeight: outerRadius * 0.9
                antialiasing: true
                color: "yellow"
            }
            foreground: Item
            {
                Rectangle
                {
                    width: outerRadius * 0.2
                    height: width
                    radius: width / 2
                    color: "green"
                    anchors.centerIn: parent
                }
            }

            tickmarkLabel: Text {
                font.pixelSize: 15
                text: styleData.value
                color: styleData.value >= 80 ? "#e34c22" : "#e5e5e5"
                antialiasing: true
            }
        }
    }

}
