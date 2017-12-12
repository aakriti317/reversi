import QtQuick 2.0

Item {
    id : board
    width: 400
    height: 400
    Grid {
        rows: 8
        columns: 8
        Repeater {
            model: 64
            Rectangle {
                id: rectangle
                width : 50
                height :50
                border.color: "#000000"
                color : (Math.floor(index/8) + (index%8)) % 2 ? "#95582C" : "#E5CB7E"
                Piece {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    state: (Math.floor(index/8) + (index%8)) % 2 ? "BLACK" : ""
                }

                MouseArea {

                }
            }
        }
    }
}
