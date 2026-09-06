#include "new_game_window.h"
#include "ui_new_game_window.h"

#include <QLabel>
#include <QShortcut>

New_Game_Window::New_Game_Window(QWidget *parent) : QWidget(parent), ui(new Ui::New_Game_Window) {
    ui->setupUi(this);

    setWindowTitle("New Game - Chess");
    setFixedSize(500, 570);
    ui->label->setText("NEW GAME");
    ui->verticalLayout->setContentsMargins(42, 30, 42, 36);
    ui->verticalLayout->setSpacing(16);
    ui->verticalLayout_2->setSpacing(12);

    auto *divider = new QFrame(this);
    divider->setObjectName("launcher_divider");
    divider->setFixedSize(96, 2);
    ui->verticalLayout->insertWidget(1, divider, 0, Qt::AlignHCenter);

    auto *subtitle = new QLabel("Choose how you want to play", this);
    subtitle->setObjectName("subtitle");
    subtitle->setAlignment(Qt::AlignCenter);
    ui->verticalLayout->insertWidget(2, subtitle);

    ui->opponent_type->setItemText(0, "Local player");
    ui->opponent_type->setItemText(1, "Computer");
    ui->ai_difficulty->setItemText(0, "Easy - Random");
    ui->ai_difficulty->setItemText(1, "Normal - Quick");
    ui->ai_difficulty->setItemText(2, "Hard - Strategic");
    ui->ai_difficulty->setItemText(3, "Expert - Deep search");
    ui->renderer_type->setItemText(0, "2D Classic");
    ui->renderer_type->setItemText(1, "3D Immersive");
    ui->play_button->setText("Start game");

    auto *back_button = new QPushButton("Back", this);
    back_button->setObjectName("back_button");
    back_button->setCursor(Qt::PointingHandCursor);
    auto *actions = new QHBoxLayout();
    ui->verticalLayout_2->removeWidget(ui->play_button);
    actions->addWidget(back_button);
    actions->addWidget(ui->play_button, 1);
    ui->verticalLayout_2->addLayout(actions);

    connect(ui->play_button, SIGNAL(clicked()), SLOT(onPlayButtonClicked()));
    connect(ui->opponent_type, SIGNAL(currentIndexChanged(int)), SLOT(opponentChanged()));
    connect(back_button, &QPushButton::clicked, this, [this]() {
        if (on_close_callback_) on_close_callback_();
    });
    auto *back_shortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(back_shortcut, &QShortcut::activated, this, [this]() {
        if (on_close_callback_) on_close_callback_();
    });

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

    if (on_close_callback_) {
        on_close_callback_();
    }
}

void New_Game_Window::onPlayButtonClicked() {
    if (on_play_button_clicked_callback_) {
        on_play_button_clicked_callback_(
                ui->opponent_type->currentIndex(),
                ui->ai_difficulty->currentIndex() + 1,
                ui->renderer_type->currentIndex()
        );
    }
}

void New_Game_Window::opponentChanged() {
    if (ui->opponent_type->currentIndex() == 1) {
        ui->ai_difficulty_frame->show();

        return;
    }

    ui->ai_difficulty_frame->hide();
}
