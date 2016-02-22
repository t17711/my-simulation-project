import QtQuick 2.0

Rectangle {
    width: 360
    height: 360

    Component {
        id: cityDelegate

        Column {
            height: 50

            Text {
                id: cityText
                text: city + " (" + country + ")"
            }

            Text {
                text: "Population: " + population + ", area: " + area
            }
        }
    }

    ListView {
        model: _model
        delegate: cityDelegate

        anchors.fill: parent
        section.property: "country"
        section.delegate: Rectangle {
            height: 20
            width: parent.width
            color: "red"

            Text {
                text: section
            }
        }
    }
}
