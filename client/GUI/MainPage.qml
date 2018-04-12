import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0

MyPage {
    id: mainPage

    // Динамически подгружаемые чаты
    ListModel {
        id: chatModel

        // Здесь нужно заполнять динамически для каждого чата
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

    // Колонка с конкретным чатом
    Row {
        id: currentDialogue
        x: dialogues.width
        y: parent.y
        width: parent.width * 0.7
        height: parent.height

        Rectangle {
            id: chatPanel
        }


        // Панель набора сообщения
        Rectangle {
            id: messagePanel
            color: palette.darkPrimary
            anchors.bottom: parent.bottom
            height: 40
            width: parent.width
            y: parent.y

            // TODO: Исправить относительные позиции элементов
            Button {
                id: attach
                x: 0
                y: 0
                text: "Attach"
                //x: parent.x
            }

            TextArea {
                id: messageField
                anchors.fill: parent
                visible: true
                wrapMode: TextArea.Wrap
                placeholderText: "Write your message..."
                color: messageField.focus == true ? palette.darkText : palette.text
                anchors.rightMargin: 99
                anchors.leftMargin: 100
                //width: parent.width * 0.7
                //x: attach.x + attach.width
            }

            Button {
                id: send
                x: 740
                y: 0
                text: "Send"
                //x: messageField.x + messageField.width
            }

        // TODO: Заполнить
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
                        parent.color = palette.currentHighlightItem
                    }
                }

                onExited: {
                    if (!menuIsShown) {
                        parent.color = palette.darkPrimary
                    }
                }

                onClicked: {
                    clickAnimation
                }
            }

            PropertyAnimation {
                id: clickAnimation
                target: chatItem

                // TODO: допилить анимацию при нажатии
            }
        }
    }
}
