import  QtQuick
import  QtQuick.Layouts
import  QtQuick.Effects
import  Mplayer 1.0

Rectangle{
    id : root
    property string coverImage : "../../../res/images/cover5.jpg";
    property string title : "No Songs in Queue";
    property string artist  : "select a song";
    property string album : "from album";

    Connections{
        target: QueuePlaying
        function onCurrentlyPlayingChanged() {
            let song = QueuePlaying.getCurrentSong();
            root.coverImage = song.coverImage
            root.artist = song.artist 
            root.title = song.title 
            root.album = song.album
            console.log(song.album)
        }
    }
    

    height : parent.height 
    width : parent.width 
    radius : 20
    gradient: Gradient{
        GradientStop{
            position : 0
            color : "#490e5e"
        }
        GradientStop{
            position : 1
            color :  "#ff00b3"
        }
    }

    Rectangle{

        width : parent.width - 4
        height : parent.height - 4

        anchors.centerIn : parent

        radius : parent.radius - 2

        gradient : Gradient{
            GradientStop{
                position : 0
                color : "#241445"
            }
            GradientStop{
                position : 1
                color : "#511e57"
            }
        }



        ColumnLayout{
            width : parent.width * 0.9
            height : parent.height * 0.85
            anchors.centerIn :parent
            Item{
                Layout.preferredWidth : parent.width
                Layout.fillHeight : true

                
                Image{
                    id: backwaves
                    source :   "../../../res/images/wavesong.png"
                    width : parent.width 
                    height : parent.height * 0.5
                    visible: false
                }

                MultiEffect{
                    source: backwaves
                    brightness : 1.0 
                    colorization : 1.0 
                    colorizationColor :  "#462F58"
                    width : backwaves.width
                    height: backwaves.height
                    anchors.centerIn: parent
                }
                
                RadiusImage{
                    img : root.coverImage
                    anchors.centerIn : parent
                    _radius : 20
                    _width : parent.width*.6
                    _height : parent.height*.9
                }

            }
            Item{
                Layout.preferredWidth : parent.width 
                Layout.preferredHeight : 80

                Column{
                    anchors.fill : parent
                    Item {
                        id : titleContainer
                        width: parent.width *.8
                        height: 30
                        anchors.horizontalCenter :  parent.horizontalCenter
                        clip : true
                        Text {
                            id: scrollingText
                            text: root.title
                            color: "white"
                            x: titleContainer.width  // Start off-screen on the right

                            font{
                                family:"Impact, fantasy"
                                pixelSize :  16
                                bold : true
                            }

                            NumberAnimation on x {
                                id: anim
                                from: titleContainer.width
                                to: -scrollingText.width
                                duration: Math.max(4000, scrollingText.width * 20)
                                loops: Animation.Infinite
                            }
                            onTextChanged : anim.restart()
                            Component.onCompleted: anim.start()
                        }
                    }
                    Row{
                        height :  30 
                        width : parent.width
                        
                        DotText{
                            txt : root.artist
                        }

                        DotText{
                            txt : root.album
                        }
                    }
                }

            }
        }

    } 
}