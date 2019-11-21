import QtQuick 2.0
import QtLocation 5.3
import QtPositioning 5.0

Item {
    Plugin
    {
        id: mapPlugin
        name: "osm"
    }

    Map
    {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(37.399027, 126.966028)
        zoomLevel: 14
    }
}
