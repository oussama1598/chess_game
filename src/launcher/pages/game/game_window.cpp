#include "game_window.h"

#include <algorithm>

#include <QGuiApplication>
#include <QPoint>
#include <QScreen>

namespace {
int active_piece_count(Game *game) {
    int count = 0;
    for (const auto &row: game->get_board_pieces()) {
        count += static_cast<int>(std::count_if(row.begin(), row.end(), [](const Piece *piece) {
            return piece->get_player_id() != -1;
        }));
    }
    return count;
}
}

Game_Window::Game_Window(int ai_player_id, int ai_level, int renderer_type, QWidget *parent)
        : QWidget(parent),
          ai_player_id_{ai_player_id},
          ai_level_{ai_level} {
    timer_ = new QTimer(this);
    game_ = new Game();
    states_window_ = new States_Window(ai_player_id, ai_level, game_, this);

    renderer_type_ = renderer_type;

    connect(timer_, SIGNAL(timeout()), this, SLOT(Render()));
    timer_->setTimerType(Qt::PreciseTimer);

    if (renderer_type_ == 0) {
        init_2D_renderer_();
    } else {
        init_3D_renderer_();
    }

    // Both renderers use presentation-vsync. Queue the next frame as soon as the
    // previous swap completes instead of combining vsync with a second 16 ms gate.
    timer_->start(0);
    states_window_->show();
}

Game_Window::~Game_Window() {
    delete timer_;
    delete states_window_;
    delete renderer_2D_;
    delete renderer_3D_;
    delete dialog_box_;
    delete game_;
}

void Game_Window::init_2D_renderer_() {
    renderer_2D_ = new Renderer_2D(game_);

    renderer_2D_->on_window_move([this](int x, int y) {
        position_states_window_(x, y, 800);
    });
    renderer_2D_->on_move(
            [this](const std::string &from, const std::string &to) {
                states_window_->add_move(from, to);
            });
}

void Game_Window::init_3D_renderer_() {
    renderer_3D_ = new Renderer_3D(game_, ai_player_id_ != -1);

    renderer_3D_->on_window_move([this](int x, int y) {
        position_states_window_(x, y, 1000);
    });
    renderer_3D_->on_move(
            [this](const std::string &from, const std::string &to) {
                states_window_->add_move(from, to);
            });
}

void Game_Window::render_2D_() {
    if (renderer_2D_ != nullptr) {
        if (game_->is_game_ended() && dialog_box_ == nullptr) {
            finish_game_();
            return;
        }

        if (!renderer_2D_->is_running()) {
            close_game_();
            return;
        }


        if (renderer_2D_->is_running() && !game_->is_game_ended()) {
            renderer_2D_->render();

            if (game_->get_current_player()->player_id == ai_player_id_ &&
                !AI_Player::is_thinking()) {
                const auto previous_move = game_->get_latest_move();
                const int pieces_before = active_piece_count(game_);
                AI_Player::make_a_move(game_, ai_level_);
                const auto ai_move = game_->get_latest_move();

                if (ai_move != previous_move && !ai_move.first.empty()) {
                    states_window_->add_move(ai_move.first, ai_move.second);
                    renderer_2D_->play_move_sound(active_piece_count(game_) < pieces_before);
                }

                renderer_2D_->render();
            }
        }
    }
}

void Game_Window::render_3D_() {
    if (renderer_3D_ != nullptr) {
        if (game_->is_game_ended() && dialog_box_ == nullptr) {
            finish_game_();
            return;
        }

        if (!renderer_3D_->is_running()) {
            close_game_();
            return;
        }


        if (renderer_3D_->is_running() && !game_->is_game_ended()) {
            renderer_3D_->render();

            if (game_->get_current_player()->player_id == ai_player_id_ &&
                !AI_Player::is_thinking() && !renderer_3D_->is_animating()) {
                const auto previous_move = game_->get_latest_move();
                const int pieces_before = active_piece_count(game_);
                AI_Player::make_a_move(game_, ai_level_);
                const auto ai_move = game_->get_latest_move();

                if (ai_move != previous_move && !ai_move.first.empty()) {
                    states_window_->add_move(ai_move.first, ai_move.second);
                    renderer_3D_->prepare_move_feedback(
                            ai_move.second, active_piece_count(game_) < pieces_before);
                }

                renderer_3D_->check_for_board_changes();
                renderer_3D_->render();
            }
        }
    }
}

void Game_Window::position_states_window_(int renderer_x, int renderer_y, int renderer_width) {
    QScreen *screen = QGuiApplication::screenAt(QPoint(renderer_x, renderer_y));
    if (screen == nullptr) {
        screen = QGuiApplication::primaryScreen();
    }
    if (screen == nullptr) {
        return;
    }

    const QRect available = screen->availableGeometry();
    constexpr int gap = 12;
    int target_x = renderer_x + renderer_width + gap;
    if (target_x + states_window_->width() > available.right() + 1) {
        target_x = renderer_x - states_window_->width() - gap;
    }

    target_x = std::max(available.left(),
                        std::min(target_x, available.right() - states_window_->width() + 1));
    const int target_y = std::max(available.top(),
                                  std::min(renderer_y,
                                           available.bottom() - states_window_->height() + 1));

    if (states_window_->pos() != QPoint(target_x, target_y)) {
        states_window_->move(target_x, target_y);
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

void Game_Window::finish_game_() {
    dialog_box_ = new QMessageBox(this);
    dialog_box_->setWindowTitle("Game Over");

    if (game_->get_result() == Game::Result::Stalemate) {
        dialog_box_->setText("Draw by stalemate");
    } else {
        const std::string message =
                "Player " + std::to_string(game_->get_winner_id() + 1) +
                " has won by checkmate";
        dialog_box_->setText(QString::fromStdString(message));
    }

    dialog_box_->exec();
    close_game_();
}

void Game_Window::close_game_() {
    if (closing_) {
        return;
    }

    closing_ = true;
    timer_->stop();
    states_window_->hide();

    if (on_close_callback_) {
        on_close_callback_();
    }
}

void Game_Window::closeEvent(QCloseEvent *event) {
    event->ignore();
    close_game_();
}
