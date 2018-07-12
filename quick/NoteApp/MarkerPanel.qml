import QtQuick 2.0

Item {
    id: root
    width: 150
    height: 450

    property string activeMarker: "personal"

    property variant markerData: [
        { markerid: "personal", img: "images/personalmarker.png" },
        { markerid: "fun", img: "images/funmarker.png" },
        { markerid: "work", img: "images/workmarker.png" }
    ]

    Column {
        id: layout
        anchors.fill: parent
        spacing: 5

        Repeater {
            model: markerData
            delegate:
                Marker {
                    id : marker
                    source: modelData.img
                    onClicked: root.activeMarker = modelData.markerid
                }
        }
    }
}
