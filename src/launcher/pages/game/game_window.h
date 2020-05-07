#pragma once

#include <functional>
#include <thread>
#include <boost/any.hpp>
#include <boost/variant.hpp>
#include "QWidget"
#include "QTimer"
#include "QMessageBox"
#include "game/Game.h"
#include "renderers/2d/renderer_2D.h"
#include "renderers/3d/renderer_3D.h"
#include "ai/ai_player.h"

#include "../states/states_window.h"

class Game_Window : public QWidget {
Q_OBJECT
private:
    QTimer *timer_{nullptr};
    Game *game_{nullptr};
    Renderer_2D *renderer_2D_{nullptr};
    Renderer_3D *renderer_3D_{nullptr};
    States_Window *states_window_{nullptr};
    QMessageBox *dialog_box_{nullptr};

    int renderer_type_;

    std::function<void()> on_close_callback_;

    int ai_player_id_;
    int ai_level_;

private:
    void init_2D_renderer_();

    void init_3D_renderer_();

    void render_2D_();

    void render_3D_();

private slots:

    void Render();

public:

    Game_Window(int ai_player_id, int ai_level, int renderer_type, QWidget *parent = nullptr);

    ~Game_Window();

    void set_close_callback(std::function<void()> callback);
};
