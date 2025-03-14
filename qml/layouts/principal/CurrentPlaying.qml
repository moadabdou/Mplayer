pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts
import Mplayer 1.0


Rectangle{
    property real contentAspect : 0.8 
    id :  root
    anchors.fill : parent
    color:"#0d0a1b"

    //left gardient border 
    Rectangle{
        anchors{
            top: parent.top
            bottom:parent.bottom
            left:parent.left
        }
        gradient: Gradient{
            GradientStop{
                position:0
                color : "#ca0091"
            }
            GradientStop{
                position:1
                color : "#2b112d" 
            }
        }
        width:2.5
    }

    ColumnLayout{
        anchors.fill :parent
        spacing : 20
        //Header
        Item{
            id :  header
            Layout.preferredWidth : parent.width
            Layout.preferredHeight : 80
            RowLayout{
                width : parent.width
                height: 35
                anchors.verticalCenter : parent.verticalCenter
                layoutDirection :  Qt.RightToLeft 
                spacing :  10
                Item{
                    Layout.preferredWidth: parent.height
                    Layout.preferredHeight: parent.height
                    Layout.rightMargin: 20
                    CircularIcon{
                        size: parent.height
                        path: "../../../res/icons/person.png"
                        
                    }
                }
                Item{
                    Layout.preferredWidth: parent.height
                    Layout.preferredHeight: parent.height
                    CircularIcon{
                        size: parent.height
                        path: "../../../res/icons/bell.png"
                    }
                }
                Item{
                    Layout.fillWidth : true
                }
            }
        }
        //current playing
        Item{
            id: currentPlaying


            Layout.preferredWidth :  parent.width
            Layout.preferredHeight : 290;

            clip : true

            ColumnLayout{
                height : parent.height
                width : parent.width * root.contentAspect
                anchors.horizontalCenter : parent.horizontalCenter
                spacing : 15
                TextIcon{
                    txt :  "Currently Playing"
                }

                Item{
                    Layout.preferredWidth :  parent.width
                    Layout.fillHeight : true
                    PlayingWindow{} 
                }
            }
        }

        //next songs 
        Item{
            id: nextSongs
            Layout.preferredWidth : parent.width
            Layout.fillHeight : true          

            ColumnLayout{
                height : parent.height
                width : parent.width 
                spacing : 20
                Item{
                    Layout.preferredHeight : nextSongTitleLayout.implicitHeight
                    Layout.preferredWidth: parent.width * root.contentAspect 
                    Layout.alignment : Qt.AlignHCenter
                    RowLayout{
                        id : nextSongTitleLayout
                        width : parent.width
                        spacing : 10
                        anchors.verticalCenter :  parent.verticalCenter
                        TextIcon{ 
                            txt :  "Queue"
                        }
                        Item{
                            Layout.fillWidth : true
                            Layout.preferredHeight : parent.height
                        }

                    }
                }

                Item{
                    Layout.preferredWidth : parent.width
                    Layout.fillHeight : true
                    clip : true
                    
                    Layout.bottomMargin : 15

                    EmptyUfo{
                        visible :  QueuePlaying.currentlyPlaying == -1
                    } 

                    Item{
                        width : parent.width * root.contentAspect 
                        height : parent.height
                        anchors.horizontalCenter : parent.horizontalCenter
                        
                        ListView{
                            id : songlistview
                            anchors.fill : parent
                            model: QueuePlaying
                            snapMode: ListView.SnapToItem
                            spacing : 10
                            currentIndex : QueuePlaying.currentlyPlaying

                            delegate: Rectangle{
                                id : listElement
                                required property int index 
                                required property string filePath 
                                required property string artist 
                                required property string title
                                required property string duration
                                required property string coverImage
                                
                                
                                width: ListView.view.width
                                height :  75
                                radius :  15
                                
                                gradient: Gradient{
                                    GradientStop{
                                        position : 0
                                        color : "#1E104B"
                                    }
                                    GradientStop{
                                        position : 1
                                        color : "#460846"
                                    }
                                }

                                

                                Rectangle{
                                    width :  songlistview.currentIndex  == listElement.index ? parent.width   : parent.width - 2
                                    height:  songlistview.currentIndex  == listElement.index ? parent.height   : parent.height - 2
                                    anchors.centerIn : parent
                                    radius : parent.radius - 1
                                    color :  songlistview.currentIndex  == listElement.index ? "#EF4369"  : "#0D0C1B"
                                    Behavior on  color{
                                        ColorAnimation{
                                            duration : 300
                                        }
                                    }
                                    RowLayout{
                                        anchors.fill : parent
                                        spacing : 0
                                        Item{
                                            Layout.preferredHeight: parent.height
                                            Layout.preferredWidth : parent.width * 2.5/12
                                            
                                            RadiusImage{
                                                img :  listElement.coverImage
                                                _height : 45
                                                _width : 45
                                                _radius : 10
                                                anchors.centerIn : parent
                                            }
                                        }

                                        Item{
                                            Layout.preferredHeight: parent.height
                                            Layout.preferredWidth : parent.width * 6.5/12
                                            
                                            Column{
                                                spacing :  4
                                                width : parent.width
                                                anchors.verticalCenter : parent.verticalCenter
                                                Text{
                                                    text : listElement.title
                                                    width : parent.width
                                                    elide: Text.ElideRight 
                                                    font{
                                                        bold:true
                                                        pointSize : 12
                                                    }
                                                    color : "#FFF"
                                                }
                                                Text{
                                                    text : listElement.artist
                                                    width : parent.width
                                                    elide: Text.ElideRight 
                                                    font{
                                                        pointSize : 9
                                                    }
                                                    color : "#fff"
                                                }
                                            }
                                            
                                        }

                                        Item{
                                            Layout.preferredHeight: parent.height
                                            Layout.preferredWidth : parent.width * 3/12
                                            Text{
                                                anchors.centerIn: parent
                                                text : listElement.duration
                                                color : "#fff"
                                                font{
                                                    pointSize : 9
                                                }
                                            }
                                        }

                                    }

                                }


                                NormalIcon {
                                    icon : "../../../res/icons/audiowave.png"
                                    color : "#EF4369"
                                    size : 40
                                    x : -40
                                    anchors.verticalCenter : parent.verticalCenter
                                    visible :  songlistview.currentIndex  == listElement.index ? true : false
                                }
    

                                MouseArea{
                                    anchors.fill : parent
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: {
                                        QueuePlaying.currentlyPlaying  = listElement.index
                                    }
                                }

                            }

                            

                        }
                    }

                }

            }


        }


    } 
}
