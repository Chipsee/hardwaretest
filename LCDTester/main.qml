import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    visible: true
    id: main
    color:"red"

    /*A modal window prevents other windows from receiving input events*/
    modality: Qt.ApplicationModal

    /** show full screen 1 **/
    /*don't use flags to control when you use visibility to showfullscreen*/
    /*has one bugs, you can see some gost image after change picture*/
    //visibility: "FullScreen"

    /** show full screen 2**/
    /* Produces a borderless window, need fix the width and height and height plus 1 and remove gost image bug*/
    //width:1024
    //height:601
    //flags: Qt.FramelessWindowHint | Qt.Window

    /** show full screen 3 **/
    /*use second way and get width and height dynamic*/
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableWidth + 1
    flags: Qt.FramelessWindowHint | Qt.Window

    property int trig: 0
    property int time: 100

    MouseArea {
        anchors.fill: parent
        onClicked: {
            trig++
            if (trig == 6)
                Qt.quit()
        }
    }

    Timer{
        interval: time
        running: {
            if(trig == 0)
                true
            else
                false
        }
        repeat: false
        onTriggered: {
            main.color = "red"
        }
    }

    Timer{
        interval: time
        running: {
            if(trig == 1)
                true
            else
                false
        }
        repeat: false
        onTriggered: {
            main.color = "green"
        }
    }

    Timer{
        interval: time
        running: {
            if(trig == 2)
                true
            else
                false
        }
        repeat: false
        onTriggered: {
            main.color = "blue"
        }
    }

    Timer{
        interval: time
        running: {
            if(trig == 3)
                true
            else
                false
        }
        repeat: false
        onTriggered: {
            main.color = "white"
        }
    }

    Timer{
        interval: time
        running: {
            if(trig == 4)
                true
            else
                false
        }
        repeat: false
        onTriggered: {
            main.color = "grey"
        }
    }

    Timer{
        interval: time
        running: {
            if(trig == 5)
                true
            else
                false
        }
        repeat: false
        onTriggered: {
            main.color = "black"
        }
    }

}
