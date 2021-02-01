import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true; width: 400; height: 400
    Rectangle {
        FontLoader {
            id: fixedFont; name: "Courier"
        }
        FontLoader {
            id: webFont
            source: "http://www.mysite.com/myfont.ttf"
        }
        Text {
            text: "Fixed-size font";
            font.family: fixedFont.name
        }
    }
}
