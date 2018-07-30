import QtQuick 2.5
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3
import "."
import "./keyboard"

Item {
    id: item1
    property var oldObject: ({})

    function new_entry(){
        notification.text = ""
        lv.visible = true
        resource_text.text = (typeof resource_model.get(0) === "undefined") ? ""  : resource_model.get(0).resource
        text_select.visible = true
        button_delete.visible = false
        textArea_description.text = ""
        slid_h.value = 0.5
        slid_min.value = 0.5
        from_text.text = slay.stringToDate(slay.curDate)
        to_text.text = slay.stringToDate(slay.curDate)
        time_from.text = "12:30"
        time_to.text = "12:30"
        pointer_from.visible = true
        pointer_to.visible = false
        button_changeDate.state = "FROM"
        button_changeDate.sliderValues = [{"h":0.5, "min":0.5},{"h":0.5, "min":0.5}]
        resource_model.index = 0
        name_model.index = 0
        department_model.index = 0
        name_text.text = ""
        department_text.text = ""
        lv.model = resource_model
        lv.model_index = 0
        text_select.text = "Select resource"

    }

    function edit_entry(str){
        notification.text = ""
        var obj = JSON.parse(str)
        oldObject = obj
        textArea_description.text = obj.description

        name_text.text = obj.firstName + " " + obj.lastName
        resource_text.text = obj.resource
        department_text.text = obj.section

        button_delete.visible = true
        lv.visible = false
        text_select.visible = false
        pointer_from.visible = false
        pointer_to.visible = true

        var h = obj.timeto.substring(0,2)
        var min = obj.timeto.substring(3,5)

        var hOld = obj.timefrom.substring(0,2)
        slid_h.value = h /23
        var minOld = obj.timefrom.substring(3,5)
        slid_min.value = min /55

        button_changeDate.sliderValues[0] = {"h": hOld/23, "min": minOld/55}

        from_text.text = obj.datefrom
        to_text.text = obj.dateto
        time_from.text = obj.timefrom
        time_to.text = obj.timeto

        button_changeDate.state = "TO"
}

    ListView{
        id: lv
        x: 24
        y: 230
        width: 278
        height: 221
        z: -1
        property int model_index: 0
        model: resource_model
        delegate: Item{
            width: parent.width
            height: 30
            id: lv_item

            Text{
                id: lv_text
                text:resource
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    lv.currentIndex = index

                    var modell

                    switch(lv.model_index){
                        case 0: modell = resource_model
                                break
                        case 1: modell = name_model
                                for(var i = 0; i < department_model.count; i++){
                                    if(department_model.get(i).resource == names.get(index).section){
                                        department_model.index = i
                                        department_model.display.text = names.get(index).section
                                    }
                                }

                                break
                        case 2: modell = department_model
                                break
                    }
                    modell.index = index
                    modell.display.text = modell.get(index).resource
                }
            }
        }
        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        focus: true
    }

    Text {
        x: 24
        y: 187
        width: 158
        height: 24
        id: text_select
        text : "Select ressource"
    }

    Text {
        id : text_desc
        x: 24
        y: 21
        width: 146
        height: 30
        text: "Description"
        verticalAlignment: Text.AlignVCenter
    }

    TextArea {
        id: textArea_description
        x: 24
        y: 65
        width: 278
        height: 88
        text: ""
    }

    Button {
        id: button_description
        x: 209
        y: 19
        width: 77
        height: 35
        text: qsTr("Edit")
        onClicked: {
            getInput(textArea_description, text_desc.text)
        }

        function getInput(obj,name,text){
            slay.lastIndex = background.newentry
            slay.edit(obj,name,text)
        }
}

    Button {
        id: button_save
        x: 484
        y: 418
        text: qsTr("Save")
        onClicked: {
            if(wrong_date()){
                return
            }

            var ls = [resource_text, name_text, department_text]
            if(slay.empty_fields(ls, notification)){
                return
            }

            lv.visible ? newEntry() : changeEntry()
        }

        function changeEntry(){
            var req = new XMLHttpRequest();
            req.open("POST", background.url + "sql_update/borrowed");
            req.setRequestHeader('Content-type','application/json');
            req.onreadystatechange = function() {
              if (req.readyState == XMLHttpRequest.DONE) {
                  if(req.responseText == "UPDATE done"){
                      slay.currentIndex = background.home
                      slay.reload_list()
                  }
                  else{
                      console.log("Time Collision")
                      server_error(req.responseText)
                  }
              }
            }
            req.onerror = function(){
              console.log("httpERROR")
              server_error("Connecton ERROR")
            }
            var e = {}
            e = oldObject
            e.datefrom = slay.dateToString(e.datefrom) + " " + e.timefrom
            e.dateto = slay.dateToString(e.dateto) + " " + e.timeto
            e.datefromNew = slay.dateToString(from_text.text) +  " " + time_from.text
            e.datetoNew = slay.dateToString(to_text.text) +  " " + time_to.text
            e.descriptionNew = textArea_description.text
            var str = JSON.stringify(e)
            req.send(str)
        }

        function newEntry(){
            var req = new XMLHttpRequest();
            req.open("POST", background.url + "sql_post/entry");
            req.setRequestHeader('Content-type','application/json');
            req.onreadystatechange = function() {
              if (req.readyState == XMLHttpRequest.DONE) {
                  if(req.responseText == "POST ENTRY DONE"){
                      slay.currentIndex = background.home;
                      slay.reload_list()
                  }
                  else{
                      console.log("Server ERROR")
                      server_error("Time collision!")
                  }
              }
            }
            req.onerror = function(){
              console.log("httpERROR")
                server_error("Connection ERROR")
            }
            var e = {}
            e.firstName = names.get(name_model.index).firstName
            e.lastName = names.get(name_model.index).lastName
            e.resource = resource_model.get(resource_model.index).resource
            e.section = department_model.get(department_model.index).resource
            e.description = textArea_description.text
            e.datefrom = slay.dateToString(from_text.text) +  " " + time_from.text
            e.dateto = slay.dateToString(to_text.text) +  " " + time_to.text
            var str = JSON.stringify(e)
            req.send(str)
        }

        function wrong_date(){
            var f = new Date(slay.dateToString(from_text.text) + "T" + time_from.text)
            var t = new Date(slay.dateToString(to_text.text) + "T" + time_to.text)

            if(f >= t){
                notification.text = "wrong date"
                return true
            }
            notification.text = ""
            return false
        }

        function server_error(str){
            notification.text = str
        }
    }

    Button {
        id: button_cancel
        x: 628
        y: 418
        text: qsTr("Cancel")

        onClicked: function() {
            slay.currentIndex = background.home
        }
    }

    Connections{
        id: con
        target: slay
        onResources_loaded:  {
            var resources = JSON.parse(o)
            resource_model.clear()
            for(var i = 0; i < resources.length; i++){
                resource_model.append(resources[i])
            }

            con.load_names(function(){
                con.load_departments(function(){
                    if(state === background.neww){
                        new_entry()
                    }else{
                        edit_entry(data)
                    }
                })
            })
        }

        function load_names(callback){
            var req = new XMLHttpRequest();
            req.open("GET", background.url + "sql_get/persons");
            req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

            req.onreadystatechange = function() {
                if (req.readyState == 4) {
                    if (req.status == 200) {
                        var arr = JSON.parse(req.responseText)
                        name_model.clear()
                        var o
                        for(var i = 0; i < arr.length; i++){
                            o = {}
                            o.resource = arr[i].firstName + " " + arr[i].lastName
                            name_model.append(o)
                            names.append(arr[i])
                        }
                        callback()
                    }
                }
            }
            req.send()
        }

        function load_departments(callback){
            var req = new XMLHttpRequest();
            req.open("GET", background.url + "sql_get/department");
            req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

            req.onreadystatechange = function() {
                if (req.readyState == 4) {
                    if (req.status == 200) {
                        var arr = JSON.parse(req.responseText)
                        department_model.clear()
                        var o
                        for(var i = 0; i < arr.length; i++){
                            o = {}
                            o.resource = arr[i].section
                            department_model.append(o)
                        }
                        callback()
                    }
                }
            }
            req.send()
        }
  }

    ListModel{ id: names } //for complete objects

    ListModel{
        id: name_model
        property var display: name_text
        property int index: 0
    }

    ListModel{
        id: department_model
        property var display: department_text
        property int index: 0
    }

    ListModel{
        id: resource_model
        property var display: resource_text
        property int index: 0
    }

    Text {
        id: text_fromN
        x: 340
        y: 150
        width: 45
        height: 29
        text: qsTr("From")
        font.pointSize: 11
        verticalAlignment: Text.AlignVCenter
    }

    Text {
        id: from_text
        x: 423
        y: 150
        width: 95
        height: 29
        text: "2018-01-01"
        verticalAlignment: Text.AlignVCenter
    }

    Text {
        id: text_toN
        x: 340
        y: 186
        width: 45
        height: 30
        text :  "To"
        verticalAlignment: Text.AlignVCenter
    }


    Text {
        id: to_text
        x: 423
        y: 187
        width: 95
        height: 29
        text: "2018-01-01"
        verticalAlignment: Text.AlignVCenter
    }

    Button {
        id: button_changeDate
        x: 693
        y: 164
        width: 85
        height: 35
        text: qsTr("From")
        onClicked: {
            sswitch(function(cb){

                var test = 1
                if(test == cb){
                    test = 0
                }

                sliderValues[cb] = {"h": slid_h.value, "min": slid_min.value}
                slid_h.value = sliderValues[test].h
                slid_min.value = sliderValues[test].min
            })
        }

        function sswitch(cb){
            var test = 0
            if(state == "TO"){
                state = "FROM"
                test = 1
            }else{
                state = "TO"
                test = 0
            }
            cb(test)
        }

        property var sliderValues: [{"h":0.5, "min":0.5},{"h":0.5, "min":0.5}]
        state: "FROM"
        states: [
            State {
                name: "TO"
                PropertyChanges {
                    target: pointer_from
                    visible: false
                }
                PropertyChanges {
                    target: pointer_to
                    visible: true
                }
                PropertyChanges {
                    target: button_changeDate
                    text: "FROM"
                }
            },
            State {
                name: "FROM"
                PropertyChanges {
                    target: pointer_from
                    visible: true
                }
                PropertyChanges {
                    target: pointer_to
                    visible: false
                }
                PropertyChanges {
                    target: button_changeDate
                    text: "TO"
                }
            }
        ]
    }

    Button {
        id: button_delete
        x: 340
        y: 418
        text: qsTr("Delete")
        visible: true
        enabled: true
        onClicked: function(){
            var req = new XMLHttpRequest();
            req.open("POST", background.url + "sql_delete");
            req.setRequestHeader('Content-type','application/json');
            req.onreadystatechange = function() {
              if (req.readyState == XMLHttpRequest.DONE) {
                  if(req.responseText == "DELETE DONE"){
                    slay.currentIndex = background.home;
                    slay.reload_list()
                  }
                  else{
                      console.log("ServerERROR")
                      server_error(req.responseText)
                  }
              }
            }
            req.onerror = function(){
              console.log("httpERROR")
              button_save.server_error("Connection ERROR")
            }
            var e = {}
            e = oldObject
            e.datefrom = slay.dateToString(e.datefrom) + " " + e.timefrom
            e.dateto = slay.dateToString(e.dateto) + " " + e.timeto
            var str = JSON.stringify(e)
            req.send(str)
        }
    }

    Button {
        id: button
        x: 616
        y: 221
        width: 162
        height: 35
        text: "Select Date"
        onClicked: {
            if(pointer_from.visible == true){
                slay.select_date(from_text)
            }else{
                slay.select_date(to_text)
            }
            slay.currentIndex = background.dateSelection
        }
    }

    Text {
        id: text1
        x: 340
        y: 242
        width: 115
        height: 29
        text: qsTr("Select hour")
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 11
    }

    Text {
        id: text2
        x: 340
        y: 321
        width: 115
        height: 29
        text: qsTr("Select minute")
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 11
    }

    Text {
        id: time_from
        x: 611
        y: 151
        width: 76
        height: 28
        text: ""
        font.pointSize: 11
        verticalAlignment: Text.AlignVCenter
    }

    Text {
        id: time_to
        x: 611
        y: 187
        width: 76
        height: 28
        text: ""
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 11
    }

    Text {
        id: pointer_from
        x: 391
        y: 151
        width: 31
        height: 29
        text: qsTr("->")
        z: 1
        verticalAlignment: Text.AlignVCenter
    }

    Text {
        id: pointer_to
        x: 391
        y: 187
        width: 31
        height: 29
        text: qsTr("->")
        visible: true
        verticalAlignment: Text.AlignVCenter
    }

    Button {
        id: button1
        x: 340
        y: 19
        width: 155
        height: 35
        text: "Resource"
        onClicked: {
            lv.model = resource_model
            text_select.text = "Select resource"
            lv.model_index = 0
            lv.currentIndex = resource_model.index
        }
    }

    Button {
        id: button2
        x: 340
        y: 60
        width: 155
        height: 35
        text: "Name"
        onClicked: {
            lv.model = name_model
            text_select.text = "Select name"
            lv.model_index = 1
            lv.currentIndex = name_model.index
        }
    }

    Button {
        id: button3
        x: 340
        y: 101
        width: 155
        height: 35
        text: "Department"
        onClicked: {
            lv.model = department_model
            text_select.text = "Select department"
            lv.model_index = 2
            lv.currentIndex = department_model.index
        }
    }

    Slider {
        id: slid_min
        x: 340
        y: 355
        width: 438
        height: 28
        value: 0.5
        onValueChanged: function(){
            var val = Math.round(slid_min.value * 11)
            min = val * 5
            slid_h.sliderChanged()
        }
        property int min: 30
    }

    Slider {
        id: slid_h
        x: 340
        y: 277
        width: 438
        height: 28
        value: 0.5
        onValueChanged: {
            hour = Math.round(slid_h.value * 23)
            sliderChanged()
        }
        function sliderChanged(){
            var h = hour
            var m = slid_min.min

            if(h < 10){
                h = "0" + h
            }
            if(m < 10){
                m = "0" + m
            }

            if(pointer_from.visible == true){
                time_from.text = h + ":" + m
            }else{
                time_to.text = h + ":" + m
            }
        }
        property int hour: 12
    }

    Text {
        id: resource_text
        x: 506
        y: 22
        width: 272
        height: 29
        text: ""
        verticalAlignment: Text.AlignVCenter
    }

    Text {
        id: name_text
        x: 506
        y: 63
        width: 272
        height: 29
        text: ""
        verticalAlignment: Text.AlignVCenter
    }

    Text {
        id: department_text
        x: 506
        y: 104
        width: 272
        height: 29
        text: ""
        verticalAlignment: Text.AlignVCenter
    }

    Text {
        id: notification
        x: 340
        y: 389
        width: 199
        height: 23
        color: "#f42222"
        text: ""
        font.pointSize: 9
        verticalAlignment: Text.AlignVCenter
    }
}
