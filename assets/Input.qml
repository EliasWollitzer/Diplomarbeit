import QtQuick 2.5
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3
import "."
import "./keyboard"

Item {
    id: input_layout
    property var textObject

    Keyboard {
        x: 0
        y: -10
        width: 800
        height: 600
        id: kb

        TextArea {
            id: text_area
            x: 177
            y: 21
            width: 588
            height: 91
            text: ""
            cursorPosition: text_area.text.length
            property bool marked: false
            onSelectedTextChanged: function() {
                var s = selectionStart
                var e = selectionEnd                

                if(s == e){     //one position selected

                    if(!marked) {
                        text_area.cursorPosition = e
                    }

                    marked = false

                }else {                   
                    marked = true                   
                }
            }
        }

        Connections{
            target: slay
            onEdit: {   //parameters STR name STR text
                slay.currentIndex = background.input
                input_name.text = name
                text_area.text = obj.text
                text_area.cursorPosition = text_area.text.length
                input_layout.textObject = obj
            }
        }

        function set_char(key) {

            var txt1 = text_area.text
            var txt2 = txt1.slice(0, text_area.cursorPosition) + key + txt1.slice(text_area.cursorPosition);

            var cp = text_area.cursorPosition
            text_area.text = txt2
            text_area.cursorPosition = cp + 1

        }

        function del_pressed() {

            var str = text_area.text
            var cp

            if(text_area.marked == true) {
                del_selected_text()
                return
            }

            if(text_area.cursorPosition <= 0) {
                cp = text_area.cursorPosition

            }else {
                cp = text_area.cursorPosition - 1
            }

            str = str.slice(0, cp) + str.slice(text_area.cursorPosition);

            text_area.cursorPosition = cp
            text_area.text = str
            text_area.cursorPosition = cp
        }

        function del_selected_text() {
            var s = text_area.selectionStart
            var e = text_area.selectionEnd

            var str = text_area.text
            str = str.slice(0,s) + str.slice(e)          
            text_area.cursorPosition = 0

            text_area.text = str

            text_area.marked = false
            text_area.cursorPosition = s

        }

        function ent_pressed() {
            textObject.text = text_area.text

            text_area.cursorPosition = 0
            text_area.text = ""

            slay.currentIndex = slay.lastIndex
        }

        Text {
            id: input_name
            x: 27
            y: 21
            width: 144
            height: 37
            color: "#f7f6f6"
            font.pixelSize: 19
        }

        Text {
            id: shift
            x: 50
            y: 305
            width: 57
            height: 69
            color: "#fdf4f4"
            text: qsTr("SHIFT")
            font.pixelSize: 20
        }

        Text {
            id: del
            x: 700
            y: 305
            width: 57
            height: 69
            color: "#fdf4f4"
            text: qsTr("DEL")
            font.pixelSize: 20
        }

        Text {
            id: ent
            x: 700
            y: 401
            width: 57
            height: 69
            color: "#fdf4f4"
            text: qsTr("ENT")
            font.pixelSize: 20
        }

        Button {
            id: button_cancel
            x: 27
            y: 64
            width: 131
            height: 41
            text: qsTr("Cancel")
            onClicked: function(){
                slay.currentIndex = slay.lastIndex

                //Zurücksetzen
            }
        }
    }
}
