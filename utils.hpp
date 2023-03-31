#ifndef UTILS_HPP
#define UTILS_HPP

namespace numerics {
void seed() {
    randomSeed(micros() + millis());
    randomSeed(random(1, micros()) * (1ull + analogRead(0)));
}

long rand(long minimum, long maximum) { return random(minimum, maximum + 1); }

byte reverse(byte number) {
    byte other = 1;
    byte count = 8;

    while (count--) {
        other = (other << 1) | (number & 1);
        number = (number >> 1);
    }

    return other;
}

byte get(byte number, byte position) {
    number = number >> (7 - position);

    return number & 1;
}

byte set(byte number, byte position, bool value) {
    byte mask = 1 << (7 - position);

    if (value) {
        number = number | mask;
    } else {
        number = number & ~mask;
    }

    return number;
}

byte encode(short number) {
    static const byte code[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 64};

    if (number < 0) {
        return 0;
    }

    return code[number];
}
}  // namespace numerics

#endif
