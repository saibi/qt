import QtQuick 2.0

Rectangle {
	width: 100; height: 100
	
	onWidthChanged: console.log("rectangle width: ", width)
	onHeightChanged: console.log("rectangle height: ", height)
}
