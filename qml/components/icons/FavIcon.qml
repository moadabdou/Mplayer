import QtQuick 2.0
import QtQuick.Effects

Rectangle{
    id : root
    required property real size 
    required property bool isfav

    width : size
    height : size
    radius : size / 2
    border{
        color:  "#EF4369"
        width : .5
    }

    color : "#0f081b"

    Image {
        id :  icon
        source: root.isfav ? "../../../res/icons/heart.png" : "../../../res/icons/heart_empty.png"
        visible: false
    }

    MultiEffect{
        source: icon
        anchors.centerIn : parent
        anchors.verticalCenterOffset:2
        width :  root.size * 0.65
        height : root.size * 0.65
        colorization:1.0
        brightness: 1.0
        colorizationColor :  "#EF4369"
    }

    MouseArea{
        anchors.fill:parent
        cursorShape: Qt.PointingHandCursor
    }

}