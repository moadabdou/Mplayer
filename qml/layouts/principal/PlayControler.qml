import QtQuick 
import QtQuick.Effects
import QtQuick.Shapes
import QtQuick.Layouts
Item {
    required property Item inner
    id: root
    height: 80
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
            parent.anchors.bottomMargin = -80
        }
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
                    Layout.preferredWidth : 50
                    
                    RadiusImage{
                        img :  "../../../res/images/cover1.jpg"
                        _height : parent.width
                        _width : parent.width
                        _radius : 10
                        anchors.centerIn : parent
                    }
                }

                Item{
                    Layout.preferredHeight: parent.height
                    Layout.preferredWidth : 80
                    
                    Column{
                        spacing :  4
                        width : parent.width
                        anchors.verticalCenter : parent.verticalCenter
                        Text{
                            text : "Galaxy"
                            elide: Text.ElideRight 
                            font{
                                bold:true
                                pointSize : 12
                            }
                            color : "#FFF"
                        }
                        Text{
                            text : "John Doe"
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
                    }
                }
                Rectangle{
                    Layout.preferredWidth : 55
                    Layout.preferredHeight : 55
                    radius : Layout.preferredWidth / 2
                    color: "#EF4369"
                    NormalIcon{
                        anchors.centerIn : parent
                        icon : "../../../res/icons/pause.png"
                        size : 30
                        color : "#0D0C1B"
                    }
                    MouseArea{
                        anchors.fill : parent
                        cursorShape : Qt.PointingHandCursor
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
                    text : "00:30"
                    color : "white"
                }
                Rectangle{
                    Layout.preferredWidth : progressBar.width * .55
                    Layout.preferredHeight : 5
                    radius :  Layout.preferredHeight / 2
                    color : "#aaa"
                    MouseArea{
                        anchors.fill : parent
                        cursorShape : Qt.PointingHandCursor
                    }
                    Rectangle{
                        height : parent.height 
                        width : parent.width * .4
                        radius : height/2
                        color : "#EF4369"
                    }
                }
                Text{
                    text : "03:30"
                    color : "white"
                }
                
            }
        }
        Item{
            Layout.fillHeight : true 
            Layout.preferredWidth : 200
            RowLayout{
                anchors.centerIn : parent 
                spacing : 10
                NormalIcon{
                    icon : "../../../res/icons/shuffle.png"
                    size : 30
                    color : "#fff"
                    opacity : 0.7
                    MouseArea{
                        anchors.fill : parent
                        cursorShape : Qt.PointingHandCursor
                    }
                }
                NormalIcon{
                    icon : "../../../res/icons/exchange.png"
                    size : 30
                    color : "#fff"
                    opacity : 0.7
                    MouseArea{
                        anchors.fill : parent
                        cursorShape : Qt.PointingHandCursor
                    }
                }
                NormalIcon{
                    icon : "../../../res/icons/medium-volume.png"
                    size : 30
                    color : "#fff"
                    opacity : 0.7
                    MouseArea{
                        anchors.fill : parent
                        cursorShape : Qt.PointingHandCursor
                    }
                }
            }
        }
    }
}