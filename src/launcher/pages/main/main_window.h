#pragma once

#include <iostream>
#include <functional>
#include "QMainWindow"
#include "QCloseEvent"

namespace Ui {
    class Main_Window;
}

class Main_Window : public QMainWindow {
Q_OBJECT

private:
    Ui::Main_Window *ui;

    std::function<void()> on_new_game_clicked_callback_;
    std::function<void()> on_about_clicked_callback_;
    std::function<void()> on_quit_clicked_callback_;

private slots:

    void onNewGameClicked();

    void onAboutClicked();

    void onQuitClicked();

public:
    explicit Main_Window(QWidget *parent = nullptr);

    ~Main_Window() override;

    void set_on_new_game_clicked(std::function<void()> callback);

    void set_on_about_clicked(std::function<void()> callback);

    void set_on_quit_clicked(std::function<void()> callback);

    void closeEvent(QCloseEvent *event);
};
