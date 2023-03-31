#include "things.hpp"
#include "geometry.hpp"

#ifndef SNAKE_HPP
#define SNAKE_HPP

namespace snake {
  static const short size{64};

  geometry::object<size> body;

  geometry::point food;
  geometry::point sense;

  bool left{};
  bool right{};
  bool top{};
  bool bottom{};

  short level{};

  short steps{};
  short count{};

  short length{};
  short points{};

  void start();
  void read();
  void draw();
  void repos();
  bool found();
  void change();
  bool verify();
  void update(int);

  canvas::frame image({
    0b11111111,
    0b10000001,
    0b10011001,
    0b10010001,
    0b10011101,
    0b10111101,
    0b10000001,
    0b11111111
  });

  void start() {
    read();
    repos();

    numerics::seed();

    length = 1;
    points = 1;

    short line = numerics::rand(3, 4);
    short column = numerics::rand(3, 4);

    for (auto &part: body) {
      part.set(line, column);
    }

    food.set(
      numerics::rand(0, 7),
      numerics::rand(0, 7)
    );

    sense.set(0, 0);
  }

  void read() {
    left = things::left.read();
    bottom = things::right.read();
    top = things::top.read();
    right = things::bottom.read();

    short line = sense.get(true);
    short column = sense.get(false);

    if (line or (not (line or column))) {
      if (left) {
        sense.set(0, -1);
      }
      else if (right) {
        sense.set(0, 1);
      }
    }
    if (column or (not (line or column))) {
      if (top) {
        sense.set(-1, 0);
      }
      else if (bottom) {
        sense.set(1, 0);
      }
    }

    steps = 20 - 4 * level;
    count = 1 + count % steps;
  }

  void draw() {
    things::frame.set(body, true);

    things::frame.set(food, true);

    things::layer.set(0, -1);
    things::layer.set(1, points / 10);
    things::layer.set(2, points % 10);
    things::layer.set(3, -1);

    things::display.separator(false);
  }

  bool verify() {
    for (short index = 1; index < length; index++) {
      if (body[0] == body[index]) {
        return true;
      }
    }

    short line = body[0].get(true);
    short column = body[1].get(false);

    if (line < 0 or line > 7) {
      return true;
    }
    if (column < 0 or column > 7) {
      return true;
    }

    return false;
  }

  bool found() {
    return body[0] == food;
  }

  void change() {
    while (true) {
      food.set(
        numerics::rand(0, 7),
        numerics::rand(0, 7)
      );

      bool valid = true;

      for (auto &part: body) {
        if (food == part) {
          valid = false;

          break;
        }
      }

      if (valid) {
        break;
      }
    }
  }

  void repos() {
    if (count == steps) {
      short index = length - 1;

      while (index) {
        body[index] = body[index - 1];

        index--;
      }

      body[index].translate(sense);

      if (found()) {
        change();

        length++;
        points++;
      }
      else {
        index = length;

        while (index < size) {
          body[index] = body[index - 1];

          index++;
        }
      }
    }
  }

  void update(int value) {
    level = value;

    read();
    repos();
    draw();
  }
}

#endif