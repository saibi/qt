import QtQuick 2.0
import QtLocation 5.3


MapQuickItem
{
    id: marker
    anchorPoint.x: marker.width / 4
    anchorPoint.y: marker.height
    sourceItem: Image
    {
        id: icon
        source: "qrc:///images/map-marker-icon.png"
        sourceSize.width: 50
        sourceSize.height: 50
    }

}
