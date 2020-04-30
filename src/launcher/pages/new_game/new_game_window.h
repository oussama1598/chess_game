#pragma once

#include <QWidget>
#include "QCloseEvent"

namespace Ui {
    class New_Game_Window;
}

class New_Game_Window : public QWidget {
Q_OBJECT

private:
    Ui::New_Game_Window *ui;

    std::function<void(int, int, int)> on_play_button_clicked_callback_;
    std::function<void()> on_close_callback_;

private slots:

    void onPlayButtonClicked();

    void opponentChanged();

public:

    explicit New_Game_Window(QWidget *parent = nullptr);

    ~New_Game_Window() override;

    void set_play_button_callback(std::function<void(int, int, int)> callback);

    void set_close_callback(std::function<void()> callback);

    void closeEvent(QCloseEvent *event);
};
