import QtQuick 2.0
import QtQuick.Effects

Item{
    id : root
    required property real size 
    required property string color
    required property string icon

    width : size
    height : size

    Image {
        id :  icon
        source: root.icon 
        visible: false
    }

    MultiEffect{
        source: icon
        anchors.centerIn : parent
        width :  root.size * 0.65
        height : root.size * 0.65
        colorization:1.0
        brightness: 1.0
        colorizationColor :  root.color
    }

}