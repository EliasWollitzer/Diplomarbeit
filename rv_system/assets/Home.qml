import QtQuick 2.5
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3
import "."
import "./keyboard"

Item {
    width:800
    id: home

    Calendar {
        id: calendar
        x: 400         //Change for relationship
        y: parent.y
        width: 400
        height: parent.height
        onClicked: function(date){
            var d = JSON.stringify(date).substring(1,11)
            console.log(d)
            slay.curDate = d
            get_list()
        }



        function get_list(){    //for one date
            var date = slay.curDate
            var req = new XMLHttpRequest();
            req.open("GET", background.url + "sql_get?date=" + slay.curDate);
            req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

            req.onreadystatechange = function() {
                if (req.readyState == 4) {
                    if (req.status == 200) {
                        var arr = JSON.parse(req.responseText)

                        entrymodel.clear()

                        for(var i = 0; i < arr.length; i++){
                            arr[i].timefrom = arr[i].datefrom.substring(11,16)
                            arr[i].timeto = arr[i].dateto.substring(11,16)
                            arr[i].datefrom = slay.stringToDate(arr[i].datefrom)
                            arr[i].dateto = slay.stringToDate(arr[i].dateto)
                            console.log(JSON.stringify(arr[i]))
                            entrymodel.append(arr[i])
                        }
                    } else {
                        console.log("error: " + req.status)
                    }
                }
            }
            req.onerror = function(){
                console.log("get_list ERROR")
            }

            req.send()

        }
    }

    Connections{
        target: slay
        onReload_list: {
            calendar.get_list()
        }
    }

    TableView {
        id: tableView
        x: parent.x
        y: parent.y
        height: 350
        width: 400
        model: entrymodel


        TableViewColumn {
            role: "resource"; title: "Ressource"; width: 218
        }

        TableViewColumn {
            role: "timefrom"; title: "From"; width: 90
        }
        TableViewColumn {
            role: "timeto"; title: "To"; width: 90
        }
        onClicked: function(row){
            slay.get_ressources(function(r){
                slay.resources_loaded(r, background.edit, JSON.stringify(entrymodel.get(row)))
                slay.currentIndex = background.newentry
                })
        }
    }

    ListModel {
            id: entrymodel
    }

    Button {
        id: new_entry_button
        x: 0
        y: 349

        width: 400
        height: 65
        text: "New Entry"
        onClicked: function() {
            slay.get_ressources(function(r){
                    slay.resources_loaded(r, background.neww, "")
                    slay.currentIndex = background.newentry
            })
        }
    }

    Button {
        id: add_component_button
        x: 0
        y: 415

        width: 400
        height: 65
        text: "Add Component"

        onClicked: function() {
            slay.load_section()
            slay.currentIndex = background.add

        }
    }
}
