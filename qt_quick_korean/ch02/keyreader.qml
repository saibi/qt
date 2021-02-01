import QtQuick 2.9

Item {
    focus: true
    Keys.onPressed: {
        console.log("loaded captured:", event.text);
        event.accepted = true;
    }
}
