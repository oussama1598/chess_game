#pragma once

#include <iostream>
#include <string>
#include "QMainWindow"
#include "QTimer"
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

private slots:

    void Render();

public:
    States_Window(int ai_player_id, int ai_level, Game *game, QWidget *parent = nullptr);

    ~States_Window() override;

    void add_move(const std::string &from, const std::string &to);
};
