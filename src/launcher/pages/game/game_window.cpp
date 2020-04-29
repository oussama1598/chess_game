#include "game_window.h"

Game_Window::Game_Window(QWidget *parent) : QWidget(parent) {
    timer_ = new QTimer();
    game_ = new Game();
    renderer_ = new Renderer2D("Chess Game");
    states_window_ = new States_Window(game_, this);

    connect(timer_, SIGNAL(timeout()), this, SLOT(Render()));
    timer_->start(1000 / 60); // 60 fps

    renderer_->set_game(game_);
    renderer_->on_window_move([this](int x, int y) {
        states_window_->move(x + 800, y - 36);
    });
    renderer_->on_move(
            [this](const std::string &from, const std::string &to) {
                states_window_->add_move(from, to);
            });


    states_window_->show();
}

Game_Window::~Game_Window() {
    delete timer_;
    delete game_;
    delete dialog_box_;
    delete renderer_;
}

void Game_Window::Render() {
    setVisible(false);

    if (renderer_ != nullptr) {
        if (game_->is_game_ended() && dialog_box_ == nullptr) {
            dialog_box_ = new QMessageBox();
            dialog_box_->setWindowTitle("Game Over");

            std::string message =
                    "Player " + std::to_string(game_->get_current_player()->player_id) +
                    " has won  ";

            dialog_box_->setText(QString(message.c_str()));
            dialog_box_->exec();

            states_window_->hide();
            on_close_callback_();

            return;
        }

        if (!renderer_->is_running()) {
            states_window_->hide();

            on_close_callback_();
        }

        if (renderer_->is_running() && !game_->is_game_ended()) {
            renderer_->render();

            // TODO: check if we're playing against an AI, if so do this
            if (game_->get_current_player()->player_id == AI_PLAYER_ID_) {
                AI_Player::make_a_move(game_, 1);
            }
        }
    }
}

void Game_Window::set_close_callback(std::function<void()> callback) {
    on_close_callback_ = callback;
}
