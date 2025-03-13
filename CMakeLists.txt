cmake_minimum_required(VERSION 3.16)

project(Mplayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)



find_package(Taglib REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Quick Core)

qt_standard_project_setup(REQUIRES 6.5)

qt_policy(SET QTP0004 NEW)

qt_add_executable(appMplayer
    main.cpp
)


target_include_directories(appMplayer  PRIVATE ${PROJECT_SOURCE_DIR}/include)

qt_add_qml_module(appMplayer
    URI Mplayer
    VERSION 1.0
    QML_FILES
        Main.qml
        qml/App.qml

        qml/layouts/principal/MainSection.qml
        qml/layouts/principal/PlayControler.qml
        qml/layouts/principal/SideBar.qml
        qml/layouts/principal/CurrentPlaying.qml

        qml/components/icons/CircularIcon.qml
        qml/components/icons/FavIcon.qml
        qml/components/icons/NormalIcon.qml
        qml/components/cards/PlayingWindow.qml
        qml/components/cards/SongCover.qml
        qml/components/cards/SongElement.qml
        qml/components/images/RoundedImage.qml  
        qml/components/images/RadiusImage.qml
        qml/components/sliders/FloatingSlider.qml


        qml/components/text/DotText.qml
        qml/components/text/TextIcon.qml
        qml/components/text/SearchBar.qml
        
    RESOURCES
        res/icons/bell.png
        res/icons/person.png
        res/icons/audiowave.png
        res/icons/search.png
        res/icons/burn.png
        res/icons/music-album.png
        res/icons/heart_empty.png
        res/icons/heart.png
        res/icons/dots.png
        res/icons/next.png
        res/icons/pause.png
        res/icons/previous.png
        res/icons/medium-volume.png
        res/icons/shuffle.png
        res/icons/exchange.png
        res/images/songImg.jpg
        res/images/cover1.jpg
        res/images/cover2.jpg
        res/images/cover4.jpg
        res/images/cover5.jpg
        res/images/cover6.jpg
        res/images/defaultCover.jpg
        res/images/wavesong.png
    SOURCES 
        include/media/MediaScanner.h src/backend/media/MediaScanner.cpp
        include/db/update_db.h src/backend/db/update_db.cpp
        include/db/fetch_db.h src/backend/db/fetch_db.cpp
        include/db/Song_db.h src/backend/db/Song_db.cpp
        include/shared/types/song.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appMplayer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appMplayer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appMplayer
    PRIVATE Qt6::Quick Qt6::Core tag  SQLite::SQLite3
)  


include(GNUInstallDirs)
install(TARGETS appMplayer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
