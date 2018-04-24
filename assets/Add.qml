import QtQuick 2.5
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3
import "."
import "./keyboard"


Item {
    id: add
    property int edit_button: 1
    property int rbutton: 0

    Connections{
        target: slay
        onEnt_pressed: {
            if(slay.lastIndex != background.add){
                return
            }
            setInput(str)

        }

    }

    function clear() {
        textField2.text = ""
        textField1.text = ""
    }

    function setInput(str) {
        for(var i = 0; i < add.children.length; i++){
            if(add.children[i].tfID == add.edit_button){
                add.children[i].text = str

            }
        }
    }

    function getInput() {
        var id = add.edit_button
        var name
        var obj

        for(var i = 0; i < add.children.length; i++){
            if(add.children[i].tfID == id){
                obj = add.children[i]
            }

            if(add.children[i].textID == id) {
                name = add.children[i].text
            }
        }
        slay.lastIndex = background.add
        slay.edit(obj, name)
    }

    function oneLine(text){
        clear()
        listView.visible = false
        text3.visible = false
        text2.text = text
        text2.visible = true
        textField2.visible = true
        button2.visible = true
        text1.visible = false
        textField1.visible = false
        button1.visible = false
    }


    RadioButton{
        id: button_ressource
        x: 41
        y: 60
        width: 200
        height: 45
        checked: true
        text: "Ressource"
        onClicked: {
            button_department.checked = false
            button_person.checked = false
            rbutton = 0
            oneLine("Ressource")
        }

    }

    RadioButton{
        text: "Department"
        id: button_department
        x: 41
        y: 159
        width: 200
        height: 45
        checked: false
        onClicked: {
            button_ressource.checked = false
            button_person.checked = false
            rbutton = 1
            oneLine("Department")
        }
    }

    RadioButton{
        id: button_person
        x: 41
        y: 261
        width: 200
        height: 45
        text: "Person"
        checked: false
        onClicked: {
            button_ressource.checked = false
            button_department.checked = false
            rbutton = 2
            clear()

            slay.load_section()
            listView.visible = true
            text3.visible = true
            text2.text = "Last name"
            text2.visible = true
            textField2.visible = true
            text1.text = "First name"
            text1.visible = true
            textField1.visible = true
            button1.visible = true
            button2.visible = true
        }
    }

    Text {
        id: text1
        property int textID: 0
        x: 283
        y: 72
        width: 124
        height: 21
        visible: false
        font.pixelSize: 15

    }

    TextField {
        id: textField1
        x: 404
        y: 70
        property int tfID: 0
        visible: false
    }

    Text {
        id: text2
        property int textID: 1
        x: 283
        y: 171
        width: 129
        height: 21
        text: qsTr("Ressource")
        font.pixelSize: 15

    }

    TextField {
        id: textField2
        x: 404
        y: 169
        property int tfID: 1
    }



    Button {
        id: button_save
        x: 484
        y: 418
        text: qsTr("Save")
        onClicked: {

            var urlComponent
            var comp = {}
            switch(rbutton){
                case 0: //Resource
                        urlComponent = "resources"
                        comp.resource = textField2.text
                        break

                case 1: //Department
                        urlComponent = "department"
                        comp.section = textField2.text
                        break

                case 2: //Person
                        urlComponent = "persons"
                        comp.firstName = textField1.text
                        comp.lastName = textField2.text
                        comp.section = department_model.get(listView.currentIndex).section
                        break
            }

            console.log(JSON.stringify(comp) + " " + urlComponent)
            add_component(JSON.stringify(comp), urlComponent)
            clear()

        }
        function add_component(strData, urlComponent){

           var req = new XMLHttpRequest();
           req.open("POST", url + "sql_post/" + urlComponent);
           //req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
           req.setRequestHeader("Content-type","application/json")
            req.onreadystatechange = function() {
             if (req.readyState == XMLHttpRequest.DONE) {
                 console.log(req.responseText)
                 if(req.responseText == "Insert Done"){
                     slay.currentIndex = background.home
                     return
                 }
                 console.log("Failed Insertion")
             }
           }
           req.onerror = function(){
               console.log("error")
           }
            //console.log(req.toString())
            console.log(strData)
            req.send(strData)
        }
    }

    Button {
        id: button_cancel
        x: 628
        y: 418
        text: qsTr("Cancel")

        onClicked: function() {
            clear()
            slay.currentIndex = background.home
        }
    }

    Button {
        id: button1
        x: 628
        y: 69
        visible: false
        text: qsTr("Edit")
        onClicked: function(){
            add.edit_button = 0
            getInput()
        }
    }

    Button {
        id: button2
        x: 628
        y: 168
        visible: true
        text: qsTr("Edit")
        onClicked: function(){
            add.edit_button = 1
            getInput()
        }
    }


    ListView {
        id: listView
        x: 486
        y: 248
        width: 227
        height: 154
        visible: false

        model: department_model
        delegate: Item{
            width: parent.width
            height: 30

            Text{
                text:   section
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    console.log("clicked " + index)
                    listView.currentIndex = index
                }

            }
        }

        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        focus: true


    }

    ListModel {
        id: department_model
    }

    Connections {
        target: slay
        onLoad_section: {
            get_sections(function(r){
                var depar = JSON.parse(r)
                department_model.clear()
                for(var i = 0; i < depar.length; i++){
                    department_model.append(depar[i])
                }
            }
            )}
    }

    Text {
        id: text3
        x: 283
        y: 272
        width: 124
        height: 24
        visible: false
        text: qsTr("Department")
        font.pixelSize: 14
        verticalAlignment: Text.AlignVCenter
    }

    function get_sections(callback){
        var req = new XMLHttpRequest();
        req.open("GET", background.url + "sql_get/department");

        req.onreadystatechange = function() {
          if (req.readyState === XMLHttpRequest.DONE) {
              var r = JSON.parse(req.responseText)
              console.log(JSON.stringify(req.responseText))
              callback(JSON.stringify(r))
          }
        }
        req.onerror = function(){
            console.log("get_department ERROR")
        }
        req.send()
    }
}
