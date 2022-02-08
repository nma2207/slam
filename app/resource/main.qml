import QtQuick
import QtQuick.Controls
//import info

Item {
    id: mainWindow
    width: 640
    height: 550
    visible: true

    signal processorTypeChanged(int type)

    Image {
        id: img
        anchors.left: parent.left
        anchors.top: parent.top
        height: 480
        width: 640
    }

    ComboBox {
        anchors.top: img.bottom
        anchors.left: parent.leftor
        textRole: "text"
        valueRole: "value"
        Component.onCompleted: currentIndex = indexOfValue(0)
        model: ListModel{
            id: model
            ListElement{ value: 0; text: qsTr("None") }
            ListElement{ value: 1; text: qsTr("OrbExtractor") }
            ListElement{ value: 2; text: qsTr("Yolo detector") }
        }
        onCurrentValueChanged: {
            processorTypeChanged(currentValue)
        }
    }

    function updateImage()
    {
        img.source = "image://imageholder/row?" + Math.random()
    }

}
