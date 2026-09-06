#include "launcher.h"

#include <QDir>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPropertyAnimation>
#include <QScreen>
#include <QTimer>

namespace {
void show_centered(QWidget *window) {
    window->show();
    QTimer::singleShot(0, window, [window]() {
        QScreen *screen = window->screen();
        if (screen == nullptr) {
            screen = QGuiApplication::primaryScreen();
        }
        if (screen == nullptr) {
            return;
        }

        const QRect available = screen->availableGeometry();
        const QRect frame = window->frameGeometry();
        window->move(available.center() - QPoint(frame.width() / 2, frame.height() / 2));
    });
}
}

Launcher_Shell::Launcher_Shell(QWidget *parent)
        : QStackedWidget(parent),
          backdrop_("./assets/ui/launcher_backdrop_v2.jpg") {
    setAttribute(Qt::WA_OpaquePaintEvent);
}

void Launcher_Shell::paintEvent(QPaintEvent *event) {
    QStackedWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    if (!backdrop_.isNull()) {
        const QPixmap cover = backdrop_.scaled(
                size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        const QRect source((cover.width() - width()) / 2,
                           (cover.height() - height()) / 2,
                           width(), height());
        painter.drawPixmap(rect(), cover, source);
    } else {
        painter.fillRect(rect(), QColor(5, 11, 21));
    }

    painter.fillRect(rect(), QColor(7, 16, 29, 115));

    QRadialGradient warm_glow(QPointF(width() * .88, height() * .82), width() * .62);
    warm_glow.setColorAt(0.f, QColor(145, 83, 35, 48));
    warm_glow.setColorAt(1.f, QColor(30, 18, 12, 0));
    painter.fillRect(rect(), warm_glow);

    QLinearGradient readability(0, 0, 0, height());
    readability.setColorAt(0.f, QColor(5, 11, 21, 51));
    readability.setColorAt(1.f, QColor(5, 11, 21, 209));
    painter.fillRect(rect(), readability);
}

Launcher::Launcher(int &argc, char **argv) : QApplication(argc, argv) {
    // Every renderer uses paths relative to the executable's asset directory.
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    // setting a global css for the app
    QFile File(css_path_);
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    setStyleSheet(StyleSheet);

    setQuitOnLastWindowClosed(true);

    launcher_window_ = new Launcher_Shell();
    launcher_window_->setObjectName("launcher_shell");
    launcher_window_->setFixedSize(500, 590);

    // show the main window
    open_main_window_();
}

void Launcher::add_launcher_page_(QWidget *page) {
    page->setWindowFlags(Qt::Widget);
    page->setMinimumSize(0, 0);
    page->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    page->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    launcher_window_->addWidget(page);
}

void Launcher::show_launcher_page_(QWidget *page, const QString &title) {
    const bool page_changed = launcher_window_->currentWidget() != page;
    QGraphicsOpacityEffect *opacity = nullptr;
    if (page_changed) {
        opacity = new QGraphicsOpacityEffect(page);
        opacity->setOpacity(0.f);
        page->setGraphicsEffect(opacity);
    }

    launcher_window_->setWindowTitle(title);
    launcher_window_->setCurrentWidget(page);
    show_centered(launcher_window_);

    if (opacity != nullptr) {
        auto *animation = new QPropertyAnimation(opacity, "opacity", page);
        animation->setDuration(140);
        animation->setStartValue(0.f);
        animation->setEndValue(1.f);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        connect(animation, &QPropertyAnimation::finished, page, [page, opacity]() {
            if (page->graphicsEffect() == opacity) page->setGraphicsEffect(nullptr);
        });
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void Launcher::open_main_window_() {
    if (main_window_ == nullptr) {
        main_window_ = new Main_Window();
        add_launcher_page_(main_window_);

        main_window_->set_on_quit_clicked([this]() {
            on_quit_clicked_();
        });

        main_window_->set_on_about_clicked([this]() {
            open_about_window_();
        });

        main_window_->set_on_new_game_clicked([this]() {
            open_new_game_window_();
        });
    }

    show_launcher_page_(main_window_, "Chess");
}

void Launcher::open_new_game_window_() {
    if (new_game_window_ == nullptr) {
        new_game_window_ = new New_Game_Window();
        add_launcher_page_(new_game_window_);

        new_game_window_->set_close_callback([this]() {
            open_main_window_();
        });

        new_game_window_->set_play_button_callback(
                [this](int opponent_type, int ai_difficulty, int renderer_type) {
                    new_game_window_->hide();

                    open_game_window_(opponent_type, ai_difficulty, renderer_type);
                });
    }

    show_launcher_page_(new_game_window_, "New Game - Chess");
}

void Launcher::open_game_window_(int opponent_type, int ai_difficulty, int renderer_type) {
    if (game_window_ == nullptr) {
        int ai = opponent_type == 0 ? -1 : 1;

        game_window_ = new Game_Window(ai, ai_difficulty, renderer_type);

        game_window_->set_close_callback([this]() {
            game_window_->hide();

            game_window_->deleteLater();
            game_window_ = nullptr;

            open_main_window_();
        });
    }

    launcher_window_->hide();
    game_window_->show();
}

void Launcher::open_about_window_() {
    if (about_window_ == nullptr) {
        about_window_ = new About_Window();
        add_launcher_page_(about_window_);

        about_window_->set_close_callback([this]() {
            open_main_window_();
        });
    }

    show_launcher_page_(about_window_, "About Chess");
}

void Launcher::on_quit_clicked_() {
    quit();
}
