import QtQml 2.2

QtObject {
	id: myQmlObject
	readonly property real myNumber: {
		return Math.random() + 1
	}

	property int myNumber2: myNumber + 1
	property var myChildObject: QtObject {}
	signal done(string message)
	onDone: {
		print(message);
		doSomething();
	}

	function doSomething() {
		print("Hello again!");
	}

	Component.onCompleted: {
		console.log("Hello World! %1 %2".arg(myNumber).arg(myNumber2))
		done("I'm done!")
	}
}
