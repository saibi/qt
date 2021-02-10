import QtQuick 2.9


Item {
    id: item

    width: 640
    height: 480

    signal qmlSignal(var msg)

    MouseArea {
        anchors.fill: parent
        onClicked: {
            item.qmlSignal(item)
        }
    }

    function readValues(anArray, anObject)
    {
        for (var i = 0 ; i < anArray.length; ++i )
            console.log("Array item:", anArray[i])

        for (var prop in anObject )
            console.log("Object item:", prop, "=", anObject[prop])
    }

    function readDate(dt)
    {
        console.log("The given date is:" , dt.toUTCString());
        return new Date();
    }
}
