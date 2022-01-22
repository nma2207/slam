import QtQuick

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    Image {
        id: img
        anchors.fill: parent
        height: 480
        width: 640
        Timer {
            interval: 20
            running: true
            repeat: true
            onTriggered: {
                img.source = "image://imageholder/row?" + Math.random()
            }
        }
    }


}
