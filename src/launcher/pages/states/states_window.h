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

public:
    explicit States_Window(Game *game, QWidget *parent = nullptr);

    ~States_Window() override;

    void add_move(const std::string &from, const std::string &to);
};
