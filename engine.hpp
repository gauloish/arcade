#include "invaders.hpp"
#include "pong.hpp"
#include "snake.hpp"
#include "tetris.hpp"

#include "paint.hpp"

#include "things.hpp"
#include "utils.hpp"

#ifndef ENGINE_HPP
#define ENGINE_HPP

namespace engine {
int choice = 0;

void (*started[])(void) = {
    pong::start,     //
    snake::start,    //
    tetris::start,   //
    invaders::start  //
};

void (*updated[])(int) = {
    pong::update,     //
    snake::update,    //
    tetris::update,   //
    invaders::update  //
};

bool (*verified[])(void) = {
    pong::verify,     //
    snake::verify,    //
    tetris::verify,   //
    invaders::verify  //
};

canvas::frame image[4] = {pong::image, snake::image, tetris::image, invaders::image};

/**
 * @brief Change screen and display brightness
 */
void brightness() {
    int bright = things::bright.read(0, 100);

    things::screen.brightness(bright);
    things::display.brightness(bright);
}

/**
 * @brief Read level game in knob
 *
 * @return Level game
 */
int difficulty() {
    short level = things::level.read(0, 3);

    return level;
}

/**
 * @brief Render in screen and display
 */
void render() {
    things::screen.render(things::frame);
    things::display.render(things::layer);
}

/**
 * @brief Clear screen and display
 */
void clear() {
    things::frame.clear();
    things::layer.clear();
}

/**
 * @brief Start game
 */
void start() {  //
    started[choice]();
}

/**
 * @brief Update game state
 */
void update() {
    brightness();

    short level = difficulty();

    updated[choice](level);
}

/**
 * @brief Verify defeat
 *
 * @return Defeat
 */
bool verify() {  //
    return verified[choice]();
}

/**
 * @brief Selector to select game
 */
void selector() {
    clear();

    while (true) {
        choice = things::first.read(0, 3);

        for (int index = 0; index < 8; index++) {
            things::frame[index] = image[choice][index];
        }

        bool stop = bool(things::left.read() || things::right.read() || things::top.read() || things::bottom.read());

        if (stop) {
            for (int index = 0; index < 4; index++) {
                things::layer.set(index, 10);
            }

            brightness();
            render();

            delay(1000);
            break;
        }

        render();
    }
}

/**
 * @brief Initialize game
 */
void init() {
    numerics::seed();

    Serial.begin(9600);

    selector();
    start();
}

/**
 * @brief Run
 */
void run() {
    clear();
    update();
    render();

    if (verify()) {
        delay(500);

        start();
    }

    delay(things::period);
}
}  // namespace engine

#endif
