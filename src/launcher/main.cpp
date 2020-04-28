#include <iostream>
#include <QtWidgets>
#include "pages/game/game_window.h"

#include "game/Game.h"
#include "renderers/2d/renderer_2D.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    // add stylesheet to the app
    QFile File(
            "/home/red-scule/Desktop/projects/cpp_projects/chess_game/src/launcher/styles/main.css");
    File.open(QFile::ReadOnly);

    QString StyleSheet = QLatin1String(File.readAll());

    app.setStyleSheet(StyleSheet);

    Game_Window game_window;

    game_window.show();

    return app.exec();
}
