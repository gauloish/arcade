#include "utils.hpp"
#include "geometry.hpp"

#ifndef CANVAS_HPP
#define CANVAS_HPP

namespace canvas {
  class frame {
  private:
    short pattern[8];

  public:
    frame() {
      clear();
    }

    frame(const byte (&values)[8]) {
      short index = short();

      for (short &line: pattern) {
        line = values[index++];
      }
    }

    void clear() {
      for (short &line: pattern) {
        line = short();
      }
    }

    bool get(short line, short column) const {
      if (line < 0 or line > 7) {
        return false;
      }
      if (column < 0 or column > 7) {
        return false;
      }

      return numerics::get(pattern[line], column);
    }

    bool get(const geometry::point &point) const {
      short line = point.get(true);
      short column = point.get(false);

      return get(line, column);
    }

    void set(short line, short column, bool value) {
      pattern[line] = numerics::set(pattern[line], column, value);
    }

    void set(const geometry::point &point, bool value) {
      short line = point.get(true);
      short column = point.get(false);

      set(line, column, value);
    }

    template <const short size>
    void set(const geometry::object<size> &points, bool value) {
      for (auto &point: points) {
        set(point, value);
      }
    }

    template <const short size>
    void set(const geometry::object<size> &&points, bool value) {
      set<size>(points, value);
    }

    short* begin() const {
      return pattern;
    }

    short* end() const {
      return pattern + 8;
    }

    short &operator[](short index) {
      return pattern[index];
    }
  };

  class layer {
  private:
    short pattern[4];

  public:
    layer() {
      clear();
    }

    layer(const short (&values)[4]) {
      short index = short();

      for (short &line: pattern) {
        line = values[index++];
      }
    }

    void clear() {
      for (short &line: pattern) {
        line = short();
      }
    }

    short get(short index) const {
      if (index < 0 or index > 3) {
        return 0;
      }

      return numerics::encode(pattern[index]);
    }

    void set(short index, short value) {
      pattern[index] = value;
    }

    short* begin() const {
      return pattern;
    }

    short* end() const {
      return pattern + 4;
    }

    short &operator[](short index) {
      return pattern[index];
    }
  };
}

#endif