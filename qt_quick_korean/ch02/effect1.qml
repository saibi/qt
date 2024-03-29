import QtQuick 2.9
import QtQuick.Window 2.3
import QtGraphicalEffects 1.0

Window {
    visible: true
    width: 300
    height: 300

    Item {
        anchors.fill: parent

        Image {
            id: lenna
            source: "images/lenna.png"

            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }

        Image {
            id: butterfly
            source: "images/butterfly.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }

        Image {
            id: bug
            source: "images/bug.jpg"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }

        Image {
            id: ball
            source: "images/ball.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
//        Blend {
//            anchors.fill: lenna
//            source: lenna
//            foregroundSource: butterfly
//            mode: "average"
//            cached: true
//        }

//        BrightnessContrast {
//            anchors.fill: lenna
//            source: lenna
//            brightness: 0.5
//            contrast: 0.5
//        }
//        Colorize {
//            anchors.fill: butterfly
//            source: butterfly
//            hue: 0.8
//            saturation: 1.0
//            lightness: 0.2
//        }

//        Desaturate {
//            anchors.fill: bug
//            source: bug
//            desaturation: 1.0
//        }

//        GammaAdjust {
//            anchors.fill: lenna
//            source: lenna
//            gamma: 2.0
//        }
//        HueSaturation {
//            anchors.fill: lenna
//            source: lenna
//            saturation: 0
//            lightness: 0
//        }

//        LevelAdjust {
//            anchors.fill: butterfly
//            source: butterfly

//            minimumInput: "#00000070"
//            maximumInput: "#ffffff"
//            minimumOutput: "#000000"
//            maximumOutput: "#ffffff"
//        }

//        DropShadow {
//            anchors.fill: ball

//            radius: 8.0
//            samples: 16
//            horizontalOffset: 0
//            verticalOffset:20
//            spread: 0
//            source: ball
//            color: "#aa000000"

//        }
//        FastBlur {
//            anchors.fill: lenna
//            source: lenna
//            radius: 32
//        }
//        GaussianBlur {
//            anchors.fill: lenna
//            source: lenna
//            deviation: 4
//            radius: 8
//            samples: 16
//        }
//        DirectionalBlur {
//            anchors.fill: lenna
//            source: lenna
//            angle: 90
//            length:32
//            samples: 24
//        }
//        RadialBlur {
//            anchors.fill: lenna
//            source: lenna
//            samples: 24
//            angle: 30
//        }
//        Glow {
//            anchors.fill: ball
//            source: ball
//            radius: 30
//            samples: 16
//            color: "green"
//        }

       Rectangle {
           id: rect
           color: "black"
           anchors.centerIn: parent
           width: Math.round(parent.width / 1.5)
           height: Math.round(parent.height / 2.0)
           radius: 25
       }

       RectangularGlow {
           id: effect
           anchors.fill: rect
           glowRadius: 10
           spread: 0.2
           color: "white"
           cornerRadius: rect.radius + glowRadius

       }
    }

}
