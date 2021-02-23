import QtQuick 2.9
import QtQuick.Controls 1.3
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.1


ApplicationWindow {

    width: 220
    height: 250
    visible: true


    Rectangle {
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        color: "#494d53"

        ListModel {
            id: listModel
            ListElement { foo: "A1"; bar: "B1"; baz: "C1" }
            ListElement { foo: "A2"; bar: "B2"; baz: "C2" }
            ListElement { foo: "A3"; bar: "B3"; baz: "C3" }
        }

        Tumbler {
            anchors.centerIn: parent

            TumblerColumn { model: listModel; role: "foo" }
            TumblerColumn { model: listModel; role: "bar" }
            TumblerColumn { model: listModel; role: "baz" }
        }
    }

}
