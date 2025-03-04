import  QtQuick
import  QtQuick.Layouts
import  QtQuick.Effects
import  Mplayer 1.0

Rectangle{
    id : root
    required property string songimg

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
            width : parent.width * 0.7
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
                
                RoundedImage{
                    img : root.songimg 
                    anchors.centerIn : parent
                }

            }
            Item{
                Layout.preferredWidth : parent.width 
                Layout.preferredHeight : 80

                Column{
                    anchors.fill : parent
                    Text{
                        text :  "GALAXY"
                        height :  30
                        anchors.horizontalCenter : parent.horizontalCenter
                        font{
                            family:"Impact, fantasy"
                            pixelSize :  16
                            bold : true
                        }
                        color :  "#fff"
                    }
                    Row{
                        height :  30 
                        width : parent.width
                        
                        DotText{
                            txt : "Travis Scott wo47473847398798"
                        }

                        DotText{
                            txt : "Best of 2020 493746394364"
                        }
                    }
                }

            }
        }

    } 
}