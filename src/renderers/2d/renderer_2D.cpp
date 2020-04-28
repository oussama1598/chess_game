#include "renderer_2D.h"

Renderer2D::Renderer2D(const char *title) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        throw std::runtime_error(SDL_GetError());

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        throw std::runtime_error(SDL_GetError());

    if (TTF_Init() != 0)
        throw std::runtime_error(SDL_GetError());

    window_ = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width_,
            height_,
            SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (window_ == nullptr)
        throw std::runtime_error("Could not create a window_");

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_PRESENTVSYNC);

    if (renderer_ == nullptr)
        throw std::runtime_error("Could not create a renderer");

    is_running_ = true;

    // set the cursors
    arrow_cursor_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    hand_cursor_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    load_fonts_();
    load_texture_();
    load_sounds_();

    // this function is responsible for doing the size's calculations for the pieces rectangles
    init_table_();
}

Renderer2D::~Renderer2D() {
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Renderer2D::on_window_move(window_move_callback callback) {
    on_window_move_callback_ = callback;
}

void Renderer2D::on_move(move_callback callback) {
    on_move_callback_ = callback;
}

void Renderer2D::load_fonts_() {
    text_renderer.add_font(
            "open-sans-24",
            24,
            "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/fonts/OpenSans-Regular.ttf"
    );
    text_renderer.add_font(
            "open-sans-20",
            20,
            "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/fonts/OpenSans-Regular.ttf"
    );
}

void Renderer2D::load_texture_() {
    // TODO: remove the absolute path
    SDL_Surface *pieces_sprite = IMG_Load(
            "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/sprites/chess_pieces.png");
    SDL_Surface *guide_circle_sprite = IMG_Load(
            "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/sprites/circle.png");

    if (pieces_sprite == nullptr || guide_circle_sprite == nullptr)
        throw std::runtime_error(SDL_GetError());

    texture_ = SDL_CreateTextureFromSurface(renderer_, pieces_sprite);
    guide_circle_texture_ = SDL_CreateTextureFromSurface(renderer_, guide_circle_sprite);

    if (texture_ == nullptr || guide_circle_texture_ == nullptr)
        throw std::runtime_error(SDL_GetError());

    SDL_FreeSurface(pieces_sprite);
    SDL_FreeSurface(guide_circle_sprite);
}

void Renderer2D::load_sounds_() {
    for (auto &sound: sounds_)
        sound_manager_.add_sound(sound.first, sound.second);
}

void Renderer2D::init_table_() {
    std::vector<SDL_Rect> dark_table_rectangles{};
    std::vector<SDL_Rect> light_table_rectangles{};

    SDL_Color dark_color{222, 172, 93, 255};
    SDL_Color light_color{249, 227, 192, 255};

    piece_width_ = (int) (width_ / cols_);
    piece_height_ = (int) (height_ / rows_);

    table_texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_TARGET,
                                       width_, height_);

    // to render to the texture instead of screen
    SDL_SetRenderTarget(renderer_, table_texture_);

    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j) {
            if (i % 2 == 0) {
                if (j % 2 == 0)
                    light_table_rectangles.push_back(
                            {piece_width_ * j, piece_height_ * i, piece_width_, piece_height_});
                else {
                    dark_table_rectangles.push_back(
                            {piece_width_ * j, piece_height_ * i, piece_width_, piece_height_});
                }
            } else {
                if (j % 2 != 0)
                    light_table_rectangles.push_back(
                            {piece_width_ * j, piece_height_ * i, piece_width_, piece_height_});
                else {
                    dark_table_rectangles.push_back(
                            {piece_width_ * j, piece_height_ * i, piece_width_, piece_height_});
                }
            }
        }

    SDL_SetRenderDrawColor(renderer_, dark_color.r, dark_color.g, dark_color.b, dark_color.a);
    SDL_RenderFillRects(renderer_, dark_table_rectangles.data(), dark_table_rectangles.size());

    SDL_SetRenderDrawColor(renderer_, light_color.r, light_color.g, light_color.b, light_color.a);
    SDL_RenderFillRects(renderer_, light_table_rectangles.data(), light_table_rectangles.size());

    std::array<std::string, 8> letters{"a", "b", "c", "d", "e", "f", "g", "h"};

    for (int i = rows_; i > 0; --i) {
        SDL_Color number_color = i % 2 ? light_color : dark_color;
        SDL_Rect number_rect{15, piece_height_ * (rows_ - i), 0, 0};

        SDL_Color letter_color = i % 2 ? dark_color : light_color;
        SDL_Rect letter_rect{(piece_width_ - 5) + piece_width_ * (rows_ - i),
                             piece_height_ * 8 - 30, 0, 0};

        text_renderer.render_text(std::to_string(i), number_rect, renderer_, "open-sans-20",
                                  number_color);
        text_renderer.render_text(letters[rows_ - i], letter_rect, renderer_, "open-sans-20",
                                  letter_color);
    }

    SDL_SetRenderTarget(renderer_, nullptr);
}

void Renderer2D::handle_move_(Piece::piece_coordinates from_coordinates,
                              Piece::piece_coordinates to_coordinates) {
    std::string source = Piece::get_id_from_coordinates(from_coordinates);
    std::string destination = Piece::get_id_from_coordinates(to_coordinates);

    Player *current_player = game_->get_current_player();

    if (from_coordinates.line == to_coordinates.line &&
        from_coordinates.column == to_coordinates.column)
        return;

    try {
        game_->make_move(source, destination);

        sound_manager_.play_sound("move");

        on_move_callback_(source, destination);
    } catch (std::exception &error) {
        std::cout << error.what() << std::endl;

        if (error.what() == Errors::ILLEGAL_MOVE) {
            sound_manager_.play_sound("illegal");

            show_flash_message_(from_coordinates);
        }

        if (error.what() == Errors::KING_IS_NOT_SAFE) {
            sound_manager_.play_sound("illegal");

            Piece::piece_coordinates king_coordinates = game_->get_board().find_king(
                    *current_player);

            show_flash_message_(king_coordinates);
        }
    }

    if (game_->is_game_ended())
        std::cout << "Game Over" << std::endl;
    else if (game_->is_game_in_check())
        std::cout << "Game is in check" << std::endl;
}

void Renderer2D::handle_mouse_press_down_(SDL_MouseButtonEvent &event) {
    Board::piecesType pieces = game_->get_board_pieces();
    Piece *piece = pieces[mouse_i_][mouse_j_];

    if (event.button == SDL_BUTTON_LEFT &&
        piece->get_player_id() != -1 &&
        piece->get_player_id() == game_->get_current_player()->player_id &&
        !game_->is_game_ended()) {
        selected_ = true;

        selected_i_ = mouse_i_;
        selected_j_ = mouse_j_;

        guides_ = game_->get_board().get_possible_moves_for(
                *game_->get_current_player(),
                Piece::get_id_from_coordinates({mouse_i_, mouse_j_}));
    }
}

void Renderer2D::handle_mouse_press_up_(SDL_MouseButtonEvent event) {
    if (event.button == SDL_BUTTON_LEFT && selected_) {
        selected_ = false;

        handle_move_({selected_i_, selected_j_}, {mouse_i_, mouse_j_});
    }
}

void Renderer2D::handle_events_() {
    SDL_Event event;

    SDL_PollEvent(&event);

    int current_mouse_i_;
    int current_mouse_j_;

    switch (event.type) {
        case SDL_QUIT:
            is_running_ = false;
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&mouse_x_, &mouse_y_);

            current_mouse_i_ = (height_ - mouse_y_) / piece_height_;
            current_mouse_j_ = mouse_x_ / piece_width_;

            if (current_mouse_i_ >= rows_ || current_mouse_j_ >= cols_)
                break;

            mouse_i_ = current_mouse_i_;
            mouse_j_ = current_mouse_j_;

            break;
        case SDL_MOUSEBUTTONDOWN:
            handle_mouse_press_down_(event.button);
            break;
        case SDL_MOUSEBUTTONUP:
            handle_mouse_press_up_(event.button);
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_MOVED) {
                on_window_move_callback_(event.window.data1, event.window.data2);
            }
            break;
    }
}

void
Renderer2D::show_flash_message_(Piece::piece_coordinates position,
                                SDL_Color color, unsigned int duration) {
    flash_message_.start_time = SDL_GetTicks();
    flash_message_.position = position;
    flash_message_.color = color;
    flash_message_.duration = duration;
    flash_message_.show = true;
}

void Renderer2D::render_fps_() {
    SDL_Rect fps_rect{width_ - 10, 0, 0, 0};

    text_renderer.render_text(
            std::to_string(FPS_),
            fps_rect,
            renderer_,
            "open-sans-24",
            {255, 0, 0, 255}
    );
}

void Renderer2D::render_table_() {
    SDL_Rect src = {0, 0, width_, height_};

    SDL_RenderCopy(renderer_, table_texture_, &src, &src);
}

void Renderer2D::render_cursor_() {
    Board::piecesType pieces = game_->get_board_pieces();
    Piece *piece = pieces[mouse_i_][mouse_j_];

    if (piece->get_player_id() == -1 ||
        piece->get_player_id() != game_->get_current_player()->player_id) {
        SDL_SetCursor(arrow_cursor_);

        return;
    }

    SDL_SetCursor(hand_cursor_);

    SDL_Rect hover_rectangle = {
            piece_width_ * mouse_j_,
            piece_height_ * (rows_ - 1 - mouse_i_),
            piece_width_, piece_height_
    };

    // TODO: remove the hardcoded color
    SDL_SetRenderDrawColor(renderer_, 135, 100, 43, 50);
    SDL_RenderFillRect(renderer_, &hover_rectangle);
}

void Renderer2D::render_flash_message_() {
    if (!flash_message_.show)
        return;

    unsigned int current_time = SDL_GetTicks();
    unsigned int duration = current_time - flash_message_.start_time;
    unsigned int flash_duration = flash_message_.duration / flash_message_.flashes_per_second;

    if (duration >= flash_message_.duration) {
        flash_message_.show = false;

        return;
    }

    if ((int) (duration / flash_duration) % 2 != 0)
        return;

    SDL_Rect flash_message_rect{
            piece_width_ * flash_message_.position.column,
            piece_height_ * (rows_ - 1 - flash_message_.position.line),
            piece_width_,
            piece_height_
    };

    SDL_SetRenderDrawColor(
            renderer_,
            flash_message_
                    .color.r,
            flash_message_.color.g,
            flash_message_.color.b,
            flash_message_.color.a
    );
    SDL_RenderFillRect(renderer_, &flash_message_rect);
}

void Renderer2D::render_guides_() {
    if (!selected_)
        return;

    for (auto &spot: guides_) {
        Piece::piece_coordinates coordinates = Piece::get_piece_coordinates_from_id(spot);

        SDL_Rect rect
                {
                        coordinates.column * piece_width_,
                        (rows_ - 1 - coordinates.line) * piece_height_,
                        piece_width_,
                        piece_height_
                };

        SDL_RenderCopy(renderer_, guide_circle_texture_, nullptr, &rect);
    }

}

void Renderer2D::render_game_() {
    std::vector<Player> players = game_->get_players();
    Board::piecesType pieces = game_->get_board_pieces();

    for (int i = 0; i < (int) pieces.size(); ++i) {
        for (int j = 0; j < (int) pieces.at(i).size(); ++j) {
            Piece *piece = pieces[i][j];

            if (piece->get_player_id() == -1) {
                continue;
            }

            int x = piece_width_ * j;
            int y = piece_height_ * (rows_ - 1 - i);

            if (selected_ && i == selected_i_ && j == selected_j_) {
                x = mouse_x_ - (piece_width_ / 2);
                y = mouse_y_ - (piece_height_ / 2);
            }

            std::string graphics_type = players.at(piece->get_player_id()).is_dark ? "dark"
                                                                                   : "light";
            SDL_Rect *src_rectangle = &pieces_texture_rectangles_.at(piece->get_symbol()).at(
                    graphics_type);

            SDL_Rect dist{x, y, piece_width_, piece_height_};
            SDL_RenderCopy(renderer_, texture_, src_rectangle, &dist);
        }
    }
}

void Renderer2D::render() {
    // calculate the frame rate
    unsigned int current_time = SDL_GetTicks();

    if (current_time - last_time_ >= 1000) {
        FPS_ = current_frames_count_;
        current_frames_count_ = 0;
        last_time_ = current_time;
    }

    current_frames_count_ += 1;

    handle_events_();

    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    SDL_RenderClear(renderer_);

    render_table_();
    render_cursor_();
    render_flash_message_();
    render_guides_();
    render_game_();

    // Render fps if we're in debug mode
#ifdef DEBUG
    render_fps_();
#endif

    SDL_RenderPresent(renderer_);
}
