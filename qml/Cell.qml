import QtQuick 2.0

Item {
    id: container
    property alias cellColor: rectangle.color
    property alias cellBorder: rectangle.border
    signal clicked(color cellColor)

    width: 80; height: 60

    Rectangle {
        id: rectangle
        border.color: "gray"
        anchors.fill: parent
        radius: 2
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            rectangle.border.width=4
            container.clicked(container.cellColor)
        }
    }
}
