#include "MyEngine"
using namespace MyEngine;


struct NumRoot {
    Matrix2D<SDL_Color>::Position start_pos, end_pos;
};

//////////////////////////////////////////////////////////////////////////////
NumRoot root_1 = {{0, 4}, {3, 15}};
NumRoot root_2 = {{4, 0}, {16, 3}};
NumRoot root_3 = {{4, 16}, {16, 19}};
NumRoot root_4 = {{17, 4}, {20, 15}};
NumRoot root_5 = {{21, 0}, {31, 3}};
NumRoot root_6 = {{21, 16}, {31, 19}};
NumRoot root_7 = {{32, 4}, {35, 15}};
//////////////////////////////////////////////////////////////////////////////


Matrix2D<SDL_Color> generateNumber(char ch = '0') {
    Matrix2D<SDL_Color> matrix(36, 20);
    switch (ch) {
    case '0':
        matrix.fillRect(root_1.start_pos, root_1.end_pos, StdColor::White);
        matrix.fillRect(root_2.start_pos, root_2.end_pos, StdColor::White);
        matrix.fillRect(root_3.start_pos, root_3.end_pos, StdColor::White);
        matrix.fillRect(root_5.start_pos, root_5.end_pos, StdColor::White);
        matrix.fillRect(root_6.start_pos, root_6.end_pos, StdColor::White);
        matrix.fillRect(root_7.start_pos, root_7.end_pos, StdColor::White);
        break;
    case '1':
        matrix.fillRect(root_3.start_pos, root_3.end_pos, StdColor::White);
        matrix.fillRect(root_6.start_pos, root_6.end_pos, StdColor::White);
        break;
    case '2':
        matrix.fillRect(root_1.start_pos, root_1.end_pos, StdColor::White);
        matrix.fillRect(root_3.start_pos, root_3.end_pos, StdColor::White);
        matrix.fillRect(root_4.start_pos, root_4.end_pos, StdColor::White);
        matrix.fillRect(root_5.start_pos, root_5.end_pos, StdColor::White);
        matrix.fillRect(root_7.start_pos, root_7.end_pos, StdColor::White);
        break;
    case '3':
        matrix.fillRect(root_1.start_pos, root_1.end_pos, StdColor::White);
        matrix.fillRect(root_3.start_pos, root_3.end_pos, StdColor::White);
        matrix.fillRect(root_4.start_pos, root_4.end_pos, StdColor::White);
        matrix.fillRect(root_6.start_pos, root_6.end_pos, StdColor::White);
        matrix.fillRect(root_7.start_pos, root_7.end_pos, StdColor::White);
        break;
    case '4':
        matrix.fillRect(root_2.start_pos, root_2.end_pos, StdColor::White);
        matrix.fillRect(root_3.start_pos, root_3.end_pos, StdColor::White);
        matrix.fillRect(root_4.start_pos, root_4.end_pos, StdColor::White);
        matrix.fillRect(root_6.start_pos, root_6.end_pos, StdColor::White);
        break;
    case '5':
        matrix.fillRect(root_1.start_pos, root_1.end_pos, StdColor::White);
        matrix.fillRect(root_2.start_pos, root_2.end_pos, StdColor::White);
        matrix.fillRect(root_4.start_pos, root_4.end_pos, StdColor::White);
        matrix.fillRect(root_6.start_pos, root_6.end_pos, StdColor::White);
        matrix.fillRect(root_7.start_pos, root_7.end_pos, StdColor::White);
        break;
    case '6':
        matrix.fillRect(root_1.start_pos, root_1.end_pos, StdColor::White);
        matrix.fillRect(root_2.start_pos, root_2.end_pos, StdColor::White);
        matrix.fillRect(root_4.start_pos, root_4.end_pos, StdColor::White);
        matrix.fillRect(root_5.start_pos, root_5.end_pos, StdColor::White);
        matrix.fillRect(root_6.start_pos, root_6.end_pos, StdColor::White);
        matrix.fillRect(root_7.start_pos, root_7.end_pos, StdColor::White);
        break;
    case '7':
        matrix.fillRect(root_1.start_pos, root_1.end_pos, StdColor::White);
        matrix.fillRect(root_3.start_pos, root_3.end_pos, StdColor::White);
        matrix.fillRect(root_6.start_pos, root_6.end_pos, StdColor::White);
        break;
    case '8':
        matrix.fillRect(root_1.start_pos, root_1.end_pos, StdColor::White);
        matrix.fillRect(root_2.start_pos, root_2.end_pos, StdColor::White);
        matrix.fillRect(root_3.start_pos, root_3.end_pos, StdColor::White);
        matrix.fillRect(root_4.start_pos, root_4.end_pos, StdColor::White);
        matrix.fillRect(root_5.start_pos, root_5.end_pos, StdColor::White);
        matrix.fillRect(root_6.start_pos, root_6.end_pos, StdColor::White);
        matrix.fillRect(root_7.start_pos, root_7.end_pos, StdColor::White);
        break;
    case '9':
        matrix.fillRect(root_1.start_pos, root_1.end_pos, StdColor::White);
        matrix.fillRect(root_2.start_pos, root_2.end_pos, StdColor::White);
        matrix.fillRect(root_3.start_pos, root_3.end_pos, StdColor::White);
        matrix.fillRect(root_4.start_pos, root_4.end_pos, StdColor::White);
        matrix.fillRect(root_6.start_pos, root_6.end_pos, StdColor::White);
        matrix.fillRect(root_7.start_pos, root_7.end_pos, StdColor::White);
        break;
    default:
        matrix.fillRect({0, 0}, {matrix.rows() - 1, 10}, StdColor::White);
        break;
    }

    return matrix;
}

void initNumber(std::array<std::shared_ptr<Texture>, 10>& number_list, Renderer* renderer) {
    for (int i = '0'; i <= '9'; ++i) {
        auto surface = Algorithm::drawSurfaceByColorMap(generateNumber(static_cast<char>(i)));
        number_list[i - '0'] = std::make_shared<Texture>(std::move(surface), renderer);
    }
}

struct Controller {
    Graphics::Rectangle rect;
    float max_height{}, min_height{};
    float speed{2.5f};
    bool is_pressed_down{false};
    bool is_move_down{false};

    void moveUp() {
        rect.move(rect.position().x, std::clamp(rect.position().y - speed, min_height, max_height));
    }

    void moveDown() {
        rect.move(rect.position().x, std::clamp(rect.position().y + speed, min_height, max_height));
    }

    [[nodiscard]] float getY() const {
        return rect.position().y;
    }
};

struct Ball {
    Texture texture;
    Size window_size;
    void resetBall() {
        texture.property()->move(window_size.width / 2 - texture.property()->size().width / 2,
        window_size.height / 2 - texture.property()->size().height / 2);
    }
};

struct Score {
    explicit Score(const std::array<std::shared_ptr<Texture>, 10> &textures, const Vector2& position)
        : _textures{textures}, _position{position} {
        initScore();
    }
    void getPoint() { if (++_score >= 1000) --_score; updateScore(); }
    void maxPoint() { _score = 999; updateScore(); }
    void clearPoint() { _score = 0; updateScore(); }
    [[nodiscard]] Texture* self(size_t idx) const { return _numerics[idx].get(); }
private:
    std::array<std::shared_ptr<Texture>, 10> _textures;
    std::array<std::shared_ptr<Texture>, 3> _numerics{};
    Vector2 _position;
    uint16_t _score{0};
    void initScore() {
        uint8_t n1 = _score / 100, n2 = (_score / 10) % 10, n3 = _score % 10;
        _numerics[0] = std::make_shared<Texture>(_textures[n1]->surface(), _textures[0]->renderer(), true);
        _numerics[1] = std::make_shared<Texture>(_textures[n2]->surface(), _textures[1]->renderer(), true);
        _numerics[2] = std::make_shared<Texture>(_textures[n3]->surface(), _textures[2]->renderer(), true);
        _numerics[0]->property()->move(_position);
        _numerics[1]->property()->move(_position.x + _numerics[0]->property()->size().width + 8.f, _position.y);
        _numerics[2]->property()->move(_numerics[1]->property()->position().x + _numerics[1]->property()->size().width + 8.f, _position.y);
    }
    void updateScore() const {
        uint8_t n1 = _score / 100, n2 = (_score / 10) % 10, n3 = _score % 10;
        _numerics[0]->setImageFromSurface(_textures[n1]->surface(), true);
        _numerics[1]->setImageFromSurface(_textures[n2]->surface(), true);
        _numerics[2]->setImageFromSurface(_textures[n3]->surface(), true);
        _numerics[0]->property()->move(_position);
        _numerics[1]->property()->move(_position.x + _numerics[0]->property()->size().width + 8.f, _position.y);
        _numerics[2]->property()->move(_numerics[1]->property()->position().x + _numerics[1]->property()->size().width + 8.f, _position.y);
    }
};

enum GameStatus {
    NoGaming,
    Gaming,
    GamePaused,
    GameOver
};

GameStatus status;

void initController(Window *&window, Controller &con1, Controller &con2) {
    auto win_geo = toGeometryFloat(window->geometry());
    con1.rect.move(60, win_geo.size.height / 2 - con1.rect.size().height / 2);
    con2.rect.move(win_geo.size.width - 60 - con2.rect.size().width,
        win_geo.size.height / 2 - con2.rect.size().height / 2);
}

int main() {
    Engine engine;
    engine.setFPS(60);
    auto window = new Window(&engine, "Pong", 1024, 768);
    window->renderer()->setVSyncMode(Renderer::Enabled);
    std::array<std::shared_ptr<Texture>, 10> numbers_list{};
    initNumber(numbers_list, window->renderer());
    Size windows_size = {static_cast<float>(window->geometry().width), static_cast<float>(window->geometry().height)};
    Score score_1(numbers_list, Vector2(windows_size.width / 2 - 120, 60));
    Score score_2(numbers_list, Vector2(windows_size.width / 2 + 20, 60));
    Controller player1(Graphics::Rectangle(0, 0, 32, windows_size.height / 5.f,
        0, {}, StdColor::White),
        windows_size.height - windows_size.height / 5.f, 0, 3.f, false);
    Controller player2(Graphics::Rectangle(0, 0, 32, 143,
        0, {}, StdColor::White),
        windows_size.height - 143.f, 0, 3.f, false);
    Ball ball(Texture(Algorithm::drawFilledCircleSurface(32), window->renderer()), windows_size);
    SFX sfx(AudioSystem::global()->mixer(), SFX::SINE_HZ_C5);
    ball.resetBall();
    initController(window, player1, player2);
    bool is_muted{false}, is_rand{false}, is_NB{false};
    std::string text{"Press 'R' key to start game, type 'F1' to display for help!"};
    Timer delayer(100, [&] {
        delay(2);
        status = GameStatus::Gaming;
        ball.resetBall();
        text.clear();
        is_rand = false;
    });
    Timer win_sfx(10, [&sfx] {
        uint8_t i = 2;
        while (i--) {
            sfx.play(false, 25);
            size_t idx = sfx.lastIndex();
            sfx.setSpeedAndPitch(i == 1 ? 1.f : 2.f, idx);
            delayMS(125);
            sfx.stop(idx, i == 0 ? 150 : 0);
        }
    });
    Timer push_sfx(10, [&sfx] {
        sfx.play(false, 25);
        size_t idx = sfx.lastIndex();
        sfx.setSpeedAndPitch(1.f, idx);
        delayMS(150);
        sfx.stop(idx, 50);
    });
    Timer paused_sfx(10, [&sfx] {
        uint8_t i = 4;
        while (i--) {
            sfx.play(false, 25);
            size_t idx = sfx.lastIndex();
            sfx.setSpeedAndPitch(i % 2 ? 2.f : 1.5f, idx);
            delayMS(i > 0 ? 125 : 200);
            sfx.stop(idx, i == 0 ? 150 : 0);
        }
    });
    bool check_collider = true;
    Timer disabled_check(1, [&check_collider] { delayMS(500); check_collider = true; });

    bool show_help = false;
    window->installPaintEvent([&] (Renderer*) {
        auto ev = EventSystem::global();
        static float dx = 0.f,
                     dy = 0.f;
        if (!is_rand) {
            auto seg = RandomGenerator::randUInt(0, 100);
            dx = (seg > 50 ? 1.f : -1.f) * RandomGenerator::randFloat(2.f, 6.f);
            dy = RandomGenerator::randFloat(-5.f, 5.f);
            is_rand = true;
        }

        if (status == Gaming) {
            if (ev->captureKeyboard(SDL_SCANCODE_W)) {
                if (!player1.is_pressed_down) {
                    player1.is_pressed_down = true;
                    player1.is_move_down = false;
                }
            } else if (ev->captureKeyboard(SDL_SCANCODE_S)) {
                if (!player1.is_pressed_down) {
                    player1.is_pressed_down = true;
                    player1.is_move_down = true;
                }
            } else {
                player1.is_pressed_down = false;
            }

            if (ev->captureKeyboard(SDL_SCANCODE_UP)) {
                if (!player2.is_pressed_down) {
                    player2.is_pressed_down = true;
                    player2.is_move_down = false;
                }
            } else if (ev->captureKeyboard(SDL_SCANCODE_DOWN)) {
                if (!player2.is_pressed_down) {
                    player2.is_pressed_down = true;
                    player2.is_move_down = true;
                }
            } else {
                player2.is_pressed_down = false;
            }

            if (player1.is_pressed_down) {
                if (player1.is_move_down) player1.moveDown();
                else player1.moveUp();
            }
            if (player2.is_pressed_down) {
                if (player2.is_move_down) player2.moveDown();
                else player2.moveUp();
            }

            auto ball_geo = ball.texture.property()->geometry();
            auto win_size = toGeometryFloat(window->geometry()).size;
            
            static GeometryF geo(0, 0, win_size.width, win_size.height);
            if (Algorithm::compareGeometryF(ball_geo, geo) != 2) {
                auto win_width = win_size.width;
                auto win_height = win_size.height;
                auto new_x = std::clamp(ball_geo.pos.x, 0.f, win_width - ball_geo.size.width);
                auto new_y = std::clamp(ball_geo.pos.y, 0.f, win_height - ball_geo.size.height);
                ball_geo.move(new_x, new_y);
                float rand_probability = RandomGenerator::randFloat(-0.5f, 0.5f);
                dy = -dy;
                dy *= 1.f + rand_probability;
                if (is_NB && (new_x <= 1.f || new_x >= win_width - ball_geo.size.width - 1.f)) dx = -dx;
                if (!is_muted && !push_sfx.enabled()) push_sfx.start();
            }
            if (!is_NB && ball_geo.pos.x <= player1.rect.size().width + player1.rect.position().x - ball_geo.size.width) {
                // player 2 win
                status = GameOver;
                score_2.getPoint();
                text = "Player 2 win";
                if (!is_muted && !win_sfx.enabled()) win_sfx.start();
                delayer.start();
                
                return;
            }
            if (!is_NB && ball_geo.pos.x >= win_size.width - ball_geo.size.width - player2.rect.size().width) {
                // player 1 win
                status = GameOver;
                score_1.getPoint();
                text = "Player 1 win";
                if (!is_muted && !win_sfx.enabled()) win_sfx.start();
                delayer.start();
                return;
            }
            if (check_collider && (Algorithm::compareGeometryF(ball_geo, player1.rect.geometry()) > 0 ||
                Algorithm::compareGeometryF(ball_geo, player2.rect.geometry()) > 0)) {
                dx = -dx;
                uint8_t probability = RandomGenerator::randUInt(0, 100);
                float rand_probability = RandomGenerator::randFloat(0.f, 1.f);
                dy = probability >= 50 ? dy : -dy;
                dy *= 1.f + rand_probability;
                if (!is_muted && !push_sfx.enabled()) push_sfx.start();
                check_collider = false;
                disabled_check.start();
            }
            ball.texture.property()->move(std::clamp(ball_geo.pos.x + dx, -0.1f, win_size.width - ball_geo.size.width + 0.1f),
                std::clamp(ball_geo.pos.y + dy, -0.1f, win_size.height - ball_geo.size.height + 0.1f));
        }
    });

    Graphics::Rectangle back(0, 0, 420, 100, 0,
        {}, {255, 255, 255, 128});
    Graphics::Line line1(player1.rect.position().x + player1.rect.size().width - 2.f, 0,
        player1.rect.position().x + player1.rect.size().width - 2.f, windows_size.height, 4, StdColor::DarkGray);
    Graphics::Line line2(player2.rect.position().x, 0, player2.rect.position().x, windows_size.height, 4, StdColor::DarkGray);

    window->installPaintEvent([&] (Renderer* r) {
        r->fillBackground(SDL_Color(24, 24, 28, 255));
        r->drawLine(&line1);
        r->drawLine(&line2);
        r->drawRectangle(&player1.rect);
        r->drawRectangle(&player2.rect);
        ball.texture.draw();
        for (int i = 0; i < 3; ++i) {
            score_1.self(i)->draw();
            score_2.self(i)->draw();
        }
        r->drawDebugText(text, {20, 20}, StdColor::White);
        if (show_help) {
            r->setBlendMode(SDL_BLENDMODE_BLEND);
            r->drawRectangle(&back);
            r->drawDebugText("Type 'W' or 'S' to control player 1", {20, 30});
            r->drawDebugText("Type UP key or DOWN key to control player 2", {20, 40});
            r->drawDebugText("Type Space to resume/paused the current game", {20, 50});
            r->drawDebugText("Type Esc key to restart the game", {20, 60});
            r->drawDebugText("Type 'M' key to muted the SFX", {20, 70});
            r->drawDebugText("Type 'F1' key again to hide the help info", {20, 80});
            r->setBlendMode(SDL_BLENDMODE_NONE);
        }
    });



    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [&] (const SDL_Event &e) {
        static std::deque<SDL_Scancode> NB_cmd;
        if (e.type == SDL_EVENT_KEY_DOWN) {
            if (e.key.key == SDLK_SPACE) {
                if (status == GameStatus::Gaming) {
                    status = GameStatus::GamePaused;
                    text.assign("Game Paused");
                    if (!is_muted && !paused_sfx.enabled()) paused_sfx.start();
                } else if (status == GameStatus::GamePaused) {
                    status = GameStatus::Gaming;
                    text.clear();
                    if (!is_muted && !paused_sfx.enabled()) paused_sfx.start();
                }
            }
            if (e.key.key == SDLK_R) {
                if (status == NoGaming) {
                    status = GameStatus::Gaming;
                    initController(window, player1, player2);
                    ball.resetBall();
                    text.clear();
                }
            }
            if (e.key.key == SDLK_M) {
                is_muted = !is_muted;
            }
            if (e.key.key == SDLK_F1) { show_help = !show_help; }
            if (e.key.key == SDLK_ESCAPE) {
                status = GameStatus::NoGaming;
                score_1.clearPoint();
                score_2.clearPoint();
                is_NB = false;
                check_collider = true;
                player1.rect.resize(player1.rect.size().width, windows_size.height / 5.f);
                player2.rect.resize(player2.rect.size().width, windows_size.height / 5.f);
                player1.speed = 3.f;
                player2.speed = 3.f;
                player1.max_height = windows_size.height - player1.rect.size().height;
                player2.max_height = windows_size.height - player2.rect.size().height;
                initController(window, player1, player2);
                ball.resetBall();
                text.assign("Press 'R' key to start game");
            }
            if (e.key.key == SDLK_A) {
                if (status == NoGaming) return;
                static std::deque<SDL_Scancode> NB = {
                    SDL_SCANCODE_UP, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_DOWN,
                    SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT,
                    SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT,
                    SDL_SCANCODE_B, SDL_SCANCODE_A, SDL_SCANCODE_B
                };
                if (!is_NB && std::ranges::equal(NB_cmd, NB)) {
                    is_NB = true;
                    if (!is_muted && !win_sfx.enabled()) win_sfx.start();
                    player1.rect.resize(player1.rect.size().width, 400.f);
                    player2.rect.resize(player2.rect.size().width, 400.f);
                    player1.speed = 6.f;
                    player2.speed = 6.f;
                    player1.max_height = windows_size.height - player1.rect.size().height;
                    player2.max_height = windows_size.height - player2.rect.size().height;
                    initController(window, player1, player2);
                    score_1.maxPoint();
                    score_2.maxPoint();
                }
            }

            if (NB_cmd.size() + 1 >= 12) {
                NB_cmd.pop_front();
            }
            NB_cmd.push_back(e.key.scancode);
        }
    });

    window->show();
    return engine.exec();
}