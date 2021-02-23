import QtQuick 2.9
import QtQuick.Controls 1.3
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.1


ApplicationWindow {
    width: 200
    height: 200
    visible: true

    Tumbler {
        anchors.centerIn: parent
        TumblerColumn
        {
            id: myCol1
            model: 5
            onCurrentIndexChanged: {
                console.log("cur:" + myCol1.currentIndex);
            }
        }

        TumblerColumn
        {
            model: [1, 2, 3, 4]
        }

        TumblerColumn
        {
            model: ["A", "B", "C", "D", "E"]
        }

    }

}
