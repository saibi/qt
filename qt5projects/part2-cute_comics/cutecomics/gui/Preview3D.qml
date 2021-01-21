import QtQuick.Scene3D 2.0
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Extras 2.0
import Qt3D.Input 2.0

Scene3D {
    id: scene3d
    aspects: ['input', 'logic']

    width: 800
    height: 800

    Entity {
        id: sceneRoot

        Element {
            shape: "horizontal"
            translation: Qt.vector3d( 0.0, 0.0, 0.0)
            selected: true
        }

        Camera {
            id: camera
            projectionType: CameraLens.PerspectiveProjection
            fieldOfView: 45
            nearPlane : 0.1
            farPlane: 1000.0
            position: Qt.vector3d( 5.0, 5.0, 5.0)
            upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
            viewCenter: Qt.vector3d( 0.0, 0.0, 0.0)

        }

        FirstPersonCameraController {
            id: cameraController
            camera: camera
        }

        components: [
            RenderSettings {
                activeFrameGraph: ForwardRenderer {
                    id: rendered
                    camera: camera
                }
            } ,

            InputSettings {}

        ]
    }
}
