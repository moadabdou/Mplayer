import QtQuick
import QtQuick.Effects

Item{
    id : root

    required property string txt
    property string path : "../../../res/icons/audiowave.png"
    property real size : 16
    property real iconSize : 30

    height : children[0].implicitHeight
    width : children[0].implicitWidth

    Row{
        spacing : 15
        anchors.verticalCenter : parent.verticalCenter
        
        Text{
            id : itemTitle
            color : "#fff"
            anchors.verticalCenter : parent.verticalCenter
            font{
                bold : true
                pixelSize : root.size
            }
            text: root.txt
        }
        Image {
            id : waveicon
            source : root.path
            visible : false
        }
        MultiEffect{
            source: waveicon
            anchors.verticalCenter : parent.verticalCenter
            brightness : 1.0
            colorization : 1.0 
            colorizationColor :  "#EF4369"
            width : root.iconSize
            height: root.iconSize
        }
        
    }

}