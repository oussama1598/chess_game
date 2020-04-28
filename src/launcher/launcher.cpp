#include "launcher.h"

Launcher::Launcher(int &argc, char **argv) : QApplication(argc, argv) {

    // setting a global css for the app
    QFile File(css_path_);
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    setStyleSheet(StyleSheet);

    // show the main window
    open_main_window_();
}

void Launcher::open_main_window_() {
    if (main_window_ == nullptr) {
        main_window_ = new Main_Window();

        main_window_->set_on_quit_clicked([this]() {
            on_quit_clicked_();
        });

        main_window_->set_on_about_clicked([this]() {
            main_window_->close();

            open_about_window_();
        });

        main_window_->set_on_new_game_clicked([this]() {
            main_window_->close();

            open_new_game_window_();
        });
    }

    main_window_->show();
}

void Launcher::open_new_game_window_() {
    if (new_game_window_ == nullptr) {
        new_game_window_ = new New_Game_Window();
    }

    new_game_window_->show();
}

void Launcher::open_about_window_() {
    if (about_window_ == nullptr) {
        about_window_ = new About_Window();

        about_window_->set_close_callback([this]() {
            about_window_->hide();

            open_main_window_();
        });
    }

    about_window_->show();
}

void Launcher::on_quit_clicked_() {
    quit();
}
