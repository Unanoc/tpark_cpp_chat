import QtQuick 2.4
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

// Содержимое на слайдер-меню
Item {
    id: menu

    // Сигнал на нажатие (event)
    signal menuItemClicked(string item, string page)
    property alias currentItem: listViewMenu.currentIndex

    // Кнопки на слайдер-меню
    ListModel {
        id: modelMenu
        ListElement {
            item: "new_groupchat"
            icon: "qrc:/Resources/images/icon_groupchat.png"
        }
        ListElement {
            item: "add_contact"
            icon: "qrc:/Resources/images/icon_addcontact.png"
        }
        ListElement {
            item: "contacts"
            icon: "qrc:/Resources/images/icon_contacts.png"
        }
        ListElement {
            item: "important"
            icon: "qrc:/Resources/images/icon_important.png"
        }
        ListElement {
            item: "settings"
            icon: "qrc:/Resources/images/icon_settings.png"
        }
    }

    // Формируем название для каждого пункта в слайдер-меню
    function textItemMenu(item) {
        var textReturn = ""
        switch (item) {
            case "new_groupchat":
                textReturn = qsTr("New group chat")
                break;
            case "add_contact":
                textReturn = qsTr("Add contact")
                break;
            case "contacts":
                textReturn = qsTr("Contacts")
                break;
            case "important":
                textReturn = qsTr("Favorites")
                break;
            case "settings":
                textReturn = qsTr("Settings")
                break;
        }

        return textReturn
    }

    // Изображение над кнопками -- с аватаром пользователя
    Rectangle {
        id: logoWrapper
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        width: parent.width
        height: width * 0.5
        color: palette.primary
        clip: true
        Image {
            id: imgLogo
            source: "qrc:/Resources/images/background.jpg"
            height: parent.height
            width: parent.width
            antialiasing: true
            smooth: true
            anchors.top: parent.top
            anchors.left: parent.left
            opacity: 0.5
        }
    }
    
    ListView {
        id: listViewMenu
        anchors.top: logoWrapper.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true
        model: modelMenu
        delegate: componentDelegate
    }

    Component {
        id: componentDelegate

        Rectangle {
            id: wrapperItem
            height: 60
            width: parent.width
            color: wrapperItem.ListView.isCurrentItem || ma.pressed ? palette.currentHighlightItem: "transparent"
            Image {
                id: imgItem
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 0.5 * imgItem.width
                height: parent.height * 0.5
                width: height
                source: icon
                visible: icon != ""
                smooth: true
                antialiasing: true
            }

            Label {
                id: textItem
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: imgItem.right
                anchors.leftMargin: 0.7 * imgItem.width
                text: textItemMenu(item)
                font.pixelSize: parent.height * 0.3
                color: wrapperItem.ListView.isCurrentItem ? palette.darkPrimary : palette.primaryText
            }

            MouseArea {
                id: ma
                anchors.fill: parent
                enabled: app.menuIsShown
                onClicked:  {
                    if (item == "settings") {
                        settingsPopup.open()
                    }

                    //menu.menuItemClicked(item, page)
                    //listViewMenu.currentIndex = index
                }
            }
        }
    }

    // Окно настроек
    Popup {
        id: settingsPopup
        x: (app.width - width) / 2 + menuWidth
        y: (app.height - height) / 2
        width: app.width * 0.5
        height: app.height
        padding: 0

        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        Column {
            Rectangle {
                id: header
                color: palette.primary
                width: settingsPopup.width
                height: 50

                Label {
                    id: settingsLabel
                    x: 25
                    y: 10
                    text: "Settings"
                    font.pixelSize: parent.height * 2 * 0.2
                    font.bold: true
                    color: palette.text
                }

                Button {
                    id: closeBtn
                    text: ""
                    width: 20
                    height: 20
                    property string source: "qrc:/Resources/images/cross.png"
                    x: settingsPopup.width - width - 25
                    y: 10

                    onClicked: {
                        settingsPopup.close()
                    }

                    background: {
                        //color: closeBtn.hovered ? palette.primary : palette.lightPrimary
                        color: palette.primary // Проблема с assing'ом цвета на фон
                    }

                    Image {
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        source: parent.source
                    }
                }
            }

            // Информация о пользователе
            Rectangle {
                id: userInfo
                color: palette.primary
                width: settingsPopup.width
                height: settingsPopup.height * 0.3

                Image {
                    source: "qrc:/Resources/images/icon_avatarmaleinv.png"
                    id: userAvatar
                    property bool rounded: true
                    property bool adapt: true

                    layer.enabled: rounded
                    layer.effect: OpacityMask {
                        maskSource: Item {
                            width: userAvatar.width
                            height: userAvatar.height

                            Rectangle {
                                anchors.centerIn: parent
                                width: userAvatar.adapt ? userAvatar.width : Math.min(userAvatar.width, userAvatar.height)
                                height: userAvatar.adapt ? userAvatar.height : width
                                radius: Math.min(width, height)
                            }
                        }
                    }

                    height: parent.height * 0.6
                    width: parent.height * 0.6
                    anchors.left: parent.left
                    anchors.leftMargin: 0.6 * userAvatar.width
                    anchors.top: parent.top
                    anchors.topMargin: 0.2 * userAvatar.height
                    antialiasing: true
                    smooth: true
                }

                Label {
                    id: userName
                    font.pixelSize: 20
                    color: palette.text
                    x: userAvatar.width + userAvatar.x + 30
                    y: userAvatar.y + 5
                    text: "#USERNAME"
                }

                Label {
                    id: userStatus
                    font.pixelSize: 13
                    color: palette.currentHighlightItem
                    x: userAvatar.width + userAvatar.x + 30
                    y: userAvatar.y + 35
                    text: "Online"
                }

                Button {
                    id: chooseAvatar
                    text: "Choose avatar"
                    font.pixelSize: 18
                    x: userAvatar.width + userAvatar.x + 30
                    y: userStatus.y + 35

                    contentItem: Text {
                        text: chooseAvatar.text
                        color: palette.text
                        font.pixelSize: chooseAvatar.font.pixelSize
                    }

                    background: Rectangle {
                       color: chooseAvatar.hovered ? palette.currentHoveredItem : palette.currentHighlightItem
                       radius: 5
                    }
                }

                Button {
                    id: changeInfo
                    text: "Change info"
                    font.pixelSize: 18
                    x: chooseAvatar.x + chooseAvatar.width + 30
                    y: userStatus.y + 35

                    contentItem: Text {
                        text: changeInfo.text
                        color: palette.text
                        font.pixelSize: changeInfo.font.pixelSize
                    }

                    background: Rectangle {
                        color: changeInfo.hovered ? palette.currentHoveredItem2 : palette.primary
                        radius: 5
                    }
                }
            }

            Rectangle {
                id: separator
                width: settingsPopup.width
                color: "white"
                height: 10
            }

            // Настройки приложения
            Rectangle {
                id: settingsInfo
                color: palette.primary
                width: settingsPopup.width
                y: separator.height + userInfo.height
                height: settingsPopup.height - y

                Label {
                    text: "Information"
                    color: palette.text
                    font.pixelSize: settingsLabel.font.pixelSize
                    font.bold: true
                    x: settingsLabel.x
                    y: parent.y - 255
                }

                Label {
                    text: "Phone number:"
                    color: palette.darkText
                    font.pixelSize: 14
                    y: parent.y - 210
                    x: settingsLabel.x
                }

                Label {
                    text: "+X-XXX-XXX-XXX"
                    color: palette.currentHoveredItem
                    font.pixelSize: 14
                    y: parent.y - 210
                    x: settingsLabel.x + 110
                }

                Label {
                    text: "User Name:"
                    color: palette.darkText
                    font.pixelSize: 14
                    y: parent.y - 180
                    x: settingsLabel.x
                }

                Label {
                    text: "@userlogin"
                    color: palette.currentHoveredItem
                    font.pixelSize: 14
                    y: parent.y - 180
                    x: settingsLabel.x + 110
                }

                Label {
                    text: "About you:"
                    color: palette.darkText
                    font.pixelSize: 14
                    y: parent.y - 150
                    x: settingsLabel.x
                }

                Label {
                    text: "None"
                    color: palette.currentHoveredItem
                    font.pixelSize: 14
                    y: parent.y - 150
                    x: settingsLabel.x + 110
                }
            }
        }

        onAboutToShow: {
            color: "black"
            opacity: app.menuProgressOpening * 0.5
            onMenu()
        }

        enter: Transition {
            NumberAnimation {
                property: "opacity"
                from: 0.0
                to: 1.0
            }
        }

        exit: Transition {
            NumberAnimation {
                property: "opacity"
                from: 1.0
                to: 0.0
            }
        }
    }
}
