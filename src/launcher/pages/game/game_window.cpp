#include "game_window.h"

Game_Window::Game_Window(int ai_player_id, int ai_level, int renderer_type, QWidget *parent)
        : QWidget(parent),
          ai_player_id_{ai_player_id},
          ai_level_{ai_level} {
    timer_ = new QTimer();
    game_ = new Game();
    states_window_ = new States_Window(ai_player_id, ai_level, game_, this);

    renderer_type_ = renderer_type;

    connect(timer_, SIGNAL(timeout()), this, SLOT(Render()));
    timer_->start(1000 / 60); // 60 fps

    if (renderer_type_ == 0) {
        init_2D_renderer_();
    } else {
        init_3D_renderer_();
    }

    states_window_->show();
}

Game_Window::~Game_Window() {
    delete timer_;
    delete renderer_2D_;
    delete renderer_3D_;
    delete dialog_box_;
    delete game_;
}

void Game_Window::init_2D_renderer_() {
    renderer_2D_ = new Renderer_2D(game_);

    renderer_2D_->on_window_move([this](int x, int y) {
        states_window_->move(x + 800, y - 36);
    });
    renderer_2D_->on_move(
            [this](const std::string &from, const std::string &to) {
                states_window_->add_move(from, to);
            });
}

void Game_Window::init_3D_renderer_() {
    renderer_3D_ = new Renderer_3D(game_, ai_player_id_ != -1);

    renderer_3D_->on_window_move([this](int x, int y) {
        states_window_->move(x + 1000, y - 36);
    });
    renderer_3D_->on_move(
            [this](const std::string &from, const std::string &to) {
                states_window_->add_move(from, to);
            });
}

void Game_Window::render_2D_() {
    if (renderer_2D_ != nullptr) {
        if (game_->is_game_ended() && dialog_box_ == nullptr) {
            dialog_box_ = new QMessageBox();
            dialog_box_->setWindowTitle("Game Over");

            std::string message =
                    "Player " + std::to_string(game_->get_current_player()->player_id + 1) +
                    " has won  ";

            dialog_box_->setText(QString(message.c_str()));
            dialog_box_->exec();

            exit(0);
        }

        if (!renderer_2D_->is_running()) {
            states_window_->hide();

            on_close_callback_();
        }


        if (renderer_2D_->is_running() && !game_->is_game_ended()) {
            renderer_2D_->render();

            if (game_->get_current_player()->player_id == ai_player_id_ &&
                !AI_Player::is_thinking()) {
                AI_Player::make_a_move(game_, ai_level_);

                renderer_2D_->render();
            }
        }
    }
}

void Game_Window::render_3D_() {
    if (renderer_3D_ != nullptr) {
        if (game_->is_game_ended() && dialog_box_ == nullptr) {
            dialog_box_ = new QMessageBox();
            dialog_box_->setWindowTitle("Game Over");

            std::string message =
                    "Player " + std::to_string(game_->get_current_player()->player_id + 1) +
                    " has won  ";

            dialog_box_->setText(QString(message.c_str()));
            dialog_box_->exec();

            states_window_->hide();
            on_close_callback_();

            return;
        }

        if (!renderer_3D_->is_running()) {
            exit(0);
        }


        if (renderer_3D_->is_running() && !game_->is_game_ended()) {
            renderer_3D_->render();

            if (game_->get_current_player()->player_id == ai_player_id_ &&
                !AI_Player::is_thinking() && !renderer_3D_->is_animating()) {
                AI_Player::make_a_move(game_, ai_level_);

                renderer_3D_->check_for_board_changes();
                renderer_3D_->render();
            }
        }
    }
}

void Game_Window::Render() {
    setVisible(false);


    if (renderer_type_ == 0) {
        render_2D_();
    } else {
        render_3D_();
    }
}

void Game_Window::set_close_callback(std::function<void()> callback) {
    on_close_callback_ = callback;
}
