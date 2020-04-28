#include "states_window.h"
#include "ui_states_window.h"

States_Window::States_Window(Game *game, QWidget *parent) : QMainWindow(parent),
                                                            ui(new Ui::States_Window), game_{game} {
    ui->setupUi(this);

    setFixedSize(width(), height());

    timer_ = new QTimer();
    connect(timer_, SIGNAL(timeout()), this, SLOT(Render()));
    timer_->start(1000 / 60);
}

States_Window::~States_Window() {
    delete ui;
}

void States_Window::add_move(const std::string &from, const std::string &to) {
    QTableWidget *table = ui->moves_list;

    const int currentRow = table->rowCount();
    table->setRowCount(currentRow + 1);

    table->setItem(currentRow, 0, new QTableWidgetItem(from.c_str()));
    table->setItem(currentRow, 1, new QTableWidgetItem(to.c_str()));
}

void States_Window::Render() {
    if (game_->get_current_player()->player_id == 0) {
        ui->player_1_turn->show();
        ui->player_2_turn->hide();
    }

    if (game_->get_current_player()->player_id == 1) {
        ui->player_2_turn->show();
        ui->player_1_turn->hide();
    }
}
