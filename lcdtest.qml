import QtQuick 2.0
import QtQuick.Window 2.2

Rectangle {
    id: testPattern
    width: 640
    height: 480
    focus: true

    property int colorIndex: 0
    readonly property var colors: ["red", "green", "blue", "white", "grey", "black"]

    color: colors[colorIndex]

    function nextColor() {
        if (colorIndex + 1 < colors.length)
            ++colorIndex
        else
            testPattern.Window.window.close()
    }

    Keys.onEscapePressed: testPattern.Window.window.close()
    Keys.onSpacePressed: nextColor()

    MouseArea {
        anchors.fill: parent
        onClicked: testPattern.nextColor()
    }
}
