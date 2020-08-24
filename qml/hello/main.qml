import QtQuick 2.0

Rectangle {
    id: page
    width: 320; height: 480
    color: "white"
    
    Text {
        id: helloText
        text: "Hello World"
        y: 30
        anchors.horizontalCenter: page.horizontalCenter
        font.pointSize: 24; font.bold: true
    }
    
    Grid {
        id: colorPicker
        x: 4; anchors.bottom: page.bottom; anchors.bottomMargin: 4
        rows: 2; columns: 3; spacing: 3
        
        Hello { cellColor: "red"; onClicked: helloText.color = cellColor}
    }
}
