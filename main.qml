import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.3

Window {
    visible: true
    width: 640
    height: 440
    title: qsTr("REVERSI")
    property bool initialized: false
    function initialize()
    {
        initialized = true
    }

    RowLayout {
        anchors.fill: parent

        Board {
            id : board
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            visible: initialized
        }

        Home {
            id : home
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            visible: !initialized
            height: parent.height
            width: parent.width
        }

        Controls {
            id : controls
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            visible: initialized
        }
    }

}

