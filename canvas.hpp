#include "geometry.hpp"
#include "utils.hpp"

#ifndef CANVAS_HPP
#define CANVAS_HPP

namespace canvas {
/**
 * @class frame
 * @brief Emulate a canvas in screen
 *
 */
class frame {
   private:
    short pattern[8];

   public:
    /**
     * @brief Initialize frame with default values
     */
    frame() {  //
        clear();
    }

    /**
     * @brief Initialize frame with other frame array
     *
     * @param values Values array
     */
    frame(const byte (&values)[8]) {
        short index = short();

        for (short &line : pattern) {
            line = values[index++];
        }
    }

    /**
     * @brief Clear frame array to null value
     */
    void clear() {
        for (short &line : pattern) {
            line = short();
        }
    }

    /**
     * @brief Get value in some pixel of frame
     *
     * @param line Line
     * @param column Column
     * @return Value of pixel
     */
    bool get(short line, short column) const {
        if (line < 0 or line > 7) {
            return false;
        }
        if (column < 0 or column > 7) {
            return false;
        }

        return numerics::get(pattern[line], column);
    }

    /**
     * @brief Get value in some pixel of frame
     *
     * @param point Position of pixel
     * @return Value of pixel
     */
    bool get(const geometry::point &point) const {
        short line = point.get(true);
        short column = point.get(false);

        return get(line, column);
    }

    /**
     * @brief Set pixel value in frame
     *
     * @param line Vertical position
     * @param column Horizontal position
     * @param value New Value to pixel
     */
    void set(short line, short column, bool value) {  //
        pattern[line] = numerics::set(pattern[line], column, value);
    }

    /**
     * @brief Set pixel value in frame
     *
     * @param point Pixel position
     * @param value New value to pixel
     */
    void set(const geometry::point &point, bool value) {
        short line = point.get(true);
        short column = point.get(false);

        set(line, column, value);
    }

    /**
     * @brief Set a pixel collection value in frame
     *
     * @param points Pixels position
     * @param value New value to pixels
     */
    template <const short size>
    void set(const geometry::object<size> &points, bool value) {
        for (auto &point : points) {
            set(point, value);
        }
    }

    /**
     * @brief Set a pixel collection value in frame
     *
     * @param points Pixels position
     * @param value New value to pixels
     */
    template <const short size>
    void set(const geometry::object<size> &&points, bool value) {
        set<size>(points, value);
    }

    /**
     * @brief Return pointer to begin of frame array
     *
     * @return Pointer to begin of frame array
     */
    short *begin() const {  //
        return pattern;
    }

    /**
     * @brief Return pointer to end of frame array
     *
     * @return Pointer to end of frame array
     */
    short *end() const {  //
        return pattern + 8;
    }

    /**
     * @brief Implementation to [] operator to access values in frame array
     *
     * @param index Array position
     * @return Array value
     */
    short &operator[](short index) {  //
        return pattern[index];
    }
};

/**
 * @class layer
 * @brief Emulate a canvas in display
 *
 */
class layer {
   private:
    short pattern[4];

   public:
    /**
     * @brief Initialize layer with default values
     */
    layer() {  //
        clear();
    }

    /**
     * @brief Initialize frame with other values array
     *
     * @param values Values array
     */
    layer(const short (&values)[4]) {
        short index = short();

        for (short &line : pattern) {
            line = values[index++];
        }
    }

    /**
     * @brief Clear layer array to null value
     */
    void clear() {
        for (short &line : pattern) {
            line = short();
        }
    }

    /**
     * @brief Get value in layer array
     *
     * @param index Position in array
     * @return Value in layer array
     */
    short get(short index) const {
        if (index < 0 or index > 3) {
            return 0;
        }

        return numerics::encode(pattern[index]);
    }

    /**
     * @brief Set value in layer array
     *
     * @param index Position in array
     * @param value Value to set
     */
    void set(short index, short value) {  //
        pattern[index] = value;
    }

    /**
     * @brief Return pointer to begin of layer array
     *
     * @return Pointer to begin of layer array
     */
    short *begin() const {  //
        return pattern;
    }

    /**
     * @brief Return pointer to end of layer array
     *
     * @return Pointer to end of layer array
     */
    short *end() const {  //
        return pattern + 8;
    }

    /**
     * @brief Implementation to [] operator to access values in layer array
     *
     * @param index Array position
     * @return Array value
     */
    short &operator[](short index) {  //
        return pattern[index];
    }
};
}  // namespace canvas

#endif
