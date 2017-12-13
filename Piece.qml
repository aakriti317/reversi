import QtQuick 2.0


Rectangle {

    id: piece
    width: 25
    height: 25
    radius: 13
    border.width: 1


    states: [
        State {
            name: "BLACK"
            PropertyChanges {
                target: piece
                color: "black"
            }

        },
        State {
            name: "WHITE"
            PropertyChanges {
                target: piece
                color: "white"

            }
        }

    ]

}
