import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.XmlListModel 2.0

Window {
    visible: true
    width: 500
    height: 800
    title: qsTr("Window")

    function f2C (tempInF) {
        return (5/9 * (tempInF - 32)).toFixed(0)
    }

    Item {
        id: root

        width: parent.width
        height: parent.height

        property string baseURL: "http://localhost:8080"
        property string dataURL: "/weather.xml"

        XmlListModel {
            id: weatherModelCurrent
            source: "http://localhost:8080/weather.xml"
            query: "/xml_api_reply/weather/current_conditions"

            XmlRole { name: "condition"; query: "condition/@data/string()" }
            XmlRole { name: "temp_f"; query: "temp_f/@data/string()" }
            XmlRole { name: "humidity"; query: "humidity/@data/string()" }
            XmlRole { name: "icon_url"; query: "icon/@data/string()" }
            XmlRole { name: "wind_condition"; query: "wind_condition/@data/string()" }

        }

        Component {
            id: currentConditionDelegate
            Column {
                Text { text: qsTr("Today"); font.bold: true }
                Text { text: model.condition }
                Image { source: root.baseURL + model.icon_url }
                Text { text: model.temp_f + " F° / " + f2C(model.temp_f) + " C°" }
                Text { text: model.humidity }
                Text { text: model.wind_condition }
            }
        }


        XmlListModel {
            id: weatherModelForecast
            source: root.baseURL + root.dataURL
            query: "/xml_api_reply/weather/forecast_conditions"

            XmlRole { name: "day_of_week"; query: "day_of_week/@data/string()" }
            XmlRole { name: "low"; query: "low/@data/string()" }
            XmlRole { name: "high"; query: "high/@data/string()" }
            XmlRole { name: "icon_url"; query: "icon/@data/string()" }
            XmlRole { name: "condition"; query: "condition/@data/string()" }

        }

        Component {
            id: forecastConditionDelegate
            Column {
                Text { text: model.day_of_week; font.bold: true }
                Text { text: model.condition }
                Image { source: root.baseURL + model.icon_url }
                Text { text: qsTr("Lows: ") + model.low + " F° / " + f2C(model.low) + " C°" }
                Text { text: qsTr("Highs: ") + model.high + " F° / " + f2C(model.high) + " C°" }
            }
        }


        Column {
            id: allWeather
            anchors.centerIn: parent
            anchors.margins: 10
            spacing: 10

            Repeater {
                id: currentReportList
                model: weatherModelCurrent
                delegate: currentConditionDelegate
            }

            Repeater {
                id: forecastReportList
                model: weatherModelForecast
                delegate: forecastConditionDelegate
            }

//            GridView {
//                id: forecastReportList
//                width: 400
//                height: 400
//                cellWidth: 200; cellHeight: 200
//                model: weatherModelForecast
//                delegate: forecastConditionDelegate
//            }
        }

//        ListView {
//            width: parent.width
//            height: parent.height
//            model: weatherModelCurrent
//            delegate: currentConditionDelegate
//        }

        ListView {

            width: 150; height: 50
            //model: ["one", "two", "three", "four", "five"]
            model : 2
            delegate: Text { text: "Index: " + model.index + ", Data: " + model.modelData }
        }

        Column {
            Repeater {
                model: ["one", "two", "three", "four", "five"]
                Text { text: "Index: " + model.index + ", Data: " + model.modelData }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: Qt.quit()
        }
    }

}
