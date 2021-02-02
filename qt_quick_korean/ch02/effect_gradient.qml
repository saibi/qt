import QtQuick 2.9
import QtQuick.Window 2.3
import QtGraphicalEffects 1.0

Window {
    visible: true
    width: 600
    height: 200

    Row {
        ConicalGradient {
           width: 200; height: 200
           angle: 0.0

           gradient: Gradient {
               GradientStop { position: 0.0; color: "white" }
               GradientStop { position: 1.0; color: "black" }
           }
        }

        LinearGradient {
           width: 200; height: 200
           start: Qt.point(0, 0)
           end: Qt.point(0, 300)

           gradient: Gradient {
               GradientStop { position: 0.0; color: "white" }
               GradientStop { position: 1.0; color: "black" }
           }
        }

        RadialGradient {
           width: 200; height: 200

           gradient: Gradient {
               GradientStop { position: 0.0; color: "white" }
               GradientStop { position: 1.0; color: "black" }
           }
        }
    }
}
