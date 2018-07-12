import QtQuick 2.0

Item {
    id: root
    width: 200
    height: 200
    //color: "#cabf1b"

    NoteToolbar {
        id: toolbar
        height: 40

        anchors {
            top: root.top
            left: root.left
            right: root.right
        }

        drag.target: root
    }

    TextEdit {
        anchors {
            top: toolbar.bottom
            bottom: root.bottom
            left: root.left
            right: root.right
        }
        wrapMode: TextEdit.WrapAnywhere
    }

    BorderImage {
        id: noteImage
        anchors.fill: parent

        source: "images/personal_note.png"
        border.left: 20; border.top: 20
        border.right: 20; border.bottom: 20
    }
}
