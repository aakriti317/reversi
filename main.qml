import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.3

Window {
    visible: true
    width: 640
    height: 440
    title: qsTr("REVERSI")
    RowLayout {
        anchors.fill: parent

        Board {
            id : board
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }
        Controls {
            id : controls
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }
    }

}

