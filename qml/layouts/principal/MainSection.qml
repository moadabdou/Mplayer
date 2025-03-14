pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Rectangle {
    id: root
    required property string currentCategory

    onCurrentCategoryChanged : {
        if (currentCategory == "Favorites"){
            MediaScanner.scanFolders(" is_fav = 1 ORDER BY title", false) ;
        }else if(currentCategory == "Songs"){
            MediaScanner.scanFolders(" 1 ORDER BY title", false); 
        }else if(currentCategory == "New"){
            MediaScanner.scanFolders(" is_old=0 ORDER BY title", false); 
        }else if (currentCategory == "Recent"){
            MediaScanner.scanFolders(`recent_play  BETWEEN strftime('%s', 'now', '-2 days') 
                                                   AND strftime('%s', 'now') 
                                                   ORDER BY recent_play DESC`
                                    , false);
        }
    }

    anchors.fill: parent
    color: "#0d0a1b"
    Rectangle {
        id: _blur
        anchors.fill: parent
        color: "#0d0a1b"
        visible: false
        // First Circle
        Rectangle {
            id: circle
            width: 80
            height: 80
            color: "#FF3E64"
            opacity: 0.3
            radius: width / 2
            // Pulse Animation
            SequentialAnimation on scale {
                loops: Animation.Infinite
                NumberAnimation {
                    to: 4.3
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    to: 4.0
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
            }

            SequentialAnimation on x {
                loops: Animation.Infinite
                NumberAnimation {
                    to: 150
                    duration: 6000
                    easing.type: Easing.Linear
                }
                NumberAnimation {
                    to: 280
                    duration: 4000
                    easing.type: Easing.Linear
                }
                NumberAnimation {
                    to: 380
                    duration: 3000
                    easing.type: Easing.Linear
                }
            }

            SequentialAnimation on y {
                loops: Animation.Infinite
                NumberAnimation {
                    to: 250
                    duration: 5000
                    easing.type: Easing.Linear
                }
                NumberAnimation {
                    to: 100
                    duration: 4000
                    easing.type: Easing.Linear
                }
                NumberAnimation {
                    to: 30
                    duration: 3000
                    easing.type: Easing.Linear
                }
            }
        }

        // Second Circle
        Rectangle {
            id: circle1
            width: 100
            height: 100
            color: "#452ec6"
            opacity: 0.5
            radius: width / 2
            // Pulse Animation
            SequentialAnimation on scale {
                loops: Animation.Infinite
                NumberAnimation {
                    to: 3.3
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    to: 2.8
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
            }
            SequentialAnimation on x {
                loops: Animation.Infinite
                NumberAnimation {
                    to: 430
                    duration: 5000
                    easing.type: Easing.Linear
                }
                NumberAnimation {
                    to: 380
                    duration: 4000
                    easing.type: Easing.Linear
                }
                NumberAnimation {
                    to: 150
                    duration: 4000
                    easing.type: Easing.Linear
                }
            }
            SequentialAnimation on y {
                loops: Animation.Infinite
                NumberAnimation {
                    to: 150
                    duration: 5000
                    easing.type: Easing.Linear
                }
                NumberAnimation {
                    to: 200
                    duration: 4000
                    easing.type: Easing.Linear
                }
                NumberAnimation {
                    to: 330
                    duration: 4000
                    easing.type: Easing.Linear
                }
            }
            onScaleChanged: {
                blurel.source = null;
                blurel.source = liveSource;
            }
        }
    }
    ShaderEffectSource {
        id: liveSource
        sourceItem: _blur
        anchors.fill: _blur
        live: true // Ensures real-time updates with animations
    }
    FastBlur {
        id: blurel
        anchors.fill: liveSource
        source: liveSource
        radius: 140
    }

    Item {
        id: leftSide
        width: 250
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
    }

    Item {
        id: main
        anchors {
            left: leftSide.right
            bottom: parent.bottom
            right: parent.right
            top: parent.top
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 0
            Item {
                id: header
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 80

                SearchBar {
                    id: searchfield
                    inputfocus: false
                }
            }
            Item {
                id: hotSongsSlider
                Layout.topMargin: 8
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 270

                ColumnLayout {
                    width: parent.width * 0.8
                    height: parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 7
                    Item {
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: parent.width

                        TextIcon {
                            txt: "Best Songs"
                            size: 18
                            path: "../../../res/icons/burn.png"
                            iconSize: 25
                        }
                    }
                    Item {
                        Layout.fillHeight: true
                        Layout.preferredWidth: parent.width
                        FloatingSlider {
                            elms: BestSongs
                            Component.onCompleted: BestSongs.update()
                        }
                    }
                }
            }
            Item {
                id: songsGrid
                Layout.preferredWidth: parent.width
                Layout.fillHeight: true
                Layout.topMargin: 60
                ColumnLayout {
                    width: parent.width * 0.8
                    height: parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 13
                    Item {
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: parent.width

                        TextIcon {
                            txt: root.currentCategory
                            size: 18
                            path: "../../../res/icons/music-album.png"
                            iconSize: 25
                        }
                    }
                    Item {
                        Layout.preferredHeight: ["Songs", "Favorites", "New", "Recent"].find(c => root.currentCategory == c) ? 0 : 40
                        Layout.preferredWidth: parent.width
                        Behavior on Layout.preferredHeight {
                            NumberAnimation {
                                duration: 200
                            }
                        }

                        ListView {
                            property string currentCategoryItem 

                            id: categoryList
                            width: parent.width
                            height: parent.height * .9
                            anchors.verticalCenter: parent.verticalCenter
                            orientation: ListView.Horizontal
                            spacing: 10
                            clip: true  // Prevents overflow

                            model: SongDatabase.fetchCategoryItems(root.currentCategory)
                            
                            currentCategoryItem: model.length > 0 ? model[0] : ""
                            onModelChanged: currentCategoryItem = model.length > 0 ? model[0] : ""

                            delegate: Item {
                                id: categoryElm
                                required property int index
                                required property string modelData
                                width: children[0].width
                                height: parent.height
                                Rectangle {
                                    id: categoryRect
                                    width: children[0].implicitWidth + 20
                                    height: parent.height
                                    radius: 10  // Rounded corners
                                    color: categoryList.currentIndex == categoryElm.index ? "#EF4369" : "#30003b"

                                    // Add hover behavior
                                    Behavior on color {
                                        ColorAnimation {
                                            duration: 150
                                        }
                                    }

                                    Text {
                                        anchors.centerIn: parent
                                        text: categoryElm.modelData
                                        font.pixelSize: 17
                                        color: "white"
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            categoryList.currentIndex = categoryElm.index;
                                            categoryList.currentCategoryItem = categoryElm.modelData;
                                        }
                                    }
                                }
                            }

                            onCurrentCategoryItemChanged : {
                                if (root.currentCategory == "Albums"){
                                    MediaScanner.scanFolders(" album = '"+ currentCategoryItem.replace(/'/g, "''") +"' ORDER BY title", false)
                                }else if (root.currentCategory == "Artists") {
                                    MediaScanner.scanFolders(" artist = '"+ currentCategoryItem.replace(/'/g, "''") +"' ORDER BY title", false)
                                }else if (root.currentCategory == "Folders"){
                                    MediaScanner.scanFolders(" path LIKE '"+ currentCategoryItem.replace(/'/g, "''") +"%' ORDER BY title", false)
                                }
                            }

                        }
                    }
                    Item {
                        id :  gridContainer
                        Layout.fillHeight: true
                        Layout.preferredWidth: parent.width
                        clip: true
                        EmptyUfo{
                            visible :  MediaScanner.isEmpty
                        } 
                        GridView {
                            id: musicgrid
                            anchors.fill: parent
                            cellWidth: width / 2
                            cellHeight: 85
                            model: MediaScanner
                            delegate: songsElement
                            Component.onCompleted: MediaScanner.scanFolders()
                        }
                        Component {
                            id: songsElement
                            Rectangle {
                                id: elm
                                required property int index
                                required property string filePath
                                required property string coverImage
                                required property string artist
                                required property string album
                                required property string duration
                                required property string title
                                required property bool isFav

                                width: gridContainer.width / 2 - 10
                                height: 70
                                radius: 10

                                SongElement {
                                    _width: parent.width
                                    _height: parent.height
                                    img: elm.coverImage
                                    artist: elm.artist
                                    songName: elm.title
                                    duration : elm.duration
                                    album : elm.album
                                    isFav: elm.isFav
                                    filePath: elm.filePath
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
