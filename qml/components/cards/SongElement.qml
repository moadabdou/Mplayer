import  QtQuick
import QtQuick.Layouts
import QtQuick.Controls 
import Mplayer 1.0

Rectangle{
    id : root
    required property real _width
    required property real _height
    required property string img
    required property string filePath
    required property string songName 
    required property string artist
    required property bool isFav

    width : _width 
    height : _height 
    radius : 10
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        onEntered: root.color = Qt.darker(root.color, 1.2)
        onExited: root.color = Qt.darker(root.color, 1 / 1.2)
    }

    Behavior on color {
        ColorAnimation {
            duration :  100
        }
    }

    RowLayout{
        width : parent.width
        height : parent.height * .9 
        anchors.centerIn : parent 
        Item{
            Layout.fillHeight : true 
            Layout.preferredWidth : parent.height
            RadiusImage{
                _width : parent.width *.8
                _height : parent.height *.8 
                anchors.centerIn : parent 
                _radius : 10
                img : root.img
            }
        }
        Item{
            Layout.fillHeight : true
            Layout.fillWidth : true  
            Column{
                anchors.verticalCenter : parent.verticalCenter
                width : parent.width
                Text{
                    elide: Text.ElideRight
                    text : root.songName
                    font.pixelSize : 16
                    width : parent.width
                }
                Text{
                    elide: Text.ElideRight
                    text : root.artist
                    font.pixelSize : 12
                    width : parent.width
                }
            }
        }
        Item{
            Layout.fillHeight : true
            Layout.preferredWidth : parent.width * .1 
            NormalIcon{ 
                icon : "../../../res/icons/dots.png"
                size :  30 
                color : "#000"
                anchors.centerIn : parent 
                MouseArea {
                    anchors.fill: parent
                    cursorShape  : Qt.PointingHandCursor
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked: mouse => {
                        contextMenu.popup()
                    }
                    onPressAndHold:mouse => {
                        if (mouse.source === Qt.MouseEventNotSynthesized)
                            contextMenu.popup()
                    }
                    Menu {
                        id: contextMenu
                        background: Rectangle {
                            color: "#1E104B" // Background color of the menu
                            anchors.fill : parent
                            radius: 4
                        }
                        width : 180
                        MenuItem {  
                            text: "play"
                            contentItem: Text {
                                text: parent.text
                                color :  "white" 
                                font.pixelSize: 16
                            }
                            background: Rectangle {
                                color: parent.highlighted ? "#EF4369" : "transparent" // Highlight color
                                radius: 4
                            } 
                            Loader {
                                anchors.fill: parent
                                sourceComponent: MouseArea {
                                    cursorShape: Qt.PointingHandCursor
                                    acceptedButtons: Qt.NoButton
                                }
                            }
                        }
                        MenuItem { 
                            text: root.isFav ? "remove from favorite" : "add to favorite"
                            contentItem: Text {
                                text: parent.text
                                color :  "white" 
                                font.pixelSize: 16
                            }
                            background: Rectangle {
                                color: parent.highlighted ? "#EF4369" : "transparent" // Highlight color
                                radius: 4
                            } 
                            onTriggered: ()=>{
                                root.isFav = !root.isFav
                                SongDB.editSong(root.filePath, "is_fav = "+ (root.isFav ? "1" : "0") )
                            }

                            Loader {
                                anchors.fill: parent
                                sourceComponent: MouseArea {
                                    cursorShape: Qt.PointingHandCursor
                                    acceptedButtons: Qt.NoButton
                                }
                            }
                        }
                        MenuItem { 
                            text: "set next" 
                            contentItem: Text {
                                text: parent.text
                                color : "white" 
                                font.pixelSize: 16
                            }
                            background: Rectangle {
                                color: parent.highlighted ? "#EF4369" : "transparent" // Highlight color
                                radius: 4
                            } 
                            Loader {
                                anchors.fill: parent
                                sourceComponent: MouseArea {
                                    cursorShape: Qt.PointingHandCursor
                                    acceptedButtons: Qt.NoButton
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}