import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    width: 200
    height: 200
    visible: true

    ListModel {
        id: fruitModel
        ListElement { name: "Apple"; cost: 2.45 }
        ListElement { name: "Orange"; cost: 3.25 }
        ListElement { name: "Banana"; cost: 1.95 }

    }

    Component {
        id: fruitDelegate
        Row {
            spacing: 10
            Text { text: name }
            Text { text: '$' + cost }
        }
    }

    ListView {
        anchors.fill: parent
        model: fruitModel
        delegate: fruitDelegate
    }
}
