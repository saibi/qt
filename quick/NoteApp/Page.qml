import QtQuick 2.0
import "noteDB.js" as NoteDB

Item {
    id: root
    width: 600
    height: 400
    opacity: 0.0

    property string markerId
    property alias notes: container.children

    Component {
        id: noteComponent
        Note { }
    }

    Item {
        id: container
        anchors.fill: parent
    }

    function newNote() {
        newNoteObject( {"markerId": root.markerId } )
    }

    function newNoteObject(args) {
        var note = noteComponent.createObject(container, args);
        if ( note === null ) {
            console.log("note object failed to be created!");
        }
    }

    function clear() {
        for( var i = 0 ; i < container.children.length; ++i )
        {
            container.children[i].destroy();
        }
    }

    Component.onCompleted: loadNotes()

    function loadNotes() {
        var noteItems = NoteDB.readNotesFromPage(markerId);
        for ( var i in noteItems) {
            newNoteObject(noteItems[i]);
        }
    }
}
