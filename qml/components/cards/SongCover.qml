import QtQuick
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import Mplayer 1.0

Rectangle{
    id : root
    property string img 
    property bool isfav
    property string artist
    property string songName 
    
    anchors.fill : parent
    clip : true
    color : "transparent"
    radius : .13 * parent.height
    
    RadiusImage{
        id : songCover
        img : root.img
        _width : parent.width 
        _height : parent.height  
        _radius : .13 * parent.height
    }

    
    Item {
        anchors.fill: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: parent
        }
        ShaderEffectSource {
            id: liveSource
            sourceItem: songCover
            anchors{
                bottom : parent.bottom
            }

            width : parent.width
            height : parent.height* 0.35
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: parent
            }
            sourceRect :  Qt.rect(x, y, width, height)
        }

        FastBlur {
            id: blurel
            anchors.fill: liveSource
            source: liveSource
            radius: 100
        }

        Rectangle{
            anchors.fill : liveSource
            color : "#0d0a1b"
            opacity : 0.5
        }
    }

    Item{
        anchors{
            bottom : parent.bottom
        }

        width : parent.width
        height : parent.height* 0.35

        ColumnLayout{
            width : parent.width * .8 
            anchors.centerIn: parent
            Item{
                clip: true
                Layout.preferredWidth :parent.width*.9
                Layout.preferredHeight : children[0].implicitHeight
                Text{
                    text : root.songName
                    font.pixelSize : 22
                    width : parent.width
                    elide : Text.ElideRight
                    color : "#fff"
                    font.weight : 900
                }
            }
            Item{
                Layout.preferredWidth :parent.width*.9
                Layout.preferredHeight : children[0].implicitHeight
                clip: true
                Text{
                    text : root.artist
                    width : parent.width
                    elide : Text.ElideRight
                    font.pixelSize : 14
                    color : "#fff"
                    opacity : .7
                }
            }
        }

        Item{
            width : parent.width * 0.1
            anchors.right : parent.right
            anchors.verticalCenter : parent.verticalCenter
            anchors.rightMargin : 10
            height : 40
            FavIcon{
                size :  parent.height
                isfav : root.isfav
            }
        }

    }





}