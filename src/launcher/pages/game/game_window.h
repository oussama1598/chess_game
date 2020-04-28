#pragma once

#include "QWidget"
#include "QTimer"
#include "game/Game.h"
#include "renderers/2d/renderer_2D.h"
#include "../states/states_window.h"

class Game_Window : public QWidget {
Q_OBJECT
private:
    QTimer *timer_;
    Game *game_;
    Renderer2D *renderer_;
    States_Window *states_window_;

private slots:

    void Render();

public:
    explicit Game_Window(QWidget *parent = nullptr);

    ~Game_Window();
};
