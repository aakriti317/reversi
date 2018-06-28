import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4

Item
{
    objectName: "controls"
    function set_score_board(b_score,w_score)
    {
        black_score.text=b_score
        white_score.text=w_score
    }
    function status_update(message)
    {
        status.text = message + "\n" + status.text
    }

    id : controls
    width: 200
    height: 400
    ColumnLayout
    {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.fill: parent
        ColumnLayout
        {
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true

            Button
            {
                text: "START"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                //highlighted: true
            }
            Button
            {
                text: "QUIT"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
            }
            Label
            {
                text: "STATUS"
                font.bold: true

            }

            TextArea
            {
                id: status
                text: ""
                readOnly: true
                Layout.fillHeight: true
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


            Label
            {
                text: "BLACK:"
            }
            Label
            {
                id: black_score
                text: "2"
            }
            Label
            {
                text: "WHITE:"
            }
            Label
            {
                id: white_score
                text: "2"
            }
        }
    }
}

