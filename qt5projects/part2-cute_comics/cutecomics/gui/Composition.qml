import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Extras 2.0
import Qt3D.Input 2.0

Entity {
    id: composition

    property alias elements: elements
    property string selectedElement
    property bool moveCamera: true
    property string backgroundColor: "green"

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
        enabled: moveCamera
    }

    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                id: rendered
                camera: camera
                clearColor: composition.backgroundColor
            }
        },
        InputSettings {} ,
        DirectionalLight {
            worldDirection: Qt.vector3d(0, -2.0, -5.0)
            color: "#fff"
            intensity: 1
        }

    ]

    Entity {
        id: elements
    }
}
