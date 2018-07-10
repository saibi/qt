
import QtQuick 2.0

Item {
	width: 310; height: 170
	
	Column {
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.verticalCenter: parent.verticalCenter

		spacing: 5

		Rectangle {
			color: "lightblue"
			radius: 10.0
			width: 100; height: 50

			Text {
				anchors.centerIn: parent
				font.pointSize: 24
				text: "Books"
			}
		}


		Rectangle {
			color: "gold"
			radius: 10.0
			width: 300; height: 50

			Text {
				anchors.centerIn: parent
				font.pointSize: 24
				text: "Music"
			}
		}

		Rectangle {
			color: "lightgreen"
			radius: 10.0
			width: 300; height: 30

			Text {
				anchors.centerIn: parent
				font.pointSize: 14
				text: "Movies"
			}
		}
	}
}

