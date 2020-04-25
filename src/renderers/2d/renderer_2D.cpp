#include "renderer_2D.h"

Renderer2D::Renderer2D(const char *title) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error(SDL_GetError());

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
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

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);

    if (renderer_ == nullptr)
        throw std::runtime_error("Could not create a renderer");

    is_running_ = true;

    // set the cursors
    arrow_cursor_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    hand_cursor_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    load_texture_();

    // this function is responsible for doing the size's calculations for the pieces rectangles
    init_table_();
}

Renderer2D::~Renderer2D() {
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    IMG_Quit();
    SDL_Quit();
}

void Renderer2D::load_texture_() {
    // TODO: remove the absolute path
    SDL_Surface *sprite = IMG_Load(
            "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/sprites/chess_pieces_sprite.png");

    if (sprite == nullptr)
        throw std::runtime_error(SDL_GetError());

    texture_ = SDL_CreateTextureFromSurface(renderer_, sprite);

    if (texture_ == nullptr)
        throw std::runtime_error(SDL_GetError());

    SDL_FreeSurface(sprite);
}

void Renderer2D::init_table_() {
    std::vector<SDL_Rect> dark_table_rectangles{};
    std::vector<SDL_Rect> light_table_rectangles{};

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

    SDL_SetRenderDrawColor(renderer_, 222, 172, 93, 255);
    SDL_RenderFillRects(renderer_, dark_table_rectangles.data(), dark_table_rectangles.size());

    SDL_SetRenderDrawColor(renderer_, 249, 227, 192, 255);
    SDL_RenderFillRects(renderer_, light_table_rectangles.data(), light_table_rectangles.size());

    SDL_SetRenderTarget(renderer_, nullptr);
}

void Renderer2D::handle_move_(Piece::piece_coordinates from_coordinates,
                              Piece::piece_coordinates to_coordinates) {
    std::string source = Piece::get_id_from_coordinates(from_coordinates);
    std::string destination = Piece::get_id_from_coordinates(to_coordinates);

    try {
        game_->make_move(source, destination);
    } catch (std::exception &error) {
        std::cout << error.what() << std::endl;
    }
}

void Renderer2D::handle_mouse_press_down_(SDL_MouseButtonEvent &event) {
    Board::piecesType pieces = game_->get_board_pieces();
    Piece *piece = pieces[mouse_i_][mouse_j_];

    if (event.button == SDL_BUTTON_LEFT &&
        piece->get_player_id() != -1 &&
        piece->get_player_id() == game_->get_current_player()->player_id) {
        selected_ = true;

        selected_i_ = mouse_i_;
        selected_j_ = mouse_j_;
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

    switch (event.type) {
        case SDL_QUIT:
            is_running_ = false;
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&mouse_x_, &mouse_y_);

            mouse_i_ = mouse_y_ / piece_height_;
            mouse_j_ = mouse_x_ / piece_width_;
            break;
        case SDL_MOUSEBUTTONDOWN:
            handle_mouse_press_down_(event.button);
            break;
        case SDL_MOUSEBUTTONUP:
            handle_mouse_press_up_(event.button);
            break;
    }
}

void Renderer2D::render_table_() {
    SDL_Rect src = {0, 0, width_, height_};

    SDL_RenderCopy(renderer_, table_texture_, &src, &src);
}

void Renderer2D::render_game_() {
    if (game_->is_game_in_check())
        std::cout << "Game is in check" << std::endl;

    std::vector<Player> players = game_->get_players();
    Board::piecesType pieces = game_->get_board_pieces();

    for (int i = pieces.size() - 1; i >= 0; --i) {
        for (int j = 0; j < (int) pieces.at(i).size(); j++) {
            Piece *piece = pieces[i][j];

            if (piece->get_player_id() == -1) {
                continue;
            }

            int x = piece_width_ * j;
            int y = piece_height_ * i;

            if (selected_ && i == selected_i_ && j == selected_j_) {
                x = mouse_x_ - (piece_width_ / 2);
                y = mouse_y_ - (piece_height_ / 2);
            }

            std::string graphics_type = players.at(piece->get_player_id()).is_dark ? "dark"
                                                                                   : "light";
            SDL_Rect *src_rectangle = &pieces_texture_rectangles_.at(piece->get_symbol()).at(
                    graphics_type);

            SDL_Rect dist{x, y, piece_width_ - 5, piece_height_ - 5};
            SDL_RenderCopy(renderer_, texture_, src_rectangle, &dist);
        }
    }
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
            piece_height_ * mouse_i_,
            piece_width_, piece_height_
    };

    // TODO: remove the hardcoded color
    SDL_SetRenderDrawColor(renderer_, 135, 100, 43, 50);
    SDL_RenderFillRect(renderer_, &hover_rectangle);
}

void Renderer2D::render() {
    // calculate the frame rate
    unsigned int current_time = SDL_GetTicks();

    if (current_time - last_time_ >= 1000) {
        FPS_ = current_frames_count_;
        current_frames_count_ = 0;

        std::cout << FPS_ << std::endl;

        last_time_ = current_time;
    }

    current_frames_count_ += 1;

    // TODO: this has to change
    handle_events_();

    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    SDL_RenderClear(renderer_);

    render_table_();
    render_cursor_();
    render_game_();

    SDL_RenderPresent(renderer_);
}
