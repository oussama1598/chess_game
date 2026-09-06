#include "about_window.h"
#include "ui_about_window.h"

#include <QPushButton>
#include <QShortcut>

About_Window::About_Window(QWidget *parent) : QWidget(parent), ui(new Ui::About_Window) {
    ui->setupUi(this);

    setWindowTitle("About Chess");
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    setFixedSize(480, 590);
    ui->label->setText("ABOUT");
    ui->label_2->setText("Built with care");
    ui->label_3->setText("Created by");
    ui->verticalLayout_2->setContentsMargins(42, 30, 42, 36);

    auto *divider = new QFrame(this);
    divider->setObjectName("launcher_divider");
    divider->setFixedSize(96, 2);
    ui->verticalLayout_2->insertWidget(1, divider, 0, Qt::AlignHCenter);

    auto *back_button = new QPushButton("Back", this);
    back_button->setObjectName("back_button");
    back_button->setCursor(Qt::PointingHandCursor);
    ui->verticalLayout_2->addWidget(back_button);
    connect(back_button, &QPushButton::clicked, this, [this]() {
        if (on_close_callback_) on_close_callback_();
    });
    auto *back_shortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(back_shortcut, &QShortcut::activated, this, [this]() {
        if (on_close_callback_) on_close_callback_();
    });

    DrawHeart();
}

About_Window::~About_Window() {
    delete ui;
}

void About_Window::set_close_callback(std::function<void()> callback) {
    on_close_callback_ = callback;
}

void About_Window::closeEvent(QCloseEvent *event) {
    event->ignore();

    if (on_close_callback_) {
        on_close_callback_();
    }
}

void About_Window::DrawHeart() {
    heart_graphic_ = new Heart_Graphic();

    graphics_scene_ = new QGraphicsScene(0, 0, 350, 192, this);
    graphics_scene_->addItem(heart_graphic_);

    ui->heart_canvas->setScene(graphics_scene_);
    ui->heart_canvas->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->heart_canvas->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->heart_canvas->setAlignment(Qt::AlignCenter);
    ui->heart_canvas->setStyleSheet("background: transparent; border: none;");
    ui->heart_canvas->viewport()->setAttribute(Qt::WA_TranslucentBackground);
}
