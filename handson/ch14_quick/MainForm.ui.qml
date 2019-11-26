import QtQuick 2.4

Item {
    id: loginForm
    width: 400
    height: 400
    property alias mouseArea_login: mouseArea_login
    property alias textInput_password: textInput_password
    property alias mouseArea_password: mouseArea_password
    property alias text_password: text_password
    property alias textInput_email: textInput_email
    property alias text_email: text_email
    property alias mouseArea_email: mouseArea_email

    Image {
        id: background
        anchors.fill: parent
        source: "qrc:/images/background.jpg"

        Rectangle {
            id: loginRect
            x: -1
            y: 145
            width: 402
            height: 210
            color: "#fcf9f4"
            border.color: "#efedeb"
            anchors.verticalCenterOffset: 50
            anchors.horizontalCenterOffset: 0
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            Image {
                id: image
                x: 144
                width: 240
                height: 100
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: -110
                source: "qrc:/images/logo_sample.png"
            }

            Rectangle {
                id: passwordRect
                y: 108
                height: 30
                color: "#ffffff"
                border.color: "#efedeb"
                anchors.verticalCenterOffset: -18
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 25
                anchors.left: parent.left
                anchors.leftMargin: 25

                TextInput {
                    id: textInput_password
                    y: 10
                    height: 20
                    echoMode: TextInput.Password
                    anchors.verticalCenterOffset: 3
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 40
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    font.pixelSize: 12
                }

                Image {
                    id: image_password
                    x: 334
                    y: 8
                    width: 20
                    height: 20
                    fillMode: Image.PreserveAspectFit
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    source: "qrc:/images/password-icon.png"
                }

                MouseArea {
                    id: mouseArea_password
                    anchors.fill: parent
                }

                Text {
                    id: text_password
                    y: 9
                    color: "#cbbdbd"
                    text: qsTr("Password")
                    anchors.verticalCenterOffset: -1
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 40
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    font.pixelSize: 12
                }
            }

            Rectangle {
                id: emailRect
                y: 19
                height: 30
                color: "#ffffff"
                border.color: "#efedeb"
                anchors.verticalCenterOffset: -55
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 25
                anchors.left: parent.left
                anchors.leftMargin: 25

                TextInput {
                    id: textInput_email
                    y: 10
                    height: 20
                    selectionColor: "#4f0080"
                    anchors.verticalCenterOffset: 3
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 40
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    font.pixelSize: 12
                }

                Image {
                    id: image_email
                    x: 325
                    y: 8
                    width: 20
                    height: 20
                    fillMode: Image.PreserveAspectFit
                    visible: true
                    clip: false
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    source: "qrc:/images/email-icon.png"
                }

                MouseArea {
                    id: mouseArea_email
                    x: 20
                    y: 8
                    anchors.fill: parent
                }

                Text {
                    id: text_email
                    y: 9
                    color: "#cbbdbd"
                    text: qsTr("E-Mail")
                    anchors.verticalCenterOffset: -1
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 40
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    font.pixelSize: 12
                }
            }

            Rectangle {
                id: loginButton
                y: 155
                height: 30
                color: "#27ae61"
                radius: 2
                border.width: 0
                anchors.verticalCenterOffset: 35
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 25
                anchors.left: parent.left
                anchors.leftMargin: 25

                MouseArea {
                    id: mouseArea_login
                    anchors.fill: parent
                }

                Text {
                    id: text_login
                    x: 67
                    y: 9
                    text: qsTr("LOGIN")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.top: parent.top
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 12
                }
            }
        }
    }
}
