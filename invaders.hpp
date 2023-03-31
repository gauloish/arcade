#include "geometry.hpp"
#include "things.hpp"
#include "utils.hpp"

#ifndef INVADERS_HPP
#define INVADERS_HPP

namespace invaders {
geometry::object<4> ship;
geometry::object<4> enemy;

geometry::point bullet;

short position{};

short deads{};
short points{};
short level{};

short steps{};
short count{};

bool shot{};

void start();
void read();
int evade();
void draw();
void repos();
bool verify();
void update(int);

canvas::frame image({0b11111111, 0b10000001, 0b10011101, 0b10001001, 0b10010001, 0b10111001, 0b10000001, 0b11111111});

/**
 * @brief Start game
 */
void start() {
    read();
    repos();

    int initial = numerics::rand(1, 6);

    enemy[0].set(-2, initial);
    enemy[1].set(-1, initial);
    enemy[2].set(-2, initial - 1);
    enemy[3].set(-2, initial + 1);

    bullet.set(16, 16);

    shot = false;
}

/**
 * @brief Read peripherals state
 */
void read() {
    position = things::first.read(1, 6);

    bool trigger = false;

    trigger = trigger || things::left.read();
    trigger = trigger || things::right.read();
    trigger = trigger || things::top.read();
    trigger = trigger || things::bottom.read();

    if (trigger) {
        shot = trigger;

        bullet.set(6, position);
    }

    steps = 20 - 4 * level;
    count = 1 + count % steps;
}

/**
 * @brief Verify if evade
 *
 * @return Evade
 */
int evade() { return bullet.get(true) == -1; }

/**
 * @brief Verify defeat
 *
 * @return defeat
 */
bool verify() {
    short posBullet[] = {bullet.get(true), bullet.get(false)};

    for (int first = 0; first < 4; first++) {
        for (int second = 0; second < 4; second++) {
            short posShip[] = {ship[first].get(true), ship[first].get(false)};
            short posEnemy[] = {enemy[second].get(true), enemy[second].get(false)};

            if (posBullet[0] == posEnemy[0] and posBullet[1] == posEnemy[1]) {
                points++;
                return true;
            }
            if (posShip[0] == posEnemy[0] and posShip[1] == posEnemy[1]) {
                deads++;
                return true;
            }
            if (posEnemy[0] == 8) {
                deads++;
                return true;
            }
        }
    }

    return false;
}

/**
 * @brief Update position
 */
void repos() {
    if (shot) {
        if (count % (steps / 2) == 0) {
            if (evade()) {
                shot = false;
            } else {
                bullet.translate(-1, 0);
            }
        }
    }

    if (count == steps) {
        enemy.translate(1, 0);
    }

    ship[0].set(7, position);
    ship[1].set(6, position);
    ship[2].set(7, position - 1);
    ship[3].set(7, position + 1);
}

/**
 * @brief Draw game in screen and display
 */
void draw() {
    things::frame.set(ship, true);
    things::frame.set(enemy, true);
    things::frame.set(bullet, true);

    things::layer.set(0, deads / 10);
    things::layer.set(1, deads % 10);
    things::layer.set(2, points / 10);
    things::layer.set(3, points % 10);

    things::display.separator(true);
}

/**
 * @brief Update game state
 *
 * @param value Game level
 */
void update(int value) {
    level = value;

    read();
    repos();
    draw();
}
}  // namespace invaders

#endif
