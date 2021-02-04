import QtQuick 2.2
import QtQuick.Window 2.3
import MyTimer 1.0


Window {
    width: 300
    height: 200
    visible: true


    property int timerCnt: 0

    Image {
        id: loadImage
        source: "images/loading.png"
        width: 100
        height: 100

        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

    }

    PropertyAnimation {
        id: loadAni
        target: loadImage
        property: "rotation"
        from: 0
        to: 360
        duration: 2000
        loops: Animation.Infinite

        running: false
    }

    Text {
        text: timer.active ? qsTr("타이머 동작 중") : qsTr("타이머 정지 상태")
        font.pixelSize: 24
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: loadImage.bottom
        anchors.topMargin: 30

    }

    MyTimer {
        id: timer
        interval: 1000
        onTimeout: {
            console.log("Timer Call :", timerCnt++)
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if ( timer.active == false ) {
                console.log("Timer start")
                timer.start()
                loadAni.start()
            } else {
                console.log("Timer stop")
                timer.stop()
                loadAni.stop()
            }
        }
    }
}
