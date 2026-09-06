#pragma once

#include <QtWidgets>
#include "pages/main/main_window.h"
#include "pages/about/about_window.h"
#include "pages/game/game_window.h"
#include "pages/new_game/new_game_window.h"

class Launcher_Shell : public QStackedWidget {
private:
    QPixmap backdrop_;

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit Launcher_Shell(QWidget *parent = nullptr);
};

class Launcher : public QApplication {
private:
    QString css_path_{
            "./assets/styles/main.css"};

    Launcher_Shell *launcher_window_{nullptr};
    Main_Window *main_window_{nullptr};
    New_Game_Window *new_game_window_{nullptr};
    About_Window *about_window_{nullptr};
    Game_Window *game_window_{nullptr};

    void open_main_window_();

    void open_new_game_window_();

    void open_game_window_(int opponent_type, int ai_difficulty, int renderer_type);

    void open_about_window_();

    void on_quit_clicked_();

    void show_launcher_page_(QWidget *page, const QString &title);

    void add_launcher_page_(QWidget *page);

public:

    Launcher(int &argc, char **argv);
};
