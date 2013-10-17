import QtQuick 2.0

Rectangle
{
    property alias btnText: buttonText.text

    id: button
    width: 70
    height: 30
    color: "lightgray"
    border.width: 1
    border.color: "#4e4e4e"
    radius: 2

    signal clicked

    Text
    {
        id: buttonText
        anchors.fill: parent
        color: "#4e4e4e"
        font.pixelSize: 15
        font.bold: true
        font.family: "OpenSymbol"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea
    {
        id: mArea
        anchors.fill: parent
        onClicked: { button.clicked();}
        onEntered: { button.color = "gray";      buttonText.color = "black";    border.color = "black"}
        onExited:  { button.color = "lightgray";  buttonText.color = "#4e4e4e"; border.color = "#4e4e4e" }
    }
}
