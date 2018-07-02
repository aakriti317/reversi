import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
    signal start_signal(int player1_type,int player2_type,int player1_intelligence,int player2_intelligence)

    property bool player1_intelligence_bar: false
    property bool player2_intelligence_bar: false
    property int player1_type_value: 0
    property int player2_type_value: 0


    objectName: "home"
    id : home
        Row {
            id: row

            width: parent.width
            height: parent.height
            spacing: 0
            Column {
                id : player1
                width : parent.width / 2
                height: parent.height
                spacing: 20

                Text {
                    id: text1
                    text: qsTr("Player 1")
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    style: Text.Raised
                    font.weight: Font.Black
                    font.pixelSize: 20

                }
                Row{
                    id : player1_type_row
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20



                    Text{
                        text: qsTr("Type")
                    }
                    ExclusiveGroup {id: player1_type }
                    RadioButton{
                        text : qsTr("HUMAN")
                        checked: true
                        exclusiveGroup: player1_type
                        onClicked: player1_type_value = 0;
                        onCheckedChanged: if(checked)
                                              player1_intelligence_bar = false

                    }
                    RadioButton{
                        text : qsTr("COMPUTER")
                        exclusiveGroup: player1_type
                        onClicked: player1_type_value = 1
                        onCheckedChanged: if(checked)
                                              player1_intelligence_bar = true

                    }
                }
                Row{
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20
                    visible: player1_intelligence_bar



                    Text{
                        text: qsTr("Intelligence Level")
                    }
                    Slider {
                        id : intelligence1
                        maximumValue: 5
                        minimumValue: 1
                        value: 1
                        stepSize: 1

                    }
                    Text{
                        text: intelligence1.value
                    }

                }


            }
            Column {
                id : player2
                width : parent.width / 2
                height: parent.height
                spacing: 20

                Text {
                    id: text2
                    text: qsTr("Player 2")
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    style: Text.Raised
                    font.weight: Font.Black
                    font.pixelSize: 20

                }
                Row{
                    id : player2_type_row
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20



                    Text{
                        text: qsTr("Type")
                    }
                    ExclusiveGroup {id: player2_type }
                    RadioButton{
                        text : qsTr("HUMAN")
                        checked: true
                        exclusiveGroup: player2_type
                        onClicked: player2_type_value = 0
                        onCheckedChanged: if(checked)
                                              player2_intelligence_bar = false

                    }
                    RadioButton{
                        text : qsTr("COMPUTER")
                        exclusiveGroup: player2_type
                        onClicked: player2_type_value = 1
                        onCheckedChanged: if(checked)
                                              player2_intelligence_bar = true
                    }
                }
                Row{
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20
                    visible: player2_intelligence_bar

                    Text{
                        text: qsTr("Intelligence Level")
                    }
                    Slider {
                        id : intelligence2
                        maximumValue: 5
                        minimumValue: 1
                        value: 1
                        stepSize: 1

                    }
                    Text{
                        text: intelligence2.value
                    }

                }

                Button {
                    text: qsTr("START")
                    onClicked: {
                                start_signal(player1_type_value,player2_type_value,intelligence1.value,intelligence2.value)
                    }
                }

            }
        }


    }
