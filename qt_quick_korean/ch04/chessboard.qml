import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    id: root
    width: 445
    height: 445

    onWidthChanged: height = width
    onHeightChanged: width = height

    visible: true
    color: "brown"

    property int cellSpace: 5
    property int cellWidth: (root.width - cellSpace*9) / 8

    Grid {
        id: grid
        anchors.fill: parent
        anchors.margins: cellSpace

        rows: 8
        columns: 8
        spacing: root.cellSpace

        Repeater {
            model: parent.rows * parent.columns
            Rectangle {
                width: root.cellWidth
                height: root.cellWidth
                color: {
                    var row = Math.floor(index / 8 )
                    var column = index % 8
                    console.log("row, column : " + row + "," + column)
                    if ( ( row + column) % 2 == 1)
                        "black"
                    else
                        "white"

                }
            }

        }
    }
}
