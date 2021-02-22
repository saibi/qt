import QtQuick 2.12
import QtQuick.Templates 2.5 as T

T.Button {
    id: control
    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)
    leftPadding: 4
    rightPadding: 4
    background: Rectangle {
        id: buttonBackground
        implicitWidth: 100
        implicitHeight: 40
        opacity: enabled ? 1 : 0.3
        border.color: "#222222"; border.width: 1
        radius: 2
        states: [
            State {
                name: "normal"
                when: !control.down
                PropertyChanges { target: buttonBackground }
            },
            State {
                name: "down"
                when: control.down
                PropertyChanges {
                    target: buttonBackground
                    border.color: "#dddddd"
                }
            }
        ]
    }
    contentItem: Text {
        id: textItem
        text: control.text; font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: "#000000"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        states: [
            State {
                name: "normal"
                when: !control.down
            },
            State {
                name: "down"
                when: control.down
                PropertyChanges {
                    target: textItem; color: "#b2b1b1"
                }
            }
        ]
    }
}
