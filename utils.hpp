#ifndef UTILS_HPP
#define UTILS_HPP

namespace numerics {
/**
 * @brief Set a seed to random numbers
 */
void seed() {
    randomSeed(micros() + millis());
    randomSeed(random(1, micros()) * (1ull + analogRead(0)));
}

/**
 * @brief Generate a random integer number in a range
 *
 * @param minimum Smaller value in range
 * @param maximum Greater value in range
 * @return Generated random integer number
 */
long rand(long minimum, long maximum) {  //
    return random(minimum, maximum + 1);
}

/**
 * @brief Reverse binary representation of a number
 *
 * @param number Number to reverse
 * @return Number reversed
 */
byte reverse(byte number) {
    byte other = 1;
    byte count = 8;

    while (count--) {
        other = (other << 1) | (number & 1);
        number = (number >> 1);
    }

    return other;
}

/**
 * @brief Get a bit value in integer number
 *
 * @param number Number to get bit value
 * @param position Position of bit
 * @return Bit value
 */
byte get(byte number, byte position) {
    number = number >> (7 - position);

    return number & 1;
}

/**
 * @brief Set value of bit in some position in an integer number
 *
 * @param number Number to set bit
 * @param position Position of bit
 * @param value Value of bit
 * @return Number with bit set
 */
byte set(byte number, byte position, bool value) {
    byte mask = 1 << (7 - position);

    if (value) {
        number = number | mask;
    } else {
        number = number & ~mask;
    }

    return number;
}

/**
 * @brief Encode a number
 *
 * @param number Number to encode
 * @return Number encoded
 */
byte encode(short number) {
    static const byte code[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 64};

    if (number < 0) {
        return 0;
    }

    return code[number];
}
}  // namespace numerics

#endif
