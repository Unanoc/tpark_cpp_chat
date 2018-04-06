import QtQuick 2.4
import QtQuick.Controls 1.2

Item {
    id: menu

    signal menuItemClicked(string item, string page)
    property alias currentItem: listViewMenu.currentIndex

    ListModel {
        id: modelMenu
        ListElement {
            item: "page_example"
            icon: "qrc:/Resources/images/icon_game.png"
        }
        ListElement {
            item: "settings"
            icon: "qrc:/Resources/images/icon_settings.png"
        }
        ListElement {
            item: "about"
            icon: "qrc:/Resources/images/icon_info.png"
        }
    }

    function textItemMenu(item) {
        var textReturn = ""
        switch (item) {
            case "page_example":
                textReturn = qsTr("Example")
                break;
            case "settings":
                textReturn = qsTr("Settings")
                break;
            case "about":
                textReturn = qsTr("About")
                break;
        }

        return textReturn
    }

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

    Image {
        id: imgShadow
        anchors.top: logoWrapper.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 10
        z: 4
        source: "qrc:/Resources/images/shadow_title.png"
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
            color: wrapperItem.ListView.isCurrentItem || ma.pressed ? pallete.currentHighlightItem: "transparent"
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
                    menu.menuItemClicked(item, page)
                    listViewMenu.currentIndex = index
                }
            }
        }
    }
}
