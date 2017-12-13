import QtQuick 2.0

Item {
    signal piece_added_signal(int row,int column,string color)
    property string current_color: "BLACK"

    function change_color(row,column,color)
    {
        var i=row*8+column
        if(grid.children[i].occupied)
            grid.children[i].piece_color=color
    }
    function occupy_cell(row,column,color)
    {
        var i=row*8+column
        if(!grid.children[i].occupied)
        {
            grid.children[i].piece_color=color
            grid.children[i].occupied=true
        }
    }
    objectName: "board"
    id : board
    width: 400
    height: 400
    Grid {
        id: grid
        rows: 8
        columns: 8
        Repeater {
            model: 64
            Rectangle {
                property int row: Math.floor(index/8)
                property int column: index%8
                property string piece_color: ""
                property bool occupied: false
                objectName: "cell"+row+column
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
                            parent.piece_color=current_color
                            piece_added_signal(row,column,current_color)
                            if(current_color=="BLACK")
                                current_color="WHITE"
                            else
                                current_color="BLACK"

                        }
                    }
                }
            }
        }
    }
}
