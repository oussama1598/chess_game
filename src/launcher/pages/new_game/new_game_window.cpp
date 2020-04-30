#include "new_game_window.h"
#include "ui_new_game_window.h"

New_Game_Window::New_Game_Window(QWidget *parent) : QWidget(parent), ui(new Ui::New_Game_Window) {
    ui->setupUi(this);

    setFixedSize(width(), height());

    connect(ui->play_button, SIGNAL(clicked()), SLOT(onPlayButtonClicked()));
    connect(ui->opponent_type, SIGNAL(currentIndexChanged(int)), SLOT(opponentChanged()));

    ui->ai_difficulty_frame->hide();
}

New_Game_Window::~New_Game_Window() {
    delete ui;
}

void New_Game_Window::set_play_button_callback(std::function<void(int, int, int)> callback) {
    on_play_button_clicked_callback_ = callback;
}

void New_Game_Window::set_close_callback(std::function<void()> callback) {
    on_close_callback_ = callback;
}

void New_Game_Window::closeEvent(QCloseEvent *event) {
    event->ignore();

    on_close_callback_();
}

void New_Game_Window::onPlayButtonClicked() {
    on_play_button_clicked_callback_(
            ui->opponent_type->currentIndex(),
            ui->ai_difficulty->currentIndex() + 1,
            ui->renderer_type->currentIndex()
    );
}

void New_Game_Window::opponentChanged() {
    if (ui->opponent_type->currentIndex() == 1) {
        ui->ai_difficulty_frame->show();

        return;
    }

    ui->ai_difficulty_frame->hide();
}
