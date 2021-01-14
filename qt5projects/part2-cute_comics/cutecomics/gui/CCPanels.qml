import QtQuick 2.9
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3
import QtMultimedia 5.9


TableSurface {
    ListModel {
        id: panelsEntity
        property int currentIndex: -1
    }

    ComicPage {
        id: pageFace
        anchors.centerIn: parent
        panelsRepeater.delegate: ComicPanel {
            displayText: pid || 0
            imageSource: pictureSource || ""

            Layout.fillWidth: true
            Layout.fillHeight: true

            onClicked: {
                panelsEntity.currentIndex = index;
                camera.imageCapture.capture();
            }
        }
        panelsRepeater.model: panelsEntity
        panelAdder.onClicked: panelsEntity.append( {"pid": panelsEntity.count, "pictureSource" : ""} )

        panelRemover.onClicked: if (panelsEntity.count > 0 ) {
                                    panelsEntity.remove(panelsEntity.count - 1)
                                }

    }

    Camera {
        id: camera
        imageCapture {
            onImageCaptured:
                panelsEntity.get(panelsEntity.currentIndex).pictureSource = preview
        }
    }

    VideoOutput {
        source: camera
        width: 120
        height: 75

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 16
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
