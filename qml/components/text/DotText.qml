import QtQuick

Item{
    id : root 

    required property string txt 
    

    width : parent.width / 2
    height :parent.height
    Rectangle{
        width : 4
        height: 4
        radius : 2 
        color : "#777"
        anchors{
            right : detailsText.left
            rightMargin : 10
            verticalCenter:parent.verticalCenter
        }
    }
    Text{
        id : detailsText
        anchors.centerIn : parent
        text : root.txt
        color : "#888"
        elide : Text.ElideRight
        width:parent.width * 0.8
        font{
            pixelSize : 14
        }
    }
}