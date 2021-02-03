import QtQuick 2.9
import QtQuick.Window 2.3
import "module"

Window {
    visible: true
    width: 600
    height: 600
    title: qsTr("Image Viewer")

    Item {
        id: screen
        width: 600
        height: 600
        property int animDuration: 500

        RotationButton {
            id: rotationButton
            duration: screen.animDuration
            z: 100
            anchors.top: screen.top
            anchors.left: screen.left

        }

        Rectangle {
            id: background
            anchors.centerIn: parent
            color: "#555560"

            width: rotationButton.angle == 0 ? parent.width : parent.height
            height: rotationButton.angle == 0 ? parent.height : parent.width
            rotation: rotationButton.angle

            Behavior on rotation {
                RotationAnimation {
                    duration: screen.animDuration
                    easing.type: Easing.InOutQuad

                }
            }

            Item {
                id: grid
                anchors.fill: parent

                function displayPicture(path) {
                    picture.source = path
                    screen.state = "displayPicture"

                    console.log("displayPicture:", path)
                }

                Thumbnail {
                    column: 0
                    row: 0
                    image: "images/101.jpg"
                    onClicked: parent.displayPicture(image)

                }
                Thumbnail {
                    column: 1
                    row: 0
                    image: "images/102.jpg"
                    onClicked: parent.displayPicture(image)
                }

                Thumbnail {
                    column: 2
                    row: 0
                    image: "images/103.jpg"
                    onClicked: parent.displayPicture(image)
                }
                Thumbnail {
                    column: 3
                    row: 0
                    image: "images/104.jpg"
                    onClicked: parent.displayPicture(image)
                }
                Thumbnail {
                    column: 0
                    row: 1
                    image: "images/105.jpg"
                    onClicked: parent.displayPicture(image)
                }
                Thumbnail {
                    column: 1
                    row: 1
                    image: "images/106.jpg"
                    onClicked: parent.displayPicture(image)
                }
                Thumbnail {
                    column: 2
                    row: 1
                    image: "images/107.jpg"
                    onClicked: parent.displayPicture(image)
                }
                Thumbnail {
                    column: 3
                    row: 1
                    image: "images/108.jpg"
                    onClicked: parent.displayPicture(image)
                }
                Thumbnail {
                    column: 0
                    row: 2
                    image: "images/109.jpg"
                    onClicked: parent.displayPicture(image)
                }
                Thumbnail {
                    column: 1
                    row: 2
                    image: "images/110.jpg"
                    onClicked: parent.displayPicture(image)
                }
                Thumbnail {
                    column: 2
                    row: 2
                    image: "images/111.jpg"
                    onClicked: parent.displayPicture(image)
                }
            }

            Image {
                id: picture
                z: 2
                x: 2 * parent.width
                y: 0

                width: parent.width;
                height: parent.height

                smooth: true
                fillMode: Image.PreserveAspectFit
            }

            Button {
                id: backButton

                width: 150
                height: 32

                x: parent.width - width - 30
                y: parent.height + 3 * height
                z: 5

                text: "Back"
                onClicked: screen.state = "displayGrid"
                visible: false
            }
        }

        state: "displayGrid"

        states: [
            State {
                name: "displayGrid"
                PropertyChanges { target: background; color: "#555560" }

            },
            State {
                name: "displayPictures"
                PropertyChanges { target: background; color: "black" }

            }

        ]

        transitions: [
            Transition {
                from: "displayGrid"
                to: "displayPicture"

                PropertyAnimation {
                    target: backButton
                    properties: "visible"
                    to: true
                }
                NumberAnimation {
                    target: grid
                    properties: "scale"
                    to: 0.5
                }

                NumberAnimation {
                    target: grid
                    property: "opacity"
                    to: 0.0
                    duration: screen.animDuration
                    easing.type: Easing.InOutQuad

                }

                NumberAnimation {
                    target: picture
                    property: "x"
                    to: 0
                    duration: screen.animDuration
                    easing.type: Easing.InOutQuad
                }

                NumberAnimation {
                    target: backButton
                    property: "y"
                    to: background.height - backButton.height - 20
                    duration: screen.animDuration * 2
                    easing.type: Easing.OutBounce
                }

            },
            Transition {
                from: "displayPicture"
                to: "displayGrid"
                SequentialAnimation {
                    ParallelAnimation {
                        NumberAnimation {
                            target: picture
                            property: "x"
                            to: 2 * screen.width
                            easing.type: Easing.InOutQuad
                            duration: screen.animDuration
                        }
                        NumberAnimation {
                            target: backButton
                            property: "y"
                            to: background.height + 3 * backButton.height
                            duration: screen.animDuration
                            easing.type: Easing.InOutQuad
                        }
                    }

                    ParallelAnimation {
                        NumberAnimation {
                            target: grid
                            property: "scale"
                            to: 1.0
                            duration: screen.animDuration
                            easing.type: Easing.InOutQuad
                        }


                        NumberAnimation {
                            target: grid
                            property: "opacity"
                            to: 1.0
                            duration: screen.animDuration
                            easing.type: Easing.InOutQuad
                        }
                    }
                }

            }

        ]
    }
}
