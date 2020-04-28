#include "new_game_window.h"
#include "ui_new_game_window.h"

New_Game_Window::New_Game_Window(QWidget *parent) : QWidget(parent), ui(new Ui::New_Game_Window) {
    ui->setupUi(this);
}

New_Game_Window::~New_Game_Window() {
    delete ui;
}
