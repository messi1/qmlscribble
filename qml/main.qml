import QtQuick 2.1
import QtQuick.Dialogs 1.0
import CustomComponents 1.0

Rectangle {
    id: mainWindow
    width: 640; height: 480

    FileDialog {
        id: fileDialog
        visible: false
        modality:  Qt.NonModal
        title: "Choose a file"
        selectExisting: true
        selectMultiple: false
        selectFolder: false
        nameFilters: [ "Image files (*.png *.jpg)", "All files (*)" ]
        selectedNameFilter: "Image files (*.png *.jpg)"
        onAccepted: { myDrawingArea.openImage(fileUrl) }
        folder: Qt.resolvedUrl(Qt.installPath)

    }

    MouseArea {
        anchors.fill: parent

        onPressed: {
            if (mouse.button == Qt.LeftButton)
                        myDrawingArea.mousePressEvent(Qt.point(mouseX, mouseY))
        }

        onPositionChanged: {
            if (pressedButtons == Qt.LeftButton)
                               myDrawingArea.mouseMoveEvent(Qt.point(mouseX, mouseY))
        }

        onReleased: {
            if (mouse.button == Qt.LeftButton)
                        myDrawingArea.mouseReleaseEvent(Qt.point(mouseX, mouseY))
        }
    }

    DrawArea  {
        id: myDrawingArea
        anchors.fill: parent
        focus: true

        Keys.onPressed: {
            if (event.key >= Qt.Key_1 || event.key <= Qt.Key_9) {
                myDrawingArea.PenWidth = parseInt(event.text)
                event.accepted = true
            }
        }
    }

    Component.onCompleted: myDrawingArea.setPenWidth(3)

Flow {
    id: colorPicker
    width: ((parent.width*4/5)>512)? 512 :  (parent.width*4/5)
    anchors { left: parent.left; leftMargin: 5; bottom: parent.bottom; bottomMargin: 5 }
    spacing: 5
    property int currentItem: 0

    Cell { cellColor: "lightgreen";      onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=0;  unSelectCell() } }
    Cell { cellColor: "greenyellow";     onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=1;  unSelectCell() } }
    Cell { cellColor: "lime";            onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=2;  unSelectCell() } }
    Cell { cellColor: "darkgreen";       onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=3;  unSelectCell() } }
    Cell { cellColor: "lightblue";       onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=4;  unSelectCell() } }
    Cell { cellColor: "steelblue";       onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=5;  unSelectCell() } }
    Cell { cellColor: "blue";            onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=6;  unSelectCell() } }
    Cell { cellColor: "darkblue";        onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=7;  unSelectCell() } }
    Cell { cellColor: "black";           onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=8;  unSelectCell() } }
    Cell { cellColor: "yellow";          onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=9;  unSelectCell() } }
    Cell { cellColor: "gold";            onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=10; unSelectCell() } }
    Cell { cellColor: "orange";          onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=11; unSelectCell() } }
    Cell { cellColor: "darkorange";      onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=12; unSelectCell() } }
    Cell { cellColor: "red";             onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=13; unSelectCell() } }
    Cell { cellColor: "maroon";          onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=14; unSelectCell() } }
    Cell { cellColor: "deeppink";        onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=15; unSelectCell() } }
    Cell { cellColor: "blueviolet";      onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=16; unSelectCell() } }
    Cell { cellColor: "purple";          onClicked: { myDrawingArea.PenColor = cellColor; colorPicker.currentItem=17; unSelectCell() } }
}

    function unSelectCell() {
        for(var i = 0; i < colorPicker.children.length ; ++i) {
            if(i != colorPicker.currentItem)
                colorPicker.children[i].cellBorder.width = 1
        }
    }

    Column {
        id: menu
        property string fileName: "image_"+Qt.formatDateTime(new Date(), "hh.mm_yy.MM.dd")+".jpg"
        property string fileFormat: "JPG"
        spacing: 5
        anchors { top: parent.top; topMargin: 5; right: parent.right; rightMargin: 5 }

        Button {
            id: loadButton
            btnText: qsTr("Load")
            onClicked: fileDialog.open()
        }

        Button {
            id: saveButton
            btnText: qsTr("Save")
            onClicked: myDrawingArea.saveImage(menu.fileName, menu.fileFormat)
        }

        Button {
            id: clearButton
            btnText: qsTr("Clear")
            onClicked: myDrawingArea.clearImage()
        }
    }


}


