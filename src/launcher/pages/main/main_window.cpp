#include "main_window.h"
#include "ui_main_window.h"

Main_Window::Main_Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Main_Window) {
    ui->setupUi(this);

    connect(ui->new_game_button, SIGNAL(clicked()), this, SLOT(onNewGameClicked()));
    connect(ui->quit_button, SIGNAL(clicked()), this, SLOT(onQuitClicked()));
    connect(ui->about_button, SIGNAL(clicked()), this, SLOT(onAboutClicked()));
}

Main_Window::~Main_Window() {
    delete ui;
}

void Main_Window::set_on_new_game_clicked(std::function<void()> callback) {
    on_new_game_clicked_callback_ = callback;
}

void Main_Window::set_on_about_clicked(std::function<void()> callback) {
    on_about_clicked_callback_ = callback;
}

void Main_Window::set_on_quit_clicked(std::function<void()> callback) {
    on_quit_clicked_callback_ = callback;
}

void Main_Window::onNewGameClicked() {
    on_new_game_clicked_callback_();
}

void Main_Window::onAboutClicked() {
    on_about_clicked_callback_();
}

void Main_Window::onQuitClicked() {
    on_quit_clicked_callback_();
}

