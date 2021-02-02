import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true
    width: 200
    height: 200

    Canvas {
        id: root
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d")

            ctx.lineWidth = 4
            ctx.strokeStyle = "blue"
            ctx.fillStyle = "steelblue"

            ctx.beginPath()
            ctx.moveTo(50, 50)
            ctx.lineTo(150, 50)
            ctx.lineTo(150, 150)
            ctx.lineTo(50, 150)
            ctx.closePath()

            ctx.fill()
            ctx.stroke()


            ctx.fillStyle = "green"
            ctx.strokeStyle = "blue"
            ctx.lineWidth = 4

            ctx.fillRect(20, 20, 80, 80)

            ctx.clearRect(30, 30, 60, 60)

            ctx.strokeRect(20, 20, 40, 40)


            var gradient = ctx.createLinearGradient(100, 0, 100, 200)
            gradient.addColorStop(0, "blue")
            gradient.addColorStop(0.5, "lightsteelblue")

            ctx.fillStyle = gradient
            ctx.fillRect(100, 0, 100, 100)

        }
    }
}
