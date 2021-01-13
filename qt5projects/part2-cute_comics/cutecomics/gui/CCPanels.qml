import QtQuick 2.9
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3


TableSurface {
    ListModel {
        id: panelsEntity
    }

    ComicPage {
        id: pageFace
        anchors.centerIn: parent
        panelsRepeater.delegate: ComicPanel {
            displayText: pid || 0
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        panelsRepeater.model: panelsEntity
        panelAdder.onClicked: panelsEntity.append( {"pid": panelsEntity.count} )

        panelRemover.onClicked: if (panelsEntity.count > 0 ) {
                                    panelsEntity.remove(panelsEntity.count - 1)
                                }

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
