import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Extras 2.0
import Qt3D.Input 2.0

import QtQuick 2.6 as QQ2

Entity {
    id: root

    property alias gameRoot: root
    property alias timerInterval: timer.interval
    property int initialTimeInterval: 80
    property int initialSnakeSize: 5
    property string state: ""



    Camera {
        id: camera

        property real x: 24.5
        property real y: 14.0

        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 45
        aspectRatio: 16/9
        nearPlane: 0.1
        farPlane: 1000.0
        position: Qt.vector3d( x, y, 33.0)
        upVector: Qt.vector3d(0.0, 1.0, 0.0 )
        viewCenter: Qt.vector3d( x, y, 0.0)
    }

    RenderSettings {
        id: frameGraph
        activeFrameGraph: ForwardRenderer {
            clearColor: Qt.rgba(0, 0, 0, 1)
            camera: camera
        }
    }

    KeyboardDevice {
        id: keyboardController
    }

    InputSettings {
        id: inputSettings
    }

    KeyboardHandler {
        id: input
        sourceDevice: keyboardController
        focus: true
        onPressed: { }
    }

    QQ2.Component.onCompleted: {
        console.log("Start game...");
        timer.start();
    }

    QQ2.Timer {
        id: timer
        interval: initialTimeInterval
        repeat: true
        onTriggered: { }
    }


    components: [frameGraph, input]
}
