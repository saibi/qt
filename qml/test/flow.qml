import QtQuick 2.0

Rectangle {
	color: "lightblue"
	width: 300; height: 200

	Flow {
		anchors.fill: parent
		anchors.margins: 4
		spacing: 10

		Text { text: "Text"; font.pixelSize:40 }
		Text { text: "items"; font.pixelSize:40 }
		Text { text: "flowing"; font.pixelSize:50 }
		Text { text: "inside"; font.pixelSize:40 }
		Text { text: "a"; font.pixelSize:30 }
		Text { text: "Flow"; font.pixelSize:20 }
		Text { text: "item"; font.pixelSize:10 }
	}
}

