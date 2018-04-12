import QtQuick 2.4
import QtQuick.Window 2.2
import Qt.labs.settings 1.0
import QtQuick.Controls 1.2

ApplicationWindow {
    id: app

    // Название приложения (для заголовка с слайдер-баром)
    //property string appTitle: "Pigeon Post"

    // Размер приложения
    width: 1200
    height: 720

    visible: true

    // Для отрисовки разными цветами
    QtObject {
        id: palette
        property color darkPrimary: "#282E33"
        property color primary: "#384147"
        property color lightPrimary: "#5A6772"
        property color text: "#F1F2F4"
        property color darkText: "#C6CAD2"
        property color accent: "#FFEB3B"
        property color primaryText: "#212121"
        property color secondaryText: "#727272"
        property color divider: "#B6B6B6"
        property color currentHighlightItem: "#009687"
        property color currentHoveredItem: "#00B3A1"
        property color currentHoveredItem2: "#70838f"
    }

    Settings {
        id: setting
    }

    property alias currentPage: loader.source

    property int durationOfMenuAnimation: 150                                           // Длительность анимации слайдер-бара
    property int menuWidth: app.width * 0.23                                            // Величина слайдер-бара в зависимости от размера окна
    property int widthOfSeizure: menuWidth * 10                                         // Величина размера для области клика
    property bool menuIsShown: Math.abs(menuView.x) < (menuWidth * 0.5) ? true : false  // Открытость слайдер-бара
    property real menuProgressOpening                                                   // Значение открытости слайдер-бара

    // Определяем наполнение окна приложения
    Rectangle {
        id: normalView
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        color: "white"

        // Определяем верхнюю панель окна приложения
        Rectangle {
            id: menuBar
            z: 5 // Для отображения поверх окон
            anchors.top: parent.top
            anchors.topMargin: 0
            width: parent.width
            height: 45
            color: palette.darkPrimary

            // Определяем кнопку слайдер-меню
            Rectangle {
                id: menuButton
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                width: 1.2 * height
                height: parent.height
                scale: maMenuBar.pressed? 1.2 : 1                                       // Изменяем размер кнопки при нажатии
                color: "transparent"

                // Анимация кнопки слайдер-меню
                MenuIconLive {  
                    id: menuBackIcon
                    scale: (parent.height / height) * 0.65                              // Изменяем размер иконки при анимации
                    anchors.centerIn: parent
                    value: menuProgressOpening
                }

                // Область на нажатие мышки, размером в кнопку
                MouseArea {
                    id: maMenuBar
                    anchors.fill: parent
                    onClicked: onMenu()
                }

                clip: true                                                              // Для ограничения кнопки
            }
/*
            SearchBar {
                id: searchBox
            }*/

            // Лейбл рядом с кнопкой
            /*Label {
                id: titleText
                anchors.left: menuButton.right
                anchors.verticalCenter: parent.verticalCenter
                text: appTitle
                font.pixelSize: 0.35 * parent.height
                color: palette.text
            }*/
        }
        
        // Определяем слайдер-меню
        Rectangle {
            id: menuView
            anchors.top: menuBar.bottom
            height: parent.height - menuBar.height
            width: menuWidth
            z: 3 // Для отображения содержимого на этом меню
            MainMenu {
                id: mainMenu
                anchors.fill: parent
                onMenuItemClicked: {
                    onMenu()
                    loader.source = page
                }
            }

            x: -menuWidth

            // Поведение для выдвижения слайдер-меню
            Behavior on x { NumberAnimation { duration: app.durationOfMenuAnimation; easing.type: Easing.OutQuad }}
            onXChanged: {
                menuProgressOpening = (1 - Math.abs(menuView.x / menuWidth))
            }

            // Пункты слайдер-меню
            /*MouseArea {
                anchors.right: parent.right
                anchors.rightMargin: app.menuIsShown ? (menuWidth - app.width) : -widthOfSeizure
                anchors.top: parent.top
                width: app.menuIsShown ? (app.width - menuWidth) : widthOfSeizure
                height: parent.height
                drag {
                    target: menuView
                    axis: Drag.XAxis
                    minimumX: -menuView.width
                    maximumX: 0
                }

                onClicked: {
                    if (app.menuIsShown) {
                        app.onMenu()
                    }
                }

                onReleased: {
                    if (Math.abs(menuView.x) > 0.5 * menuWidth) {
                        menuView.x = -menuWidth
                    } else {
                        menuView.x = 0
                    }
                }
            }*/
        }

        Rectangle {
            id: curtainMenu
            z: 1
            anchors.fill: parent
            color: "black"
            opacity: app.menuProgressOpening * 0.5
        }

        Loader {
            id: loader
            anchors.top: menuBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            onStatusChanged: {
                if (status == Loader.Loading) {
                    curtainLoading.visible = true
                    titleText.text = appTitle
                } else if (status == Loader.Ready) {
                    curtainLoading.visible = false
                } else if (status == Loader.Error) {
                    curtainLoading.visible = false
                }
            }

            Rectangle {
                id: curtainLoading
                anchors.fill: parent
                visible: false
                color: "white"
                opacity: 0.8
                BusyIndicator {
                    anchors.centerIn: parent
                }
            }
        }
    }

    function onMenu() {
        menuView.x = app.menuIsShown ? -menuWidth : 0
    }

    Component.onCompleted: {
        currentPage = "MainPage.qml"
        mainMenu.currentItem = 0
    }
}
