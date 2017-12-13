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
                property int row: Math.floor(index/8)
                property int column: index%8
                property string piece_color: ""
                property bool occupied: false
                property string uid: "rectangle"+row+column
                id: uid
                width : 50
                height :50
                border.color: "#000000"
                color : (row + column) % 2 ? "#95582C" : "#E5CB7E"
                Piece {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    state: piece_color

                    visible: occupied

                }
                Component.onCompleted:{
                    if((row==3 && column==3) || (row==4 && column==4)){
                        piece_color="WHITE"
                        occupied=true
                    }
                    else if ((row==3 && column==4) || (row==4 && column==3)){
                        piece_color="BLACK"
                        occupied=true
                    }
                    else
                        occupied=false
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if(!parent.occupied)
                        {
                            parent.occupied=true
                            parent.piece_color="WHITE"

                        }
                    }
                }
            }
        }
    }
}
