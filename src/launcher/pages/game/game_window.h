#pragma once

#include <functional>
#include "QWidget"
#include "QTimer"
#include "QMessageBox"
#include "game/Game.h"
#include "renderers/2d/renderer_2D.h"
#include "../states/states_window.h"

class Game_Window : public QWidget {
Q_OBJECT
private:
    QTimer *timer_{nullptr};
    Game *game_{nullptr};
    Renderer2D *renderer_{nullptr};
    States_Window *states_window_{nullptr};
    QMessageBox *dialog_box_{nullptr};

    std::function<void()> on_close_callback_;

private slots:

    void Render();

public:
    explicit Game_Window(QWidget *parent = nullptr);

    ~Game_Window();

    void set_close_callback(std::function<void()> callback);
};
