import QtQuick 2.0


Rectangle {
    id: piece
    width: 20
    height: 20
    radius: 10
    border.width: 1
    color: "white"

    states: [
        State {
            name: "BLACK"
            PropertyChanges {
                target: piece
                color: "black"
            }
        }

    ]

}
