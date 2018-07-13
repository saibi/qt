import QtQuick 2.0

Item {
    id: root
    width: 200
    height: 200

    property string markerId
    property int nodeId
    property alias noteText: editArea.text

    NoteToolbar {
        id: toolbar
        height: 40

        anchors {
            top: root.top
            left: root.left
            right: root.right
        }

        drag.target: root

        Tool {
            id: deleteItem
            source: "images/delete.png"
            onClicked: root.destroy()
        }
    }

    TextEdit {
        id: editArea
        anchors {
            top: toolbar.bottom
            bottom: root.bottom
            left: root.left
            right: root.right
        }
        wrapMode: TextEdit.WrapAnywhere

        onPaintedHeightChanged: updateNoteHeight()
    }

    function updateNoteHeight() {
        var noteMinHeight = 200;
        var currentHeight = editArea.paintedHeight + toolbar.height + 40;
        root.height = noteMinHeight;

        if ( currentHeight >= noteMinHeight ) {
            root.height = currentHeight;
        }
    }

    Behavior on height { NumberAnimation {} }

    BorderImage {
        id: noteImage
        anchors.fill: parent

        source: "images/personal_note.png"
        border.left: 20; border.top: 20
        border.right: 20; border.bottom: 20
    }
}
