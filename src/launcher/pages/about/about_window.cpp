#include "about_window.h"
#include "ui_about_window.h"

About_Window::About_Window(QWidget *parent) : QWidget(parent), ui(new Ui::About_Window) {
    ui->setupUi(this);

    setFixedSize(width(), height());

    DrawHeart();
}

About_Window::~About_Window() {
    delete timer_;
    delete graphics_scene_;
    delete heart_graphic_;
    delete ui;
}

void About_Window::set_close_callback(std::function<void()> callback) {
    on_close_callback_ = callback;
}

void About_Window::closeEvent(QCloseEvent *event) {
    event->ignore();

    on_close_callback_();
}

void About_Window::DrawHeart() {
    heart_graphic_ = new Heart_Graphic();

    graphics_scene_ = new QGraphicsScene(0, 0, 350, 192);
    graphics_scene_->addItem(heart_graphic_);

    ui->heart_canvas->setScene(graphics_scene_);

    timer_ = new QTimer();
    connect(timer_, SIGNAL(timeout()), graphics_scene_, SLOT(advance()));

    timer_->start(1000 / 30);
}
