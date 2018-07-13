import QtQuick 2.0
import "noteDB.js" as NoteDB

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
            PropertyChanges {
                target: root
                currentPage: personalpage
                explicit: true
            }
        },
        State {
            name: "fun"
            PropertyChanges {
                target: funpage
                opacity: 1.0
                restoreEntryValues: true

            }
            PropertyChanges {
                target: root
                currentPage: funpage
                explicit: true
            }

        },
        State {
            name: "work"
            PropertyChanges {
                target: workpage
                opacity: 1.0
                restoreEntryValues: true

            }
            PropertyChanges {
                target: root
                currentPage: workpage
                explicit: true
            }

        }
    ]

    Page { id: personalpage; anchors.fill: parent ; markerId: "personal" }
    Page { id: funpage; anchors.fill: parent; markerId: "fun" }
    Page { id: workpage; anchors.fill: parent; markerId: "work" }


    property Page currentPage: personalpage

    Component.onDestruction: saveNotesToDB()

    function saveNotesToDB() {
        NoteDB.clearNoteTable();

        NoteDB.saveNotes(personalpage.notes, personalpage.markerId);
        NoteDB.saveNotes(funpage.notes, funpage.markerId);
        NoteDB.saveNotes(workpage.notes, workpage.markerId);
    }

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
