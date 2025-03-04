import QtQuick
import Mplayer 1.0

Item {
    id: root
    anchors.fill: parent
    clip: true
    focus: true
    required property ListModel elms

    PathView {
        anchors.fill: parent
        model: root.elms
        delegate: flipCardDelegate

        path: Path {
            startX: root.width * 0.15
            startY: root.height / 2

            PathAttribute {
                name: "itemZ"
                value: 0
            }
            PathAttribute {
                name: "itemAngle"
                value: -50.0
            }
            PathAttribute {
                name: "itemScale"
                value: 0.7
            }
            PathAttribute {
                name: "itemOpacity"
                value: 0
            }

            PathLine {
                y: root.height / 2
                x: root.width * 0.4
            }

            PathAttribute {
                name: "itemOpacity"
                value: 0.7
            }

            PathLine {
                y: root.height / 2
                x: root.width * 0.5
            }

            PathAttribute {
                name: "itemAngle"
                value: 0.0
            }
            PathAttribute {
                name: "itemScale"
                value: 1.0
            }
            PathAttribute {
                name: "itemZ"
                value: 100
            }
            PathAttribute {
                name: "itemOpacity"
                value: 1.0
            }

            PathLine {
                y: root.height / 2
                x: root.width * 0.6
            }

            PathAttribute {
                name: "itemOpacity"
                value: .7
            }

            PathLine {
                y: root.height / 2
                x: root.width * .85
            }

            PathAttribute {
                name: "itemAngle"
                value: 50.0
            }
            PathAttribute {
                name: "itemScale"
                value: 0.7
            }
            PathAttribute {
                name: "itemZ"
                value: 0
            }
            PathAttribute {
                name: "itemOpacity"
                value: 0
            }
        }

        pathItemCount: 5

        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
    }

    Component {
        id: flipCardDelegate

        Item {
            id: wrapper

            required property int index
            property real rotX: PathView.itemAngle
            required property string img 
            required property bool isfav
            required property string artist
            required property string songName 

            visible: PathView.onPath
            clip : true
            width: parent.width * .8 
            height: parent.height
            scale: PathView.itemScale
            z: PathView.itemZ
            opacity: PathView.itemOpacity
            antialiasing: true

            transform: Rotation {
                axis {
                    x: 0
                    y: 1
                    z: 0
                }
                angle: wrapper.rotX
                origin {
                    x: wrapper.width / 2
                    y: wrapper.height / 2
                }
            }

            SongCover{
                songName : wrapper.songName
                img :wrapper.img
                artist : wrapper.artist 
                isfav: wrapper.isfav
            }
        }
    }
}
