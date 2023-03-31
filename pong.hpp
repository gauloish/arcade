#include "utils.hpp"
#include "things.hpp"
#include "geometry.hpp"

#ifndef PONG_HPP
#define PONG_HPP

namespace pong {
  geometry::object<3> left;
  geometry::object<3> right;

  geometry::point ball;
  geometry::point sense;

  short first{};
  short second{};
  short level{};

  short steps{};
  short count{};

  short one{};
  short two{};

  void start();
  void read();
  void draw();
  void repos();
  int collision();
  bool verify();
  void update(int);

  canvas::frame image({
    0b11111111,
    0b10000001,
    0b10000101,
    0b10100101,
    0b10100101,
    0b10100001,
    0b10000001,
    0b11111111
  });

  void start() {
    read();
    repos();

    ball.set(
      numerics::rand(3, 4),
      numerics::rand(3, 4)
    );

    sense.set(
      1 - 2 * numerics::rand(0, 1),
      1 - 2 * numerics::rand(0, 1)
    );

    short one = 0;
    short two = 0;
  }

  void read() {
    int limit = 5;

    if (level > 1) {
      limit = 6;
    }
    if (level > 2) {
      limit = 7;
    }

    first = things::first.read(0, limit);
    second = things::second.read(0, limit);

    steps = 20 - 4 * level;
    count = 1 + count % steps;
  }

  void draw() {
    things::frame.set(left, true);
    things::frame.set(right, true);

    things::frame.set(ball, true);

    things::layer.set(0, one / 10);
    things::layer.set(1, one % 10);
    things::layer.set(2, two / 10);
    things::layer.set(3, two % 10);

    things::display.separator(true);
  }

  void repos() {
    for (byte index = 0; index < 3; index++) {
      left[index].set(first + index, 0);
      right[index].set(second + index, 7);
    }

    if (level > 1) {
      left[2].set(first, 0);
      right[2].set(second, 7);
    }
    if (level > 2) {
      left[1].set(first, 0);
      right[1].set(second, 7);
    }

    if (count == steps) {
      int movement = collision();

      switch (movement) {
        case 0:
          break;
        case 1:
          sense.set(
            -sense.get(true),
            sense.get(false)
          );
          break;
        case 2:
          sense.set(
            sense.get(true),
            -sense.get(false)
          );
          break;
        case 3:
          sense.set(
            -sense.get(true),
            -sense.get(false)
          );
          break;
      }

      ball.translate(sense);
    }
  }

  int collision() {
    byte line = ball.get(true);
    byte column = ball.get(false);

    int increase = 0;

    if (level > 1) {
      increase = 1;
    }
    if (level > 2) {
      increase = 2;
    }

    if (line == 0) {
      if (column == 1 and first == 0) {
        return 3;
      }
      if (column == 6 and second == 0) {
        return 3;
      }

      return 1;
    }
    if (line == 7) {
      if (column == 1 and first == 5 + increase) {
        return 3;
      }
      if (column == 6 and second == 5 + increase) {
        return 3;
      }

      return 1;
    }

    if (column == 1) {
      if (first > line) {
        return 0;
      }
      if (first + 2 < line + increase) {
        return 0;
      }

      return 2;
    }
    if (column == 6) {
      if (second > line) {
        return 0;
      }
      if (second + 2 < line + increase) {
        return 0;
      }

      return 2;
    }

    return 0;
  }

  bool verify() {
    if (count == steps) {
      byte column = ball.get(false);

      if (column == 0) {
        two++;
        return true;
      }
      if (column == 7) {
        one++;
        return true;
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
}

#endif