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

    Image
    {
        id: icon
        source: "qrc:///images/map-marker-icon.png"
        sourceSize.width: 50
        sourceSize.height: 50
    }

    MapQuickItem
    {
        id: marker
        anchorPoint.x: marker.width / 4
        anchorPoint.y: marker.height
        coordinate: QtPositioning.coordinate(37.399061, 126.965953);

        sourceItem: icon
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
            map.addMapItem(marker)
        }
    }
}
