import QtQuick 2.5
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3
import "."
import "./keyboard"

Item {
    anchors.fill: parent

    Calendar {
        id: calendar
        x: 0
        y: 0
        width: 800
        height: 480

        property var dateText

        onDoubleClicked: {
            console.log("double")
            var d = JSON.stringify(date).substring(1,11)
            var d  = slay.stringToDate(d)
            console.log(d)
            dateText.text = d
            slay.currentIndex = background.newentry
        }

        Connections{
            target: slay
            onSelect_date: {
                var d = slay.dateToString(obj.text)
                calendar.selectedDate = d
                calendar.dateText = obj
            }
        }
    }
}
