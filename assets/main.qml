import QtQuick 2.5
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3
import "."
import "./keyboard/"


ApplicationWindow {
    id: background
    visible: true
    width: 800
    height: 480
    color: "white"

    property int home: 0
    property int newentry: 1
    property int add: 2
    property int dateSelection: 3
    property int input: 4

    property int neww: 0
    property int edit: 1
    property string url: "http://192.168.0.26:30000/"

    StackLayout{
            id: slay
            currentIndex: home
            anchors.fill:   parent

            property int lastIndex: home

            signal ent_pressed(string str)
            signal edit(var obj, string name)

            property string curDate: "2018-04-01"
            signal resources_loaded(string o, int state, string data)
            signal load_section()

            signal select_date(var obj)

            Home {}

            Newentry {}

            Add {}

            DateSelection{}

            Input {}

            function get_ressources(callback){
                var req = new XMLHttpRequest();
                req.open("GET", background.url + "sql_get/resources");

                req.onreadystatechange = function() {
                  if (req.readyState === XMLHttpRequest.DONE) {
                      var r = JSON.parse(req.responseText)
                      console.log(JSON.stringify(r))
                      callback(JSON.stringify(r))
                  }
                }
                req.onerror = function(){
                    console.log("get_ressources ERROR")
                }
                req.send()
            }

            function stringToDate(date){
                var d = JSON.stringify(date).substring(1,11)
                var y = d.substring(0,4)
                var m = d.substring(5,7)
                var d = d.substring(8,10)

                return d + "-" + m + "-" + y
            }

            function dateToString(date){
                var d = date.substring(0,2)
                var m = date.substring(3,5)
                var y = date.substring(6,10)

                return y + "-" + m + "-" + d
            }

        }
}
