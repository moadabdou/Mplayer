import QtQuick 2.0
import QtQuick.Effects

Rectangle{
    id : root
    property real size :  30
    required property string path

    width : size
    height : size
    radius : size / 2
    border{
        color: "#571b52"
        width : 1.5
    }

    gradient: Gradient{
        GradientStop{
            position:0
            color : "#241445"
        }
        GradientStop{
            position:.5
            color : "#511e57"
        }
        GradientStop{
            position:1
            color : "#61225c"
        }
    }

    Image {
        id :  icon
        source: root.path
        visible: false
    }

    MultiEffect{
        source: icon
        anchors.centerIn : parent
        width :  root.size * 0.5
        height : root.size * 0.5
        colorization:1.0
        brightness: 1.0
        colorizationColor : "#ffffff"
    }

    MouseArea{
        anchors.fill:parent
        cursorShape: Qt.PointingHandCursor
    }

}