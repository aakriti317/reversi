import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3

Item
{
    id : controls
    width: 200
    height: 400
    ColumnLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.fill: parent
        ColumnLayout{
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true

            Button {
                text: "START"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                highlighted: true
            }
            Button {
                text: "QUIT"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
            }
        }





        GridLayout
        {
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

            columns: 2
            Label
            {
                text: "SCOREBOARD"
                Layout.columnSpan: 2
            }


            Label{
                text: "BLACK:"
            }
            Label{
                text: "22"
            }
            Label{
                text: "WHITE:"
            }
            Label{
                text: "40"
            }
        }
    }
}

