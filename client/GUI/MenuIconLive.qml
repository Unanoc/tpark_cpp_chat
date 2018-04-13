import QtQuick 2.5

// Состояния в анимации кнопки слайдер-бара
Item {
    id: root
    width: 24
    height: 24

    property real value: 0
    rotation: root.state == "menu" ? value * 180 : -value * 180 // Поворот

    // Положение вверх
    Rectangle {
        id: barUp
        x: 2 + 7.5 * value
        y: 5 + 3 * value
        width: 20 - 7 * value
        height: 2
        rotation: 45 * value
        antialiasing: true
    }

    // Положение в середине
    Rectangle {
        id: barMiddle
        x: 2 + value
        y: 10 + 2 * value
        width: 20 - 3 * value
        height: 2
        antialiasing: true
    }

    // Положение вниз
    Rectangle {
        id: barDown
        x: 2 + 7.5 * value
        y: 15 + value
        width: 20 - 7 * value
        height: 2
        rotation: -45 * value
        antialiasing: true
    }

    // Конечный автомат (матрица состояний)
    state: "menu"
    states: [
        State { name: "menu" },
        State { name: "back" }
    ]

    // Изменение состояний  
    onValueChanged: {
        if (value == 1) {
            root.state = "back"
        } else {
            root.state = "menu"
        }
    }
}
