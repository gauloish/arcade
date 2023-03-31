#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

namespace geometry {
/**
 * @class point
 * @brief Emulate a point in plane
 *
 */
class point {
   private:
    short line;
    short column;

   public:
    /**
     * @brief Initialize point with default values
     */
    point() : line(short()), column(short()) {}

    /**
     * @brief Initialize point with given values
     *
     * @param line Vertical position
     * @param column Horizontal position
     */
    point(short line, short column) : line(line), column(column) {}

    /**
     * @brief Initialize point by copy
     *
     * @param other Other point
     */
    point(const point &other) {
        this->line = other.line;
        this->column = other.column;
    }

    /**
     * @brief Return a coordinate value
     *
     * @param value Coordinate (false to x, true to y)
     * @return Coordinate value
     */
    short get(bool value) const { return value ? line : column; }

    /**
     * @brief Set point position
     *
     * @param line Vertical coordinate
     * @param column Horizontal coordinate
     */
    void set(short line, short column) {
        this->line = line;
        this->column = column;
    }

    /**
     * @brief Set point position by copy
     *
     * @param other Other point
     */
    void set(const point &other) {
        short line = other.get(true);
        short column = other.get(false);

        set(line, column);
    }

    /**
     * @brief Compare two points by its coordinates
     *
     * @param other Other point to compare
     * @return Value comparation
     */
    bool operator==(const point &other) const { return this->line == other.line && this->column == other.column; }

    /**
     * @brief Translate point position
     *
     * @param line Vertical value to translate
     * @param column Horizontal value to translate
     */
    void translate(short line, short column) {
        this->line += line;
        this->column += column;
    }

    /**
     * @brief Translate point from other point
     *
     * @param other Other point to do translate
     */
    void translate(const point &other) {
        short line = other.get(true);
        short column = other.get(false);

        translate(line, column);
    }
};

/**
 * @brief Object is a point collection
 */
template <const short size>
class object {
   private:
    point dots[size];

   public:
    /**
     * @brief Initialize object with default values to points
     */
    object() {}

    /**
     * @brief Initialize object by copy
     *
     * @param other Other object to copy
     */
    object(const object<size> &other) {
        short index = short();

        for (point &dot : dots) {
            dot = other[index++];
        }
    }

    /**
     * @brief Initialize object by points array
     *
     * @param values Points array
     * @param size Amount of points in object
     */
    object(const point (&values)[size]) {
        short index = short();

        for (point &dot : dots) {
            dot = values[index++];
        }
    }

    /**
     * @brief Return object length, the amount of points in object
     *
     * @return [TODO:return]
     */
    short length() const { return size; }

    /**
     * @brief Return pointer to begin of points array
     *
     * @return Pointer to begin of points array
     */
    point *begin() const { return dots; }

    /**
     * @brief Return pointer to end of points array
     *
     * @return Pointer to end of points array
     */
    point *end() const { return dots + size; }

    /**
     * @brief Translate object position
     *
     * @param line Vertical value
     * @param column Horizontal value
     */
    void translate(short line, short column) {
        for (point &dot : *this) {
            dot.translate(line, column);
        }
    }

    /**
     * @brief Translate object position by a point
     *
     * @param other Other point
     */
    void translate(const point &other) {
        short line = other.get(true);
        short column = other.get(false);

        translate(line, column);
    }

    /**
     * @brief Implementation to [] operator
     *
     * @param index Position in points array
     * @return Point in points array
     */
    point &operator[](short index) { return dots[index]; }
};

/**
 * @brief Rotate a point around other point by an angle
 *
 * @param other Point to rotate
 * @param center Point in rotation center
 * @param angle Angle to rotate
 * @return Point rotated
 */
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

/**
 * @brief Rotate a point around other point by an orthogonal angle
 *
 * @param other Point to rotate
 * @param center Point in rotation center
 * @param angle Angle to rotate
 * @return Point rotated
 */
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

/**
 * @brief Rotate a object around a point by an angle
 *
 * @param other Object to rotate
 * @param center Point in rotation center
 * @param angle Angle to rotate
 * @return Object rotated
 */
template <const short size>
object<size> rotate(const object<size> &other, const point &center, double angle) {
    object<size> rotated{other};

    for (int index = 0; index < size; index++) {
        rotated[index] = rotate(other[index], center, angle);
    }

    return rotated;
}

/**
 * @brief Rotate a object around a point by an orthogonal angle
 *
 * @param other Object to rotate
 * @param center Point in rotation center
 * @param angle Angle to rotate
 * @return Object rotated
 */
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
