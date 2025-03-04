pragma ComponentBehavior: Bound
import QtQuick 
import QtQuick.Layouts
import QtQuick.Shapes
import Mplayer 1.0
Item{
    id : root
    height : parent.height
    width : 210
    ColumnLayout{
        width : parent.width
        spacing : 0
        id : sideBarBack
        Rectangle{
            id : sideBarController
            Layout.fillWidth: true
            Layout.preferredHeight: root.height *.2
            color : "#0D0C1B"
            border{
                width :  2
                color : "#ca0091"
            }
            radius : 15
            Layout.topMargin : -15 
            Layout.leftMargin : -15
            Behavior on Layout.preferredHeight{
                NumberAnimation{
                    duration : 200
                    easing.type : Easing.OutQuart
                }
            }
        }
        Rectangle{
            id : selectRegion
            Layout.fillWidth: true
            Layout.preferredHeight: 35
            color : "transparent"
            z:1
            Item{
                width :parent.width * .2
                height : parent.height + 16
                anchors{
                    top : parent.top 
                    topMargin : -8
                }
                Shape {
                    layer.enabled : true
                    layer.samples : 4
                    id : rounded
                    anchors.fill : parent
                    ShapePath {
                        
                        strokeWidth: 2.8
                        startX: 0 ; startY: 0
                        fillColor: "#0D0C1B"
                        strokeColor : "transparent"
                        PathLine{
                            x : rounded.width ; y : 5
                        }
                        PathArc {
                            x:  rounded.width ; y: rounded.height - 6 // End at the bottom
                            radiusX :2.5 ; radiusY: 2 
                            direction:  PathArc.Counterclockwise
                        }
                        PathLine{
                            x : 0 ; y : rounded.height 
                        }
                    }
                    ShapePath {
                        
                        strokeWidth: 2.6
                        startX: rounded.width ; startY: 6
                        strokeColor: "#ca0091"
                        fillColor: "transparent"
                        PathArc {
                            x:  rounded.width ; y: rounded.height - 6 // End at the bottom
                            radiusX :2.5 ; radiusY: 2 
                            direction:  PathArc.Counterclockwise
                        }
                    }
                }
            }
        }
        Rectangle{
            Layout.fillWidth: true
            Layout.preferredHeight : root.height 
            gradient: Gradient{
                GradientStop{
                    position:0
                    color : "#ca0091"
                }
                GradientStop{
                    position:.5
                    color : "#2b112d" 
                }
                GradientStop{
                    position:1
                    color : "#2b112d" 
                }
            }
            radius : 15
            Layout.leftMargin : -15
            Rectangle{
                width : parent.width  - 5.5
                height : parent.height - 6
                anchors.centerIn : parent
                color : "#0D0C1B"
                radius : parent.radius - 2
            }
        }
    }

//content 

    ColumnLayout{
        id : content 
        anchors.fill : root
        spacing : 0
        Item{
            Layout.preferredHeight : 100 
            Layout.fillWidth : true
            Text{
                text :  "LOGO"
                anchors.centerIn : parent 
                font.pixelSize : 22
                font.bold :  true
                color : "white"
            }
        }
        Item{
            Layout.fillHeight : true 
            Layout.fillWidth : true
            ColumnLayout{
                anchors.fill : parent
                anchors.leftMargin : 50 
                Item{
                    Layout.preferredHeight : 50 
                    Layout.fillWidth : true
                    Layout.leftMargin : -10
                    Layout.topMargin: 20
                    Text{
                        text: "My Library"
                        color : "white"
                        font.pixelSize : 20
                        font.bold : true 

                    }
                }
                Item{
                    Layout.fillWidth : true 
                    Layout.fillHeight : true
                        ListView{
                            anchors.fill : parent
                            id : listGenres
                            Component.onCompleted : {
                                var pos = children[0].mapToItem(null, 0, 0);
                                sideBarController.Layout.preferredHeight = pos.y + 25
                            }
                            model : ListModel{
                                ListElement{
                                    txt : "Songs"
                                    icon : "../../../res/icons/audiowave.png"
                                }
                                ListElement{
                                    txt : "favorite"
                                    icon : "../../../res/icons/heart.png"
                                }
                                ListElement{
                                    txt : "Albums"
                                    icon : "../../../res/icons/music-album.png"
                                }
                                ListElement{
                                    txt : "New"
                                    icon : "../../../res/icons/bell.png"
                                }
                            }
                            spacing : 0
                            interactive: false 
                            delegate: Item{
                                id :  elm
                                required property string icon
                                required property int index
                                required property string txt
                                width :  parent.width 
                                height :  50 
                                RowLayout{
                                    anchors.fill : parent
                                    Item{
                                        Layout.fillHeight:true
                                        Layout.preferredWidth : parent.width *.2
                                        NormalIcon{ 
                                            anchors.centerIn : parent
                                            icon :  elm.icon                             
                                            anchors.verticalCenterOffset:2
                                            color : listGenres.currentIndex == elm.index ?  "#EF4369" : "#8D8B98"
                                            size : 35
                                            Behavior on  color{
                                                ColorAnimation {
                                                    duration :  100
                                                }
                                            }
                                        }
                                    }
                                    Item{
                                        Layout.fillHeight: true
                                        Layout.fillWidth : true
                                        Text{
                                            text :  elm.txt
                                            anchors.verticalCenter : parent.verticalCenter
                                            color : listGenres.currentIndex == elm.index ?  "#fff" : "#8D8B98"
                                            font.pixelSize : 16
                                            font.weight : listGenres.currentIndex == elm.index  ? 700 : 400
                                            Behavior on  color{
                                                ColorAnimation {
                                                    duration :  100
                                                }
                                            }
                                        }
                                    }
                                }
                                MouseArea{
                                    anchors.fill : parent
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: mouse=> {
                                        var pos = elm.mapToItem(null, 0, 0);
                                        listGenres.currentIndex = elm.index
                                        listNetwork.currentIndex = -1
                                        sideBarController.Layout.preferredHeight = pos.y + 25
                                    }
                                }

                            }
                        }
                }
            }
        }
        Item{
            Layout.preferredHeight : 200 
            Layout.fillWidth : true
            ColumnLayout{
                anchors.fill : parent
                anchors.leftMargin : 50 
                Item{
                    Layout.preferredHeight : 50 
                    Layout.fillWidth : true
                    Layout.leftMargin : -10
                    Layout.topMargin: 10
                    Text{
                        text: "Networking"
                        color : "white"
                        font.pixelSize : 20
                        font.bold : true 

                    }
                }
                Item{
                    Layout.fillWidth : true 
                    Layout.fillHeight : true
                        ListView{
                            property int  currentIndex : -1
                            anchors.fill : parent
                            id : listNetwork
                            model : ListModel{
                                ListElement{
                                    txt : "Connect"
                                    icon : "../../../res/icons/audiowave.png"
                                }
                                ListElement{
                                    txt : "Host"
                                    icon : "../../../res/icons/heart.png"
                                }
                            }
                            spacing : 0
                            interactive: false 
                            delegate: Item{
                                id :  elmNet
                                required property string icon
                                required property int index
                                required property string txt
                                width :  parent.width 
                                height :  50 
                                RowLayout{
                                    anchors.fill : parent
                                    Item{
                                        Layout.fillHeight:true
                                        Layout.preferredWidth : parent.width *.2
                                        NormalIcon{ 
                                            anchors.centerIn : parent
                                            icon :  elmNet.icon 
                                            color : listNetwork.currentIndex == elmNet.index ?  "#EF4369" : "#8D8B98"
                                            size : 35
                                            Behavior on  color{
                                                ColorAnimation {
                                                    duration :  100
                                                }
                                            }
                                        }
                                    }
                                    Item{
                                        Layout.fillHeight: true
                                        Layout.fillWidth : true
                                        Text{
                                            text :  elmNet.txt
                                            anchors.verticalCenter : parent.verticalCenter
                                            color : listNetwork.currentIndex == elmNet.index ?  "#fff" : "#8D8B98"
                                            font.pixelSize : 16
                                            font.weight : listNetwork.currentIndex == elmNet.index  ? 700 : 400
                                            Behavior on  color{
                                                ColorAnimation {
                                                    duration :  100
                                                }
                                            }
                                        }
                                    }
                                }
                                MouseArea{
                                    anchors.fill : parent
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: mouse=> {
                                        var pos = elmNet.mapToItem(null, 0, 0);
                                        listNetwork.currentIndex = elmNet.index
                                        listGenres.currentIndex = -1
                                        sideBarController.Layout.preferredHeight = pos.y + 25
                                    }
                                }

                            }
                        }
                }
            }
        }
    }

}