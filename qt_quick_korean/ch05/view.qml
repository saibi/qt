import QtQuick 2.9

ListView {
    width:100
    height: 100
    model: MyModel

    delegate: Rectangle {
        height: 25
        width: 100
        color: model.modelData.color
        Text {
            text: name
        }
    }

}

