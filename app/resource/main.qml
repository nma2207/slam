import QtQuick
import QtQuick.Controls
import info

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
        Component.onCompleted: currentIndex = indexOfValue(ProcessorType.Yolo)
        model: ListModel{
            id: model
            ListElement{ value: ProcessorType.OrbExtractor;text: qsTr("OrbExtractor") }
            ListElement{ value: ProcessorType.Yolo;text: qsTr("Yolo detector") }
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
