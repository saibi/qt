import QtQuick 2.0
import QtLocation 5.3
import QtPositioning 5.0

Item {
    id: window

    Plugin
    {
        id: mapPlugin
        name: "osm"
    }

    function addMarker(latitude, longitude)
    {
        var component = Qt.createComponent("qrc:///qml/marker.qml")
        var item = component.createObject(window, { coordinate: QtPositioning.coordinate(latitude, longitude) } )
        map.addMapItem(item)
    }

    Map
    {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(37.399061, 126.965953)
        zoomLevel: 14

        Component.onCompleted:
        {
            addMarker(37.399061, 126.965953)
            addMarker(37.399712, 126.965669)
            addMarker(37.398685, 126.964541)
        }
    }
}
