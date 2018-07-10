
import QtQuick 2.0

Rectangle {
	width: 112; height: 112

	color: "#303030"

	Grid {
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.verticalCenter: parent.verticalCenter

		columns: 2
		spacing: 3 

		Rectangle { color: "white"; width: 50; height: 50 }
		Rectangle { color: "red"; width: 50; height: 50 }
		Rectangle { color: "blue"; width: 50; height: 50 }
		Rectangle { color: "yellow"; width: 20; height: 20 }
		Rectangle { color: "green"; width: 6; height: 10 }
		Rectangle { color: "black"; width: 30; height: 30 }
	}
}
