#pragma once

#include <iostream>
#include <string>
#include "QMainWindow"
#include "QTimer"
#include "QLabel"
#include "QTableWidgetItem"
#include "game/Game.h"

namespace Ui {
    class States_Window;
}

class States_Window : public QMainWindow {
Q_OBJECT

private:
    Ui::States_Window *ui;
    Game *game_;
    QTimer *timer_;

    int ai_player_id_;
    int ai_level_;
    int last_player_id_{-1};
    int ply_count_{0};
    QLabel *history_status_{nullptr};
    QTableWidgetItem *latest_move_item_{nullptr};

private slots:

    void Render();

public:
    States_Window(int ai_player_id, int ai_level, Game *game, QWidget *parent = nullptr);

    ~States_Window() override;

    void add_move(const std::string &from, const std::string &to);
};
