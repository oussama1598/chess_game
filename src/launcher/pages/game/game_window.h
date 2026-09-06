#pragma once

#include <functional>
#include "QWidget"
#include "QTimer"
#include "QMessageBox"
#include "QCloseEvent"
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
    bool closing_{false};

private:
    void init_2D_renderer_();

    void init_3D_renderer_();

    void render_2D_();

    void render_3D_();

    void close_game_();

    void finish_game_();

    void position_states_window_(int renderer_x, int renderer_y, int renderer_width);

private slots:

    void Render();

public:

    Game_Window(int ai_player_id, int ai_level, int renderer_type, QWidget *parent = nullptr);

    ~Game_Window();

    void set_close_callback(std::function<void()> callback);

    void closeEvent(QCloseEvent *event) override;
};
