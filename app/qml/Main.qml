import QtQuick
import QtQuick.Controls

Window {
    width: 400
    height: 300
    visible: true
    title: "Game Menu"

    property bool gameStarted: false

    Column {
        anchors.centerIn: parent
        spacing: 20

        ComboBox {
            id: gameSelector
            model: ["TicTacToe", "Checkers"]
            width: parent.width * 0.6
        }

        ComboBox {
            id: modeSelector
            model: ["Player vs AI", "AI vs AI"]
            width: parent.width * 0.6
        }

        Button {
            text: "Start Game"
            onClicked: {
                gameController.startGame(gameSelector.currentText, modeSelector.currentText)
                gameStarted = true
            }
        }

        // Optional: Score display
        Text {
            visible: gameStarted
            text: "Game: " + gameController.currentGame + " | Mode: " + gameController.mode
        }
    }
}
