import QtQuick
import QtQuick.Layouts
import Mplayer 1.0

Item {
    id : app
    anchors.fill: parent

    Item{
        anchors.fill : parent
        id :  innerContent
        RowLayout {
            anchors.fill :  parent
            spacing : 0
            Item{
                Layout.fillWidth : true
                Layout.minimumWidth : 900
                Layout.preferredHeight : parent.height
                MainSection{}
            }
            Item{
                Layout.minimumWidth :  400
                Layout.maximumWidth : 400
                Layout.preferredHeight : parent.height
                CurrentPlaying{} 
            }
        }

        SideBar{}
    }
    MouseArea {
        id: screenEdgeDetector
        width: parent.width
        height: 60
        anchors.bottom: parent.bottom
        hoverEnabled: true

        onEntered: {
            player.anchors.bottomMargin = 0;
        }
    }

    PlayControler{
        id: player
        anchors.bottom : parent.bottom
        width : parent.width
        inner :  innerContent  
        anchors.bottomMargin :  -80
        Behavior on anchors.bottomMargin{
            NumberAnimation{
                duration : 300
            }
        }
        

    }


    TapHandler {
        onTapped: {
            app.forceActiveFocus()
        }
    }
}
