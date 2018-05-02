import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4

MyPage {
    id: mainPage

    // Связь с плюсами
    Connections {
        target: receiver
        onAppendChat: {
            chatModel.append(chat)
        }
    }

    // Модель списка сообщений чата
    ListModel {
        id: messageModel

        ListElement {
            sender: "#SENDER"
            message: "Lorem ipsum"
            avatar: "qrc:/Resources/images/icon_avatarmale.png"
            //time: date
        }
    }

    // Модель списка чатов
    ListModel {
        id: chatModel

        // Тестовые данные
        ListElement {
            name: "#MALEUSERNAME"
            message: "Lorem ipsum dolor sit amet, consectet..."
            avatar: "qrc:/Resources/images/icon_avatarmaleinv.png"
        }

        ListElement {
            name: "#FEMALEUSERNAME"
            message: "Lorem ipsum dolor sit amet, consectet..."
            avatar: "qrc:/Resources/images/icon_avatarfemaleinv.png"
        }

        ListElement {
            name: "#CHATNAME"
            message: "Lorem ipsum dolor sit amet, consectet..."
            avatar: "qrc:/Resources/images/icon_avatargroupinv.png"
        }
    }

    // Колонка с чатами
    Row {
        id: dialogues
        x: parent.x
        y: parent.y
        width: parent.width * 0.3
        height: parent.height

        Column {
            width: parent.width
            height: parent.height

            Rectangle {
                width: parent.width
                height: parent.height
                color: palette.darkPrimary

                Item {
                    id: chatList
                    width: parent.width
                    height: parent.height

                    ListView {
                        id: chatListView
                        anchors.fill: parent
                        model: chatModel
                        clip: true
                        delegate: chatDelegate
                        ScrollBar.vertical: ScrollBar {
                            active: true
                            interactive: false
                        }

                        boundsBehavior: Flickable.StopAtBounds
                    }
                }
            }
        }
    }

    // Колонка с конкретным чатом
    Row {
        id: currentDialogue
        x: dialogues.width
        y: parent.y
        width: parent.width * 0.7
        height: parent.height
/*
        Rectangle {
            id: chatPanel
            x: parent.x
            color: palette.primary
            height: 50
            width: 50
        }


*/

        // Панель набора сообщения
        Item {
            anchors.bottom: currentDialogue.bottom
            //anchors.top: parent.bottom
            Rectangle {
                id: writePanel
                color: palette.darkPrimary
                height: 40
                width: parent.parent.width
                y: parent.parent.y

                Button {
                    id: attach
                    x: 0
                    y: 0
                    text: "Attach"
                    onClicked: { // Пока тестовая функция с сигналом
                        receiver.appendChat({"name": "#TEST", "message": "Lorem Ipsum", "avatar": "qrc:/Resources/images/icon_avatarmaleinv.png"});
                    }
                }

                TextField {
                    id: messageField
                    anchors.fill: parent
                    visible: true
                    wrapMode: "Wrap"
                    placeholderText: "Write your message..."
                    anchors.rightMargin: 100
                    anchors.leftMargin: 99
                }

                Button {
                    id: send
                    x: 740
                    y: 0
                    text: "Send"
                    //x: messageField.x + messageField.width
                }
            }
        }



        Rectangle {
            id: chatPanel
            height: 50
            width: 50
            color: palette.primary
            //x: parent.x
            //y: parent.y
        }
    }

    // Делегат для отображения данных на списке чатов
    Component {
        id: chatDelegate

        Rectangle {
            id: chatItem
            width: parent.width
            height: 70
            color: palette.darkPrimary

            Image {
                id: chatAvatar
                property bool rounded: true
                property bool adapt: true

                layer.enabled: rounded
                layer.effect: OpacityMask {
                    maskSource: Item {
                        width: chatAvatar.width
                        height: chatAvatar.height

                        Rectangle {
                            anchors.centerIn: parent
                            width: chatAvatar.adapt ? chatAvatar.width : Math.min(chatAvatar.width, chatAvatar.height)
                            height: chatAvatar.adapt ? chatAvatar.height : width
                            radius: Math.min(width, height)
                        }
                    }
                }

                source: avatar
                height: parent.height * 0.8
                width: parent.height * 0.8
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 0.3 * chatAvatar.width
                antialiasing: true
                smooth: true
            }

            Label {
                id: chatName

                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: chatAvatar.right
                anchors.leftMargin: 0.3 * chatAvatar.width

                font.pixelSize: parent.height * 0.2
                font.bold: true

                color: palette.text
                text: name
            }

            Label {
                id: chatMessage
                font.pixelSize: parent.height * 0.2

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.left: chatAvatar.right
                anchors.leftMargin: 0.3 * chatAvatar.width

                color: palette.text

                text: message
            }

            MouseArea {
                id: ma
                anchors.fill: parent
                hoverEnabled: true

                // TODO: дополнить кнопки логикой
                onEntered: {
                    if (!menuIsShown) {
                        parent.color = palette.currentHoveredItem
                    }
                }

                onExited: {
                    if (!menuIsShown) {
                        parent.color = palette.darkPrimary
                    }
                }

                onClicked: {
                    parent.color = palette.currentHighlightItem
                    showChat();
                }
            }

            PropertyAnimation {
                id: clickAnimation
                target: chatItem

                // TODO: допилить анимацию при нажатии
            }
        }
    }

    // Делегат для отображения сообщения
    Component {
        id: messageDelegate

        Rectangle {
            id: messageItem
            height: 50
            color: palette.darkPrimary
        }
    }
}
