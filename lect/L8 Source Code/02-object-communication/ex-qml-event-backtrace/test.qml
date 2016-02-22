import QtQuick 2.0

Rectangle {
    width: 100
    height: 62

    Text {
        anchors.centerIn: parent
        text: "Press Me"

        MouseArea {
            anchors.fill: parent
            onClicked: console.log("Hello World")
        }
    }
}
