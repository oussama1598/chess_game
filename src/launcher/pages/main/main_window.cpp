#include "main_window.h"
#include "ui_main_window.h"

#include <QLabel>

Main_Window::Main_Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Main_Window) {
    ui->setupUi(this);

    setWindowTitle("Chess");
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    setFixedSize(480, 540);

    ui->label->setText("CHESS");
    ui->verticalLayout_2->setContentsMargins(42, 30, 42, 36);
    ui->verticalLayout_2->setSpacing(18);
    ui->verticalLayout->setSpacing(12);

    auto *divider = new QFrame(this);
    divider->setObjectName("launcher_divider");
    divider->setFixedSize(96, 2);
    ui->verticalLayout_2->insertWidget(1, divider, 0, Qt::AlignHCenter);

    auto *subtitle = new QLabel("A focused game of strategy", this);
    subtitle->setObjectName("subtitle");
    subtitle->setAlignment(Qt::AlignCenter);
    ui->verticalLayout_2->insertWidget(2, subtitle);

    ui->new_game_button->setText("New game");
    ui->about_button->setText("About the game");
    ui->new_game_button->setToolTip("Choose an opponent and board style");

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
    if (on_new_game_clicked_callback_) {
        on_new_game_clicked_callback_();
    }
}

void Main_Window::onAboutClicked() {
    if (on_about_clicked_callback_) {
        on_about_clicked_callback_();
    }
}

void Main_Window::onQuitClicked() {
    if (on_quit_clicked_callback_) {
        on_quit_clicked_callback_();
    }
}

void Main_Window::closeEvent(QCloseEvent *event) {
    event->ignore();

    if (on_quit_clicked_callback_) {
        on_quit_clicked_callback_();
    }
}
