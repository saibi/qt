import QtQuick 2.9
import QtQuick.Window 2.3


Window {
    visible: true;
    width: animation.width
    height: animation.height + 8

    AnimatedImage {
        id: animation
        source: "images/ani.gif"
    }

    Rectangle {
        property int frames: animation.frameCount

        width: 4; height: 8

        x: (animation.width - width) * animation.currentFrame / frames
        y: animation.height
        color: "red"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {

            if (animation.paused == true ) {
                animation.paused = false
                console.log("clicked : resume")
            } else {
                animation.paused = true
                console.log("clicked : pause")
            }
        }
    }
}
