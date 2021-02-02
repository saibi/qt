import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3


import "js/jslib.js" as JUtils

Window {
    visible: true
    title: qsTr("Hello World")

    id: root

    width: 300
    height: 300

    property string val: ""

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 9

        RowLayout {

            Layout.fillWidth: true
            TextField {
                id: input1
                Layout.fillWidth: true
                focus: true
            }

            Label {
                text: " + "
            }

            TextField {
                id: input2
                Layout.fillWidth: true
                focus: true
            }

            Button {
                text: " 계산 "
                onClicked: {
                    root.val = input1.text.trim() + "," + input2.text.trim()
                    root.jsCall(val)
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                anchors.fill: parent
                color: '#333'
                opacity: 0.2
                radius: 5
                border.color: Qt.darker(color)
            }

            ScrollView {
                id: scrollView
                anchors.fill: parent
                anchors.margins: 9

                ListView {
                    id: resultView
                    model: ListModel {
                        id: outputModel
                    }

                    delegate: ColumnLayout {
                        width: ListView.view.width
                        Label {
                            Layout.fillWidth: true
                            color: 'green'
                            text: "> " + model.expression
                        }
                        Label {
                            Layout.fillWidth: true
                            color: 'blue'
                            text: "= " + model.result
                        }
                        Rectangle {
                            height: 1
                            Layout.fillWidth: true
                            color: '#333'
                            opacity: 0.2
                        }
                    }
                }
            }

        }
    }

    function jsCall(exp) {
        var data = JUtils.addCall(exp)
        console.log(data);
        outputModel.insert(0, data)
    }

}
