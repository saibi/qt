import QtQuick 2.9
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3


TableSurface {

    ComicPage {
        id: pageFace
        anchors.centerIn: parent
        panelsRepeater.delegate: ComicPanel {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        panelsRepeater.model: 2
    }

    DropShadow {
        source: pageFace
        anchors.fill: source
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        color: "#80000000"
    }

}
