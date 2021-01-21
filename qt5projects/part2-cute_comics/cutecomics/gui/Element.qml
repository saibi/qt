import Qt3D.Core 2.9
import Qt3D.Extras 2.9
import Qt3D.Render 2.9

Entity {
    id: element

    property string shape: ""
    property alias translation: transform.translation
    property bool selected

    signal wasSelected(string objectName)
    signal wasDeselected(string objectName)


    CuboidMesh {
        id: cuboid
        yExtent: shape === "vertical" ? 2 : 1
        xExtent: shape === "horizontal" ? 2 : 1

    }

    PhongMaterial {
        id: phongMaterial
        ambient: selected ? Qt.rgba(255, 0, 0, 1) : Qt.rgba(0, 0, 0, 0)
    }

    Transform {
        id: transform
        translation: element.translation
    }

    ObjectPicker {
        id: picker
        onMoved: {
            element.translation = Qt.vector3d( pick.worldIntersection.x, element.translation.y, pick.worldIntersection.z )
        }
        onClicked: {
            selected = !selected
            if ( selected )
                wasSelected(element.objectName)
            else
                wasDeselected(element.objectName)
        }
        dragEnabled: selected
    }

    components: [cuboid, phongMaterial, transform, picker ]
}
