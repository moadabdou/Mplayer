import QtQuick 
import QtQuick.Effects
import QtQuick.Shapes
import QtQuick.Controls
import QtQuick.Layouts
Item {
    required property Item inner
    id: root
    height: 65
    ShaderEffectSource {
        id: effectSource
        anchors.fill: parent
        sourceItem: root.inner
        sourceRect: Qt.rect(0, root.inner.height - parent.height, root.inner.width, parent.height)
        visible: false
        live: true
        layer.enabled: true
        layer.smooth: true
    }
    MultiEffect {
        anchors.fill: effectSource
        source: effectSource
        autoPaddingEnabled: false
        blurEnabled: true
        blurMax: 100
        blur: 1.0
    }
    Rectangle{
        anchors.fill : parent
        color :  "#2c1939"
        opacity :  0.7
    }
    Shape {
        property real _H : 1.5;
        id : topBorder
        width: parent.width
        height: _H
        anchors.top :  parent.top
        ShapePath {
            strokeWidth: 0
            strokeColor: "transparent"
            fillGradient: LinearGradient {
                x1: 0; y1: 0
                x2: root.width ; y2: 0
                GradientStop { position: 0; color:  "#EF4369" }
                GradientStop { position: 1; color: "#2a115b" }
            }
            startX: 0; startY: 0
            PathLine { x: root.width; y: 0 }
            PathLine { x: root.width; y: topBorder._H }
            PathLine { x: 0; y:  topBorder._H}
            PathLine { x: 0; y: 0 }
        }
    }

    
    MouseArea {

        anchors.fill: parent
        acceptedButtons: Qt.AllButtons
        hoverEnabled: true
        preventStealing: true  // Prevents flicking from being stolen by ListView
        onWheel: (event) => event.accepted = true  // Block scroll events
        onExited:{
            PlayController.isShown = false;
        }

        RowLayout{
            anchors.fill : parent 
            Item{
                Layout.fillHeight : true 
                Layout.preferredWidth : 300

                RowLayout{
                    anchors.centerIn :parent
                    spacing : 20
                    Item{
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth : 40
                        
                        RadiusImage{
                            img :  PlayController.songCover
                            _height : parent.width
                            _width : parent.width
                            _radius : 10
                            anchors.centerIn : parent
                        }
                    }

                    Item{
                        Layout.preferredHeight: parent.height
                        Layout.preferredWidth : 200
                        
                        Column{
                            spacing :  4
                            width : parent.width
                            anchors.verticalCenter : parent.verticalCenter
                            Item {
                                id : titleContainer
                                width: parent.width
                                height: 20
                                clip : true
                                Text {
                                    id: scrollingText
                                    text: PlayController.songTitle
                                    color: "white"
                                    x: titleContainer.width  // Start off-screen on the right

                                    font{
                                        family:"Impact, fantasy"
                                        pixelSize :  14
                                        bold : true
                                    }

                                    NumberAnimation on x {
                                        id: anim
                                        from: titleContainer.width
                                        to: -scrollingText.width
                                        duration: Math.max(4500, scrollingText.width * 20)
                                        loops: Animation.Infinite
                                    }
                                    onTextChanged : anim.restart()
                                    Component.onCompleted: anim.start()
                                }
                            }
                            Text{
                                text : PlayController.songArtist
                                elide: Text.ElideRight 
                                font{
                                    pointSize : 9
                                }
                                color : "#fff"
                            }
                        }
                        
                    }

                }


            }
            Item{
                Layout.fillHeight : true 
                Layout.preferredWidth : 150
                RowLayout{
                    anchors.centerIn : parent 
                    spacing : 10
                    NormalIcon{
                        icon : "../../../res/icons/previous.png"
                        size : 30
                        color : "#fff"
                        opacity : 0.7
                        MouseArea{
                            anchors.fill : parent
                            cursorShape : Qt.PointingHandCursor
                            onClicked : {
                                PlayController.prev();
                            }
                        }
                    }
                    Rectangle{
                        Layout.preferredWidth : 45
                        Layout.preferredHeight : 45
                        radius : Layout.preferredWidth / 2
                        color: "#EF4369"
                        NormalIcon{
                            anchors.centerIn : parent
                            icon :  PlayController.isPlaying ? "../../../res/icons/pause.png":"../../../res/icons/play.png"
                            size : 30
                            anchors.horizontalCenterOffset : PlayController.isPlaying ? 0 : 2
                            color : "#0D0C1B"
                        }
                        MouseArea{
                            anchors.fill : parent
                            cursorShape : Qt.PointingHandCursor
                            onClicked: {
                                PlayController.isPlaying ? PlayController.pause() : PlayController.play()
                            }
                        }
                    }
                    NormalIcon{
                        icon : "../../../res/icons/next.png"
                        size : 30
                        color : "#fff"
                        opacity : 0.7
                        MouseArea{
                            anchors.fill : parent
                            cursorShape : Qt.PointingHandCursor
                            onClicked : {
                                PlayController.next();
                            }
                        }
                    }
                }
            }
            Item{
                id :  progressBar
                Layout.fillHeight : true 
                Layout.fillWidth : true
                Layout.minimumWidth :  250
                RowLayout{
                    anchors.centerIn : parent 
                    spacing : 10
                    Text{
                        text : PlayController.songPassedDuration
                        color : "white"
                    }
                    Item{
                        Layout.preferredWidth : progressBar.width * .55
                        Layout.preferredHeight : 30
                        MouseArea{
                            anchors.fill : parent
                            cursorShape : Qt.PointingHandCursor
                            onClicked: mouse => {
                                PlayController.seek(mouse.x / width)
                            }
                        }
                        Rectangle{
                            width: parent.width
                            height : 6 
                            color : "#aaa"
                            radius : height/2
                            anchors.verticalCenter : parent.verticalCenter
                            Rectangle{
                                height : parent.height 
                                width : parent.width * PlayController.songPosition
                                radius : height/2
                                color : "#EF4369"
                                Behavior on width {
                                    NumberAnimation{
                                        duration:200
                                    }
                                }
                            }
                        }
                    }
                    Text{
                        text : PlayController.songDuration
                        color : "white"
                    }
                    
                }
            }
            Item{
                Layout.fillHeight : true 
                Layout.preferredWidth : 200
                RowLayout{
                    spacing : 10
                    anchors.fill:parent
                    Item{
                        Layout.fillHeight : true
                        Layout.preferredWidth : 30
                        Layout.minimumWidth : 30
                        NormalIcon{
                            icon : ["../../../res/icons/shuffle-off.png",
                                    "../../../res/icons/exchange.png",
                                    "../../../res/icons/repeat-current.png",
                                    "../../../res/icons/shuffle.png"][PlayController.repeatMode]
                            size : 30
                            color : "#fff"
                            opacity : 0.7
                            anchors.verticalCenter : parent.verticalCenter
                            MouseArea{
                                anchors.fill : parent
                                cursorShape : Qt.PointingHandCursor
                                onClicked : {
                                    PlayController.repeatMode =  (PlayController.repeatMode+1)%4
                                }
                            }
                        }
                    }
                    Item{
                        Layout.fillHeight : true
                        Layout.preferredWidth : 30
                        Layout.minimumWidth : 30
                        NormalIcon{
                            icon : "../../../res/icons/medium-volume.png"
                            size : 30
                            color : "#fff"
                            opacity : 0.7
                            anchors.verticalCenter : parent.verticalCenter
                            MouseArea{
                                anchors.fill : parent
                                cursorShape : Qt.PointingHandCursor
                            }
                        }
                    }
                    
                    Item{
                        Layout.fillWidth : true
                        Layout.fillHeight : true 
                        Slider {
                            id: control
                            anchors.verticalCenter :  parent.verticalCenter
                            height : 20
                            width:parent.width *.8
                            palette.light:  "#EF4369" 
                            palette.window:  "#EF4369" 
                            palette.midlight: "#aaa"
                            palette.dark:  "#EF4369" 
                            from: 0
                            to: 100
                            value : PlayController.volume
                            onValueChanged : PlayController.volume = value
                            handle: Rectangle {
                                x: control.leftPadding + (control.horizontal ? control.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
                                y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : control.visualPosition * (control.availableHeight - height))
                                implicitWidth: 20
                                implicitHeight: 20
                                radius: width / 2
                                color: control.pressed ? control.palette.light : control.palette.window
                                border.width: 3
                                border.color: "#EF4369"
                            }

                            background:  Rectangle {
                                x: control.leftPadding + (control.horizontal ? 0 : (control.availableWidth - width) / 2)
                                y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : 0)
                                implicitWidth: control.horizontal ? 200 : 4
                                implicitHeight: control.horizontal ? 4 : 200
                                width: control.horizontal ? control.availableWidth : implicitWidth
                                height: control.horizontal ? implicitHeight : control.availableHeight
                                radius: 3
                                color: control.palette.midlight
                                scale: control.horizontal && control.mirrored ? -1 : 1

                                Rectangle {
                                    y: control.horizontal ? 0 : control.visualPosition * parent.height
                                    width: control.horizontal ? control.position * parent.width : 4
                                    height: control.horizontal ? 4 : control.position * parent.height

                                    radius: 3
                                    color: control.palette.dark
                                }
                            }
                            
                        }
                    }
                }
            }
        }
    }
}