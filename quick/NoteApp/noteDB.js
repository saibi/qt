.pragma library

//import QtQuick.LocalStorage 2.0 as Sql


function openDB() {
    print("noteDB.createDB()");
    _db = openDatabaseSync("StickyNotesDB", "1.0", "The stickynotes Database", 1000000);

    createNoteTable();
}


function createNoteTable() {
    print("noteDB.createTable()");

    _db.transaction( function(tx) {
        tx.executeSql("CREATE TABLE if NOT EXISTS note (noteId INTEGER PRIMARY KEY AUTOINCREMENT, x INTEGER, y INTEGER, noteText TEXT, makerId TEXT)");
        });
}

function clearNoteTable() {

}

function readNotesFromPage(markerId) {
    print("noteDB.readNotesFromPage() " + markerId);
    var noteItems = {};
    _db.readTransaction( function(tx) {
        var rs = tx.executeSql("SELECT FROM note WHERE markerId=? ORDER BY markerId DESC", [markerId]);
        var item;
        for ( var i = 0 ; i < rs.rows.length; ++i) {
            item = rs.rows.item(i);
            noteItems[item.noteId] = item;
        }
    });
    return noteItems;
}

function saveNotes(noteItems, markerId) {
    for ( var i = 0 ; i < noteItems.length; ++i) {
        var noteItem = noteItems[i];
        _db.transaction( function(tx) {
            ts.executeSql("INSERT INTO note (markerId, x, y, noteText) VALUES (?,?,?,?)", [markerId, noteItem.x, noteItem.y, noteItem.noteText]);
        });
    }
}

