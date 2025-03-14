import QtQuick
Item{
    id:root 
    anchors.fill : parent
    Column{
        anchors.centerIn : parent 
        NormalIcon{
            id: ufoIcon
            size: Math.min(root.width*.5, root.height*.5)
            color : "#aaa"
            icon: "../../../res/icons/ufo.png"
            layer.enabled : true
            layer.samples : 4
        }
        Text{
            anchors.horizontalCenter : parent.horizontalCenter
            text : "No songs yet 🛸"
            color : "#aaa"
            font.pixelSize : ufoIcon.size *.1
        }
        
    }
}