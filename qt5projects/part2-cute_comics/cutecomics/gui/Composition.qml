import Qt3D.Core 2.0

Entity {
    id: composition

    property alias elements: elements
    property string selectedElement

    Entity {
        id: elements
    }
}
