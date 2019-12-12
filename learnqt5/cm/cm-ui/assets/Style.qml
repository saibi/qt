pragma Singleton

import QtQuick 2.9

Item {
    property alias fontAwesome: fontAwesomeLoader.name
    readonly property color colourBackground: "#f4c842"

    FontLoader {
        id: fontAwesomeLoader
        source: "qrc:/assets/fontawesome-webfont.ttf"
    }
}
