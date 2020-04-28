#pragma once

#include <iostream>
#include <cmath>
#include <functional>
#include "QWidget"
#include "QCloseEvent"
#include "QTimer"
#include "heart_graphic.h"

namespace Ui {
    class About_Window;
}

class About_Window : public QWidget {
Q_OBJECT

private:
    Ui::About_Window *ui;
    QTimer *timer_;
    Heart_Graphic *heart_graphic_;
    QGraphicsScene *graphics_scene_;

    std::function<void()> on_close_callback_;

private slots:

    void DrawHeart();

public:

    explicit About_Window(QWidget *parent = nullptr);

    ~About_Window();

    void set_close_callback(std::function<void()> callback);

    void closeEvent(QCloseEvent *event);
};
