import QtQuick

Item {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    //title: qsTr("Hello World")
    Image {
        id: img
        anchors.fill: parent
        height: 480
        width: 640
    }

    function updateImage()
    {
        console.log("UPDATE");
        img.source = "image://imageholder/row?" + Math.random()
    }

}
