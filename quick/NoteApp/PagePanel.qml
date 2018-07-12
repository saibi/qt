import QtQuick 2.0

Item {
    id: root

    states: [
        State {
            name: "personal"
            PropertyChanges {
                target: personalpage
                opacity: 1.0
                restoreEntryValues: true

            }
        },
        State {
            name: "fun"
            PropertyChanges {
                target: funpage
                opacity: 1.0
                restoreEntryValues: true

            }
        },
        State {
            name: "work"
            PropertyChanges {
                target: workpage
                opacity: 1.0
                restoreEntryValues: true

            }
        }
    ]

    Page { id: personalpage; anchors.fill: parent }
    Page { id: funpage; anchors.fill: parent }
    Page { id: workpage; anchors.fill: parent }


//    BorderImage {
//        id: background

//        anchors.fill: parent
//        source: "images/page.png"

//        border.left: 68;
//        border.right: 40;
//        border.top: 69
//        border.bottom: 80
//    }
}
