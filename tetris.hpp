#include "geometry.hpp"
#include "things.hpp"

#ifndef TETRIS_HPP
#define TETRIS_HPP

namespace tetris {
canvas::frame frame;

geometry::object<4> block;

short position{};
short rotation{};
short level{};

short steps{};
short count{};
short piece{};
short score{};

bool change{};
bool press{};

short rotate[2]{};

void start();
void generate();
void read();
void draw();
bool collision();
void repos();
bool verify();
void update(int);

canvas::frame image({0b11111111, 0b10000001, 0b10100001, 0b10100001, 0b10101101, 0b10101101, 0b10000001, 0b11111111});

void start() {
    numerics::seed();

    frame.clear();

    change = true;
}

void generate() {
    const static short lines[7][4] = {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1},
                                      {0, 0, 1, 1}, {1, 0, 0, 1}, {0, 0, 1, 1}};

    const static short columns[7][4] = {{-1, 0, 1, 2}, {-1, 0, 1, -1}, {-1, 0, 1, 0}, {-1, 0, 1, 1},
                                        {-1, 0, 0, 1}, {-1, 0, 1, 0},  {-1, 0, -1, 0}};

    block = geometry::object<4>(
        {geometry::point(lines[piece][0], columns[piece][0]), geometry::point(lines[piece][1], columns[piece][1]),
         geometry::point(lines[piece][2], columns[piece][2]), geometry::point(lines[piece][3], columns[piece][3])});

    block = geometry::orthos(block, block[1], 4 - rotate[0]);
}

void read() {
    press = bool(things::left.read() || things::right.read() || things::top.read() || things::bottom.read());

    short current = things::first.read(0, 7);

    short inferior = 0;
    short superior = 7;

    for (auto &part : block) {
        short edges[2][2] = {{0, 7}, {7, 0}};

        short line = part.get(true);
        short column = part.get(false);

        for (short inferior = 0; inferior < column; inferior++) {
            if (frame.get(line, inferior)) {
                edges[0][0] = inferior + 1;
            }
        }

        for (short superior = 7; superior > column; superior--) {
            if (frame.get(line, superior)) {
                edges[0][1] = superior - 1;
            }
        }

        for (short index = 0; index < 4; index++) {
            if (line == part.get(true)) {
                edges[1][0] = min(edges[1][0], part.get(false));
                edges[1][1] = max(edges[1][1], part.get(false));
            }
        }

        inferior = max(inferior, position + edges[0][0] - edges[1][0]);
        superior = min(superior, position + edges[0][1] - edges[1][1]);
    }

    position = current;

    if (position < inferior) {
        position = inferior;
    }
    if (position > superior) {
        position = superior;
    }

    rotate[0] = things::second.read(0, 3);

    if (rotate[0] == rotate[1]) {
        rotation = 0;
    } else {
        rotation = (rotate[0] < rotate[1] ? 3 : 1);
    }

    rotate[1] = rotate[0];

    if (change) {
        piece = numerics::rand(0, 6);

        generate();

        short shift = 0;

        for (auto &part : block) {
            shift = max(shift, part.get(true));
        }

        block.translate(-shift - 1, 0);

        change = false;
    }

    steps = 20 - 4 * level;
    count = 1 + count % steps;
}

void draw() {
    things::frame.set(block, true);

    for (short index = 0; index < 8; index++) {
        things::frame[index] |= frame[index];
    }

    short points = score;

    for (short index = 3; index > -1; index--) {
        things::layer.set(index, points % 10);

        points /= 10;
    }
}

bool collision() {
    for (auto &part : block) {
        short line = part.get(true);
        short column = part.get(false);

        if (line == 7) {
            return true;
        }
        if (frame.get(line + 1, column)) {
            return true;
        }
    }

    return false;
}

void repos() {
    short shift = position - block[1].get(false);

    block.translate(0, shift);

    if (count == steps) {
        if (collision()) {
            frame.set(block, true);

            for (short position = 0; position < 8; position++) {
                if (frame[position] == 255) {
                    for (short index = position; index > 0; index--) {
                        frame[index] = frame[index - 1];
                    }

                    score++;

                    frame[0] = 0;
                }
            }

            change = true;
            return;
        }

        block.translate(1, 0);
    }

    block = geometry::orthos(block, block[1], rotation);

    if (press) {
        while (not collision()) {
            block.translate(1, 0);
        }

        press = false;
    }

    things::frame.set(block, true);
}

bool verify() {
    if (count == steps) {
        if (collision()) {
            for (auto &part : block) {
                short line = part.get(true);

                if (line < 0) {
                    return true;
                }
            }
        }
    }

    return false;
}

void update(int value) {
    level = value;

    read();
    repos();
    draw();
}
}  // namespace tetris

#endif
