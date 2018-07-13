import QtQuick 2.0

Image {
    id: root

    property bool active: false

    signal clicked()

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: root.clicked()
        hoverEnabled: true
    }

    states: [
        State {
            name: "hovered"
            when: mouseArea.containsMouse && !root.active
            PropertyChanges { target: root; x: 5 }
        },
        State {
            name: "selected"
            when: root.active
            PropertyChanges { target: root; x: 20 }
        }
    ]

    transitions: [
        Transition {
            to: "hovered"
            NumberAnimation { target: root; property: "x"; duration: 300 }

        },

        Transition {
            to: "selected"
            NumberAnimation { target: root; property: "x"; duration: 300 }

        },

        Transition {
            to: ""
            NumberAnimation { target: root; property: "x"; duration: 300 }
        }

    ]
}
