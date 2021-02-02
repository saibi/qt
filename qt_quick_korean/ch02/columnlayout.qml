import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3


ApplicationWindow {
    width: 400
    height: 300
    visible: true

    ColumnLayout {
        id: layout
//        anchors.fill: parent
        spacing: 2

        Rectangle {
            color: 'red'

            Layout.alignment: Qt.AlignCenter

            Layout.preferredWidth: 40
            Layout.preferredHeight: 40
        }

        Rectangle {
            color: 'green'

            Layout.alignment: Qt.AlignRight

            Layout.preferredWidth: 40
            Layout.preferredHeight: 70
        }

        Rectangle {
            color: 'blue'
            Layout.alignment: Qt.AlignBottom

            Layout.fillHeight: true
            Layout.preferredWidth: 70
            Layout.preferredHeight: 40
        }
    }
}
