#include "states_window.h"
#include "ui_states_window.h"

#include <QAbstractItemView>
#include <QFrame>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace {
void install_piece_badge(QFrame *frame, const QString &glyph, const char *side) {
    frame->setStyleSheet(QString());
    frame->setProperty("side", side);
    frame->setAccessibleName(side == QString("light") ? "White player" : "Black player");

    auto *layout = new QVBoxLayout(frame);
    layout->setContentsMargins(0, 0, 0, 2);
    auto *label = new QLabel(glyph, frame);
    label->setObjectName("piece_glyph");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}
}

States_Window::States_Window(int ai_player_id, int ai_level, Game *game, QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::States_Window),
          game_{game},
          ai_player_id_{ai_player_id},
          ai_level_{ai_level} {
    ui->setupUi(this);

    setWindowTitle("Chess · Match Centre");
    setFixedSize(290, 800);

    ui->player_1_name->setText("White · Player 1");
    ui->player_2_name->setText("Black · Player 2");
    ui->frame_2->setFixedHeight(92);
    ui->frame_3->setFixedHeight(92);
    ui->horizontalLayout->setContentsMargins(16, 15, 16, 15);
    ui->horizontalLayout_2->setContentsMargins(16, 15, 16, 15);
    ui->horizontalLayout->setSpacing(12);
    ui->horizontalLayout_2->setSpacing(12);
    install_piece_badge(ui->player_1_icon, QString::fromUtf8("♔"), "light");
    install_piece_badge(ui->player_2_icon, QString::fromUtf8("♚"), "dark");

    auto *history_header = new QFrame(this);
    history_header->setObjectName("history_header");
    history_header->setFixedHeight(72);
    auto *history_layout = new QVBoxLayout(history_header);
    history_layout->setContentsMargins(16, 12, 16, 10);
    history_layout->setSpacing(3);
    auto *history_title = new QLabel("MATCH HISTORY", history_header);
    history_title->setObjectName("history_title");
    history_status_ = new QLabel("Awaiting opening move", history_header);
    history_status_->setObjectName("history_status");
    history_layout->addWidget(history_title);
    history_layout->addWidget(history_status_);
    ui->verticalLayout_5->insertWidget(1, history_header);

    ui->moves_list->setColumnCount(3);
    ui->moves_list->setHorizontalHeaderLabels({"#", "WHITE", "BLACK"});
    ui->moves_list->horizontalHeader()->setVisible(true);
    ui->moves_list->horizontalHeader()->setMinimumSectionSize(32);
    ui->moves_list->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->moves_list->horizontalHeader()->resizeSection(0, 42);
    ui->moves_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->moves_list->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->moves_list->verticalHeader()->setVisible(false);
    ui->moves_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->moves_list->setSelectionMode(QAbstractItemView::NoSelection);
    ui->moves_list->setFocusPolicy(Qt::NoFocus);
    ui->moves_list->setShowGrid(false);
    ui->moves_list->setWordWrap(false);
    ui->moves_list->verticalHeader()->setDefaultSectionSize(36);

    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(Render()));
    timer_->start(100);

    if (ai_player_id != -1) {
        ui->player_2_name->setText(QString("Black · Computer L%1").arg(ai_level));
    }
}

States_Window::~States_Window() {
    delete ui;
}

void States_Window::add_move(const std::string &from, const std::string &to) {
    QTableWidget *table = ui->moves_list;
    const bool white_move = ply_count_ % 2 == 0;
    const int row = ply_count_ / 2;

    if (white_move) {
        table->setRowCount(row + 1);
        auto *number = new QTableWidgetItem(QString("%1").arg(row + 1, 2, 10, QChar('0')));
        number->setTextAlignment(Qt::AlignCenter);
        table->setItem(row, 0, number);
        table->setItem(row, 2, new QTableWidgetItem(QString()));
    }

    const QString notation = QString("%1  →  %2")
            .arg(QString::fromStdString(from).toUpper(),
                 QString::fromStdString(to).toUpper());
    auto *move = new QTableWidgetItem(notation);
    move->setTextAlignment(Qt::AlignCenter);
    table->setItem(row, white_move ? 1 : 2, move);

    if (latest_move_item_ != nullptr) {
        QFont previous_font = latest_move_item_->font();
        previous_font.setBold(false);
        latest_move_item_->setFont(previous_font);
        latest_move_item_->setForeground(QColor("#d8e1ee"));
    }
    QFont latest_font = move->font();
    latest_font.setBold(true);
    move->setFont(latest_font);
    move->setForeground(QColor("#72dbc2"));
    latest_move_item_ = move;

    ++ply_count_;
    const int next_move = ply_count_ / 2 + 1;
    const QString next_player = ply_count_ % 2 == 0 ? "White" : "Black";
    history_status_->setText(QString("Move %1 · %2 to move").arg(next_move).arg(next_player));
    table->scrollToItem(move, QAbstractItemView::PositionAtCenter);
}

void States_Window::Render() {
    const int player_id = game_->get_current_player()->player_id;
    if (player_id == last_player_id_) {
        return;
    }

    const bool player_one_active = player_id == 0;
    ui->frame_3->setProperty("active", player_one_active);
    ui->frame_2->setProperty("active", !player_one_active);
    ui->frame_3->style()->unpolish(ui->frame_3);
    ui->frame_3->style()->polish(ui->frame_3);
    ui->frame_2->style()->unpolish(ui->frame_2);
    ui->frame_2->style()->polish(ui->frame_2);

    if (player_one_active) {
        ui->player_1_turn->show();
        ui->player_2_turn->hide();
        ui->player_1_turn->setText("Your turn");
    }

    if (!player_one_active) {
        ui->player_2_turn->show();
        ui->player_1_turn->hide();
        ui->player_2_turn->setText(ai_player_id_ == 1 ? "Computer's turn" : "Your turn");
    }

    last_player_id_ = player_id;
}
