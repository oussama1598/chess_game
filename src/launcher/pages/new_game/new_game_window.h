#pragma once

#include <QWidget>

namespace Ui {
    class New_Game_Window;
}

class New_Game_Window : public QWidget {
Q_OBJECT

private:
    Ui::New_Game_Window *ui;

public:
    explicit New_Game_Window(QWidget *parent = nullptr);

    ~New_Game_Window() override;
};
