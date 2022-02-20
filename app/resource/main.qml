import QtQuick
import QtQuick.Controls
//import info

Item {
    id: mainWindow
    width: 512
    height: 512
    visible: true

    signal processorTypeChanged(int type)

    Image {
        id: img
        anchors.left: parent.left
        anchors.top: parent.top
        height: 480
        width: 480
    }

    ComboBox {
        id:typesBox
        anchors.top: img.bottom
        anchors.left: parent.left
        textRole: "text"
                valueRole: "value"
        model: ListModel{
            id: model
        }
        onCurrentValueChanged: {
            processorTypeChanged(currentValue)
        }

    }

    function updateImage() {
        img.source = "image://imageholder/row?" + Math.random()
    }
    function addProcessorType(newType: string) {
        console.log(typesBox.model.count)
        typesBox.model.append({value: typesBox.model.count, text: newType})
    }

}
