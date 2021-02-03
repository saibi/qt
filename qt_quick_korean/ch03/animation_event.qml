import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    width: 330
    height: 330
    visible: true
    Image {
        id: logo
        source: "images/qtlogo.png"
        x: 10
        y: 20

        NumberAnimation on scale {
            id: scaleAni
            from: 0.1
            to: 1.0
            duration: 2000
            running: true
            onStarted: console.log("started")
            onStopped: console.log("stopped")
        }

        onScaleChanged: {
            if ( scale > 0.5 ) {
                scaleAni.complete()
            }
            console.log("scale : " + scale)
        }
    }
}
