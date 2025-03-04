import  QtQuick
import QtQuick.Effects
Item{
    id : root 
    required property string img 
    property real _width : 70
    property real _height : 70
    property real _radius : 10

    width : _width
    height : _height 

    Image{
        id : songimage
        source : root.img
        width : root._width
        height : root._height
        fillMode : Image.PreserveAspectCrop
        visible:false
        smooth :  true
    }

    Rectangle{
        id : imgmask
        layer.enabled : true
        width : songimage.width
        height : songimage.height
        radius : root._radius
        visible: false
    }

    MultiEffect{
        source : songimage
        width : songimage.width
        height : songimage.height
        maskEnabled : true 
        maskSource : imgmask
    }
}