import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true; width: 400
    height: 400

    Rectangle {
        width: parent.width
        height: parent.height // 전체 크기
        color: "gray"
        Rectangle { // 상단 Rectangle
            x: 50; y: 50 // x, y 좌표
            width: 300; height: 150
            color: "lightblue"
            Rectangle { // 상단 중첩 Rectangle
                x: 50; y: 50
                width: 50; height: 50
                color: "white"
            }
        }
        Rectangle { // 하단 Rectangle
            x: 50; y: 200
            width: 300; height: 150
            color: "green"
            Rectangle { // 하단 중첩 Rectangle
                x: 100; y: 50
                width: 150; height: 50
                color: "blue"
            }
        }
    }
}
