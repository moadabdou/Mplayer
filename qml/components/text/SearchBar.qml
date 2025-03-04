import QtQuick
import QtQuick.Effects
import QtQuick.Controls

Rectangle{

    property bool inputfocus :  false

    id : root
    width :  parent.width * 0.8
    height : 40
    radius : height/2
    anchors.centerIn : parent
    clip : true
    gradient: Gradient{
        GradientStop{
            position : 0
            color : "#541a7a"
        }
        GradientStop{
            position : 1
            color : "#7e107e"
        }
    }

    MouseArea{
        anchors.fill :  parent
        cursorShape: Qt.IBeamCursor
    }

    Rectangle{
        width:parent.width - 2
        height : parent.height -2 
        anchors.centerIn : parent
        color : "#0d0a1b"
        radius : width/2
    }

    Item{
        width: parent.width - 80
        height : children[0].height
        anchors.verticalCenter : parent.verticalCenter
        clip:true
        anchors{
            left : parent.left
            leftMargin :  25
        }
        TextField {
            id: searchfield
            width: parent.width
            font.pixelSize: 14
            color: "#fff"
            focus :  false
            placeholderText: "Search for a song..."
            background: Rectangle {
                color: "transparent"
            }
        }
    }

    Item{
        anchors.right : parent.right
        height : parent.height
        width : 60
        Image {
            id : searchicon
            source : "../../../res/icons/search.png"
            visible : false
        }
        MultiEffect{
            source: searchicon
            anchors.centerIn : parent
            brightness : 1.0
            colorization : 1.0 
            colorizationColor :  "#aaa"
            width : 20
            height: 20

        }
    }


}
