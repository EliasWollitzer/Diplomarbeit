import QtQuick 2.5
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3
import "."


Rectangle {
    id: root
    // TODO: inherit keyboardItem instead of agregation

    property alias source: keyboardItem.source
    property alias keyWidth: keyboardItem.keyWidth
    property alias keyHeight: keyboardItem.keyHeight
    property alias bounds: keyboardItem.bounds
    property alias mainFont: keyboardItem.mainFont
    property alias mainFontColor: keyboardItem.mainFontColor
    property alias secondaryFont: keyboardItem.secondaryFont
    property alias secondaryFontColor: keyboardItem.secondaryFontColor
    property alias keyColor: keyboardItem.keyColor
    property alias keyPressedColor: keyboardItem.keyPressedColor

    color: "#192430"
    /*
    width: 1024
    height: 640
    */
    width: 800
    height: 200
    x: 0
    y: 200

    KeyboardItem {
        id: keyboardItem
        anchors.centerIn: parent
        source: "keyboard_us.xml"

        onKeyClicked: kb.set_char(key)
        onSwitchSource: root.source = source
    }
}

