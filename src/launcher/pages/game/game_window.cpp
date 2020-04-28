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
    delete renderer_;
}

void Game_Window::Render() {
    setVisible(false);

    if (renderer_->is_running()) {
        renderer_->render();
    }
}
