#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

namespace geometry {
class point {
   private:
    short line;
    short column;

   public:
    point() : line(short()), column(short()) {}

    point(short line, short column) : line(line), column(column) {}

    point(const point &other) {
        this->line = other.line;
        this->column = other.column;
    }

    short get(bool value) const { return value ? line : column; }

    void set(short line, short column) {
        this->line = line;
        this->column = column;
    }

    void set(const point &other) {
        short line = other.get(true);
        short column = other.get(false);

        set(line, column);
    }

    bool operator==(const point &other) const { return this->line == other.line && this->column == other.column; }

    void translate(short line, short column) {
        this->line += line;
        this->column += column;
    }

    void translate(const point &other) {
        short line = other.get(true);
        short column = other.get(false);

        translate(line, column);
    }
};

template <const short size>
class object {
   private:
    point dots[size];

   public:
    object() {}

    object(const object<size> &other) {
        short index = short();

        for (point &dot : dots) {
            dot = other[index++];
        }
    }

    object(const point (&values)[size]) {
        short index = short();

        for (point &dot : dots) {
            dot = values[index++];
        }
    }

    short length() const { return size; }

    point *begin() const { return dots; }

    point *end() const { return dots + size; }

    void translate(short line, short column) {
        for (point &dot : *this) {
            dot.translate(line, column);
        }
    }

    void translate(const point &other) {
        short line = other.get(true);
        short column = other.get(false);

        translate(line, column);
    }

    point &operator[](short index) { return dots[index]; }
};

point rotate(const point &other, const point &center, double angle) {
    angle *= PI / 180.0;

    double abscissa = other.get(false);
    double ordered = other.get(true);

    double horizontal = center.get(false);
    double vertical = center.get(true);

    double line = (abscissa - horizontal) * cos(angle) - (ordered - vertical) * sin(angle) + vertical;
    double column = (abscissa - horizontal) * sin(angle) + (ordered - vertical) * cos(angle) + horizontal;

    return point(line, column);
}

point orthos(const point &other, const point &center, int angle) {
    angle = (abs(angle) % 4);

    int osin[4] = {0, 1, 0, -1};
    int ocos[4] = {1, 0, -1, 0};

    double abscissa = other.get(false);
    double ordered = other.get(true);

    double horizontal = center.get(false);
    double vertical = center.get(true);

    double line = (abscissa - horizontal) * osin[angle] + (ordered - vertical) * ocos[angle] + vertical;
    double column = (abscissa - horizontal) * ocos[angle] - (ordered - vertical) * osin[angle] + horizontal;

    return point(line, column);
}

template <const short size>
object<size> rotate(const object<size> &other, const point &center, double angle) {
    object<size> rotated{other};

    for (int index = 0; index < size; index++) {
        rotated[index] = rotate(other[index], center, angle);
    }

    return rotated;
}

template <const short size>
object<size> orthos(const object<size> &other, const point &center, double angle) {
    object<size> rotated{other};

    for (int index = 0; index < size; index++) {
        rotated[index] = orthos(other[index], center, angle);
    }

    return rotated;
}
}  // namespace geometry

#endif
